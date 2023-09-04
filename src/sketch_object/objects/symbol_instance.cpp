/*
MIT License

Copyright (c) 2023 Very Good Graphics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "./symbol_instance.h"
#include "./symbol_master.h"
#include "./bitmap.h"
#include "./document.h"
#include "./text.h"
#include "src/sketch_object/attrs/color_change.h"
#include "src/sketch_object/check.hpp"
#include "boost/algorithm/string.hpp"
#include "src/sketch_object/attrs/fill_change.h"
#include <string_view>
#include <optional>

std::unordered_map<string, const nlohmann::json*> symbol_instance::objs_;

void symbol_instance::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(sketch.at("_class").get<string>() == "symbolInstance");
    abstract_layer::change(sketch, vgg);

    vgg["class"] = string("symbolInstance");
    vgg["overflow"] = 2;

    // 保留原始数据, 用于后续属性覆盖, 最终会删除该字段
    vgg["original_info"] = sketch;

    /*
    sketch中未处理的属性包括:
    scale: 无需处理, scale 变化的时候, frame 也会变化
    verticalSpacing
    horizontalSpacing
    preservesSpaceWhenHidden
    */
}

void symbol_instance::collection_objs(const nlohmann::json &obj)
{
    symbol_instance::objs_[obj.at("id")] = &obj;

    auto it = obj.find("childObjects");
    if (it != obj.end())
    {
        for (auto &item : *it)
        {
            symbol_instance::collection_objs(item);
        }
    }
    else 
    {
        if (obj.at("class").get<string>() == "path")
        {
            for (auto &sub_shape : obj.at("shape").at("subshapes"))
            {
                if (sub_shape.at("subGeometry").at("class") != "contour")
                {
                    symbol_instance::collection_objs(sub_shape.at("subGeometry"));
                }
            }
        }            
    }
}

void symbol_instance::deal_override_attr(nlohmann::json &vgg_format)
{
    symbol_instance::objs_.clear();
    const string attrs[] = { "frames", "references" };

    // 复制一份, 防止迭代器失效
    // const auto objs = vgg_format;
    for (auto &attr : attrs)
    {
        for (auto &item : vgg_format.at(attr))
        {
            symbol_instance::collection_objs(item);
        }        
    }

    for (auto &attr : attrs)
    {
        for (auto &item : vgg_format.at(attr))
        {
            symbol_instance::recursive_deal(item, symbol_instance::override_attr);
        }        
    }

    for (auto &attr : attrs)
    {
        for (auto &item : vgg_format.at(attr))
        {
            symbol_instance::recursive_deal(item, [](nlohmann::json &obj)
            {
                assert(obj.at("class") == "symbolInstance");
                assert(obj.find("original_info") != obj.end());
                obj.erase("original_info");                
            });
        }
    }
}

void symbol_instance::recursive_deal(nlohmann::json &obj, std::function<void(nlohmann::json &obj)> fun)
{
    auto it = obj.find("childObjects");
    if (it != obj.end())
    {
        for (auto &item : *it)
        {
            symbol_instance::recursive_deal(item, fun);
        }
    }
    else 
    {
        auto class_name = obj.at("class").get<string>();
        if (class_name == "path")
        {
            for (auto &sub_shape : obj.at("shape").at("subshapes"))
            {
                symbol_instance::recursive_deal(sub_shape.at("subGeometry"), fun);
            }
        }
        else if (class_name == "symbolInstance")
        {
            fun(obj);
        }
    }
}

