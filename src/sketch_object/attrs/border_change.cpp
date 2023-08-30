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

#include "./border_change.h"
#include "src/sketch_object/check.hpp"
#include "src/sketch_object/attrs/color_change.h"
#include "src/sketch_object/attrs/gradient_change.h"
#include "src/sketch_object/attrs/context_settings_change.h"
#include "src/basic/get_json_value.hpp"

void border_change::change(const nlohmann::json &sketch_border,
    const nlohmann::json *sketch_border_option, nlohmann::json &vgg)
{
    vgg.clear();

    assert(sketch_border.at("_class").get<string>() == "border");

    vgg["class"] = "border";
    vgg["isEnabled"] = get_json_value(sketch_border, "isEnabled", false);

    auto fill_type = border_change::fill_type_change(get_json_value(sketch_border, "fillType", 0));

    auto it_color = sketch_border.find("color");
    if (it_color != sketch_border.end())
    {
        color_change::change(sketch_border.at("color"), vgg["color"]);
    }

    int position = get_json_value(sketch_border, "position", 0);
    check::ins_.check_range(position, 0, 2, 0, "invalid border position");
    vgg["position"] = position;

    vgg["thickness"] = get_json_value(sketch_border, "thickness", 1);

    //备注: 在 sketch-schema 1.0 中, contextSettings 和 gradient 是可选的
    {
        auto it = sketch_border.find("contextSettings");
        if (it != sketch_border.end())
        {
            context_settings_change::change(sketch_border.at("contextSettings"), vgg["contextSettings"]);
        }
        else
        {
            context_settings_change::get_default(vgg["contextSettings"]);
        }

        it = sketch_border.find("gradient");
        if (it != sketch_border.end())
        {
            gradient_change::change(sketch_border.at("gradient"), vgg["gradient"]);
        }
        else if (1 == fill_type) 
        {
            // 备注: 存在一个用例, 其 isEnabled 为 false, 其 fillType 为渐变, 但 gradient 不存在, 此时将其修改为纯色
            // 脏代码: 更好的策略是构造一个默认的 gradient
            // check::ins_.add_error("failed to create border.gradient");
            fill_type = 0;
        }
    }

    //备注: 确保 color 的存在性, 当 fillType 不在预期内时, 强制使用 color
    if (!fill_type && vgg.find("color") == vgg.end())
    {
        color_change::get_default(vgg["color"]);
    }
    vgg["fillType"] = fill_type;

    vgg["style"] = 0;
    vgg["dashedPattern"] = nlohmann::json::array();
    vgg["dashedOffset"] = 0.0;

    // 以下的默认值采用 ai 的标准
    vgg["lineCapStyle"] = 0;
    vgg["lineJoinStyle"] = 0;
    vgg["miterLimit"] = 4.0;
    vgg["flat"] = 0.0;
    
    if (sketch_border_option)
    {
        assert(sketch_border_option->at("_class").get<string>() == "borderOptions");

        // 备注: 由于 sketch 从旧版过来的值就是 false, 但是却需要对应的效果, 所以都当其为 true 进行处理
        //if (get_json_value(*sketch_border_option, "isEnabled", false))
        {
            try 
            {
                auto &dash_pattern = sketch_border_option->at("dashPattern");
                assert(dash_pattern.is_array());

                for (auto &item : dash_pattern)
                {
                    vgg["dashedPattern"].emplace_back(item.get<double>());
                }
                vgg["style"] = dash_pattern.empty() ? 0 : 2;
            }
            catch(...)
            {
            }

            int line_cap = get_json_value(*sketch_border_option, "lineCapStyle", 0);
            check::ins_.check_range(line_cap, 0, 2, 0, "invalid line cap");
            vgg["lineCapStyle"] = line_cap;
            
            int line_join = get_json_value(*sketch_border_option, "lineJoinStyle", 0);
            check::ins_.check_range(line_join, 0, 2, 0, "invalid line join");
            vgg["lineJoinStyle"] = line_join;
        }
    }
}

int border_change::fill_type_change(int sketch_fill_type)
{
    if (!sketch_fill_type || 1 == sketch_fill_type)
    {
        return sketch_fill_type;
    }
    else if (4 == sketch_fill_type || 5 == sketch_fill_type)
    {
        // 备注: 有遇到过该值为5, 实际为 pattern 模式填充的情况
        return 2;
    }

    //assert(false);
    //throw sketch_exception("invalid fill type");
    //return -1;
    check::ins_.add_error("invalid fill type");
    return 0;
}