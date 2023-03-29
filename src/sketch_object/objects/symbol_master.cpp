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

#include "./symbol_master.h"
#include "src/sketch_object/attrs/color_change.h"
#include "src/sketch_object/mask.h"
#include <regex>
#include <algorithm>
#include <array>

t_child symbol_master::child_;

symbol_master::symbol_master()
{
    init_child(this->child_, 1);
}

void symbol_master::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(vgg.empty());
    abstract_root::change(sketch, vgg);
    mask m_mask;

    assert(sketch.at("_class").get<string>() == "symbolMaster");

    try 
    {
        vgg["class"] = string("symbolMaster");
        vgg["hasBackgroundColor"] = get_json_value(sketch, "hasBackgroundColor", false);

        auto it = sketch.find("backgroundColor");
        if (it != sketch.end())
        {
            color_change::change(*it, vgg["backgroundColor"]);
        }
        else if (vgg["hasBackgroundColor"].get<bool>())
        {
            throw sketch_exception("fail to get symbol master background color");
        }

        vgg["includeBackgroundColorInInstance"] = get_json_value(sketch, "includeBackgroundColorInInstance", true);
        vgg["symbolID"] = sketch.at("symbolID").get<string>();
        vgg["allowsOverrides"] = get_json_value(sketch, "allowsOverrides", false);

        vgg["overrideProperties"] = nlohmann::json::array();
        it = sketch.find("overrideProperties");
        if (it != sketch.end())
        {
            symbol_master::override_properties_change(*it, vgg["overrideProperties"]);
        }
        
        vgg["childObjects"] = nlohmann::json::array();
        
        auto &layers = sketch.at("layers");
        for (auto &item : layers)
        {
            string name = item.at("_class").get<string>();

            auto it = child_.find(name);
            if (it != child_.end())
            {
                nlohmann::json out;
                it->second->change(item, out);

                m_mask.deal_mask(*it->second, out);

                assert(!out.empty());
                vgg["childObjects"].emplace_back(std::move(out));
            }
            else 
            {
                assert(name == "slice" || name == "MSImmutableHotspotLayer");
            }
        }

        /*
        sketch中未处理的属性包括:
        includeBackgroundColorInExport
        isFlowHome
        resizesContent
        presetDictionary
        */
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        assert(false);
        throw sketch_exception("fail to analyze symbol master");
    }       
}

void symbol_master::override_name_check(const string &str)
{
    //from override-name.yaml
    /*
    const static std::array<std::regex, 4> a_re = 
    { 
        std::regex("[0-9A-F]{8}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{12}((_stringValue$)|\\/)"),
        std::regex("[0-9A-F]{8}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{12}((_symbolID$)|\\/)"),
        std::regex("[0-9A-F]{8}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{12}((_image$)|\\/)"),
        std::regex("[0-9A-F]{8}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{12}((_layerStyle$)|\\/)")
    };

    if (!std::any_of(a_re.begin(), a_re.end(), [&str](const std::regex &re)
    {
        return std::regex_match(str, re);
    }))
    {
        throw sketch_exception("fail to match override name");
    }
    */

   //备注: 不对该项进行校验
}

void symbol_master::override_properties_change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg = nlohmann::json::array();

    try
    {
        if (!sketch.is_array())
        {
            throw;
        }

        for (auto &item : sketch)
        {
            assert(item.at("_class").get<string>() == "MSImmutableOverrideProperty");
            
            nlohmann::json vgg_item;
            vgg_item["class"] = string("MSImmutableOverrideProperty");
            vgg_item["canOverride"] = item.at("canOverride").get<bool>();
            vgg_item["overrideName"] = item.at("overrideName");
            symbol_master::override_name_check(vgg_item["overrideName"].get<string>());

            vgg.emplace_back(std::move(vgg_item));
        }
    }
    catch (sketch_exception &e)
    {
        throw e;
    }
    catch (...)
    {
        throw sketch_exception("fail to analyze override properties");
    }
}