void symbol_instance::override_attr(nlohmann::json &obj)
{
    assert(obj.at("class").get<string>() == "symbolInstance");
    assert(obj.find("original_info") != obj.end());
    const auto &sketch = obj.at("original_info");

    try 
    {
        auto it_master = symbol_master::get_master().find(sketch.at("symbolID"));
        if (it_master == symbol_master::get_master().end())
        {
            check::ins_.add_error("Failed to get the master associated with the instance");
            return;
        }

        auto &master = it_master->second;

        obj["masterId"] = master->master_data_.at("id");
        obj["overrideValues"] = nlohmann::json::array();

        if (!master->include_background_color_in_instance_ && !master->master_data_.at("style").at("fills").empty())
        {
            obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                { obj["masterId"] }, "style.fills", nlohmann::json::array()));
        }

        if (!master->allow_override_)
        {
            return;
        }

        auto it = sketch.find("overrideValues");
        if (it != sketch.end())
        {
            for (auto &item : *it)
            {
                // name 值示例: 
                //  015E3829-42A8-482E-A40E-90A5E5638E83_color:fill-0
                //  0B8DCD1B-9BEA-4DC3-88B9-D6A0BC5DD9C3/95C02DB7-5EC9-4A61-97B1-0FFBE0C30E83_stringValue                
                string name = item.at("overrideName").get<string>();
                nlohmann::json value = std::move(item.at("value"));

                // 单项开关判断
                {
                    string final_name = name;
                    auto pos = name.find_last_of('/');
                    if (pos != -1)
                    {
                        final_name = string(name.begin() + pos + 1, name.end());
                    }

                    auto it_switch = symbol_master::get_override_switch().find(final_name);
                    if (it_switch != symbol_master::get_override_switch().end() && !it_switch->second)
                    {
                        continue;
                    }
                }

                auto pos = std::find(name.begin(), name.end(), '_');
                if (pos == name.end())
                {
                    check::ins_.add_error("failed to get id from symbol-instance.overrideValues.overrideName");
                    continue;
                }

                string obj_id(name.begin(), pos);
                const nlohmann::json *now_obj = nullptr;

                vector<string> ids;
                boost::split(ids, obj_id, [](const char value){ return value == '/'; });
                if (ids.empty())
                {
                    check::ins_.add_error("failed to get id");
                    continue;                    
                }
                
                auto it = symbol_instance::objs_.find(ids.back());
                if (it != symbol_instance::objs_.end())
                {
                    now_obj = it->second;
                }
                else 
                {
                    // 备注: 遇到过一个文件, 会进入此处, 且对应的对象确实不存在
                    // check::ins_.add_error("failed to get master object.");
                    continue;
                }

                std::string_view sv(name.data() + (pos - name.begin()), name.end() - pos);
                if (boost::starts_with(sv, "_color:"))
                {
                    nlohmann::json color;
                    color_change::change(value, color);
                    sv.remove_prefix(strlen("_color:"));
                    string override_name = "style.";

                    const int total_item = 4;
                    const char *match_names[total_item] = 
                    {
                        "fill", "border", "shadow", "innershadow"
                    };
                    const char *out_names[total_item] = 
                    {
                        "fills", "borders", "shadows", "shadows"
                    };

                    int index = -1;
                    int match_id = 0;
                    for (; match_id < total_item; ++match_id)
                    {
                        if (boost::starts_with(sv, match_names[match_id]))
                        {
                            sv.remove_prefix(strlen(match_names[match_id]));
                            index = strtol(sv.data() + 1, nullptr, 10);
                            override_name += out_names[match_id];
                            break;
                        }
                    }

                    if (match_id == total_item)
                    {
                        check::ins_.add_error("failed to override color.");
                        continue;
                    }

                    if (match_id == 2 || match_id == 3)
                    {
                        int match_time = -1;
                        int now_id = -1;

                        for (auto &shadow : now_obj->at("style").at("shadows"))
                        {
                            if (match_id == 2 && !shadow.at("inner").get<bool>() ||
                                match_id == 3 && shadow.at("inner").get<bool>())
                            {
                                ++match_time;
                            }

                            ++now_id;

                            if (match_time == index)
                            {
                                index = now_id;
                                break;
                            }
                        }
                    }

                    override_name += (string(".") + std::to_string(index) + string(".color"));
                    obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                        ids, override_name, std::move(color)));
                }
                else if (boost::starts_with(sv, "_image"))
                {
                    string bitmap_filename;
                    bitmap::get_image_file_name(value, bitmap_filename);

                    if (now_obj->at("class") == "image")
                    {
                        // 此时, sketch 无法覆盖 image 的 fill 的 pattern
                        obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                            ids, "imageFileName", bitmap_filename));

                        // szn todo 当图像自身存在 fill 时, 进行图像内容覆盖, 产生的行为难以捉摸, 暂不处理
                    }
                    else 
                    {
                        // 覆盖 fill 的最后一个有效的 pattern 的 image
                        size_t index = 0;                        
                        auto &now_obj_fills = now_obj->at("style").at("fills");
                        for (auto it = now_obj_fills.rbegin(); it != now_obj_fills.rend(); ++it)
                        {
                            if (it->at("fillType").get<int>() == 2 && it->at("isEnabled").get<bool>())
                            {
                                auto override_name = string("style.fills.") + std::to_string(now_obj_fills.size() - index - 1) + string(".pattern.instance.imageFileName");
                                obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                                    ids, override_name, bitmap_filename));                                
                                break;
                            }

                            ++index;
                        }
                    }
                }
                else if(now_obj->at("class") == "text")
                {
                    if (value.is_string())
                    {
                        if (boost::starts_with(sv, "_stringValue"))
                        {
                            obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                                ids, "content", std::move(value)));
                        }
                        else if (boost::starts_with(sv, "_textSize"))
                        {
                            obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                                ids, "attr.*.size", strtof(value.get<string>().c_str(), nullptr)));
                        }
                        else if (boost::starts_with(sv, "_textDecoration"))
                        {
                            string str = value.get<string>();
                            if (str == "underline")
                            {
                                obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                                    ids, "attr.*.underline", 1));

                                obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                                    ids, "attr.*.linethrough", false));                                
                            }
                            else if (str == "strikethrough")
                            {
                                obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                                    ids, "attr.*.underline", 0));

                                obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                                    ids, "attr.*.linethrough", true));    
                            }                                         
                        }                    
                        else if (boost::starts_with(sv, "_textHAlign"))
                        {
                            string str = value.get<string>();
                            int horizon = -1;

                            if (str == "center")
                            {
                                horizon = 2;
                            }
                            else if (str == "right")
                            {
                                horizon = 1;
                            }
                            else if (str == "justified")
                            {
                                horizon = 3;
                            }
                            else 
                            {
                                // left
                                horizon = 0;
                            }

                            obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                                ids, "attr.*.horizontalAlignment", horizon));
                        }
                        else if (boost::starts_with(sv, "_textStyle"))
                        {
                            obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                                ids, "style", nlohmann::json(string("referenced_style_") + value.get<string>())));

                            obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                                ids, "verticalAlignment", nlohmann::json(document::get_text_vertical_align(value.get<string>()))));
                        }
                        else 
                        {
                            assert(false);
                            check::ins_.add_error("Unhandled text property overrides (string)");
                        }                          
                    }
                    else if (boost::starts_with(sv, "_textColor"))
                    {
                        nlohmann::json color;
                        color_change::change(value, color);

                        obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                            ids, "attr.*.fills.*.color", std::move(color)));
                    }
                    else if (boost::starts_with(sv, "_textWeight"))
                    {
                        // szn todo 暂时不知道如何解析
                    }
                    else 
                    {
                        assert(false);
                        check::ins_.add_error("Unhandled text property overrides");
                    }                    
                }
                else if (boost::starts_with(sv, "_symbolID"))
                {
                    assert(now_obj->at("class") == "symbolInstance");
                    auto it = symbol_master::get_master().find(value.get<string>());
                    if (it != symbol_master::get_master().end())
                    {
                        obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                            ids, "masterId", nlohmann::json(it->second->master_data_.at("id"))));                        
                    }
                    else
                    {
                        // 备注: 有一个文件能进到此处, 其 override value 为空
                        // check::ins_.add_error("when override symbol, failed to get master");
                    }
                }
                else if (boost::starts_with(sv, "_layerStyle"))
                {
                    obj["overrideValues"].emplace_back(symbol_instance::create_override_value(
                        ids, "style", nlohmann::json(string("referenced_style_") + value.get<string>())));
                }
                else if (boost::starts_with(sv, "_fillColor"))
                {
                    nlohmann::json color;
                    color_change::change(value, color);    

                    nlohmann::json fills = nlohmann::json::array();
                    fills.emplace_back(fill_change::construct_from_color(std::move(color)));
                    obj["overrideValues"].emplace_back(symbol_instance::create_override_value(ids, "style.fills", std::move(fills)));                              
                }
                else 
                {
                    assert(false);
                    check::ins_.add_error("Unhandled property overrides");
                }
            }
        }
    }
    catch(...)
    {
        check::ins_.add_error("failed to override symbol instance attr");
    }
}

nlohmann::json symbol_instance::create_override_value(const vector<string> &obj_id,
    const string &override_name, nlohmann::json &&override_value)
{
    nlohmann::json out;
    out["class"] = string("overrideValue");
    
    out["objectId"] = nlohmann::json::array();
    for (auto &item : obj_id)
    {
        out["objectId"].emplace_back(item);
    }

    out["overrideName"] = override_name;
    out["overrideValue"] = std::move(override_value);
    return out;
}
