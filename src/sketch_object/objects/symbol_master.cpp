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
#include "src/sketch_object/check.hpp"
#include "src/sketch_object/attrs/fill_change.h"
#include <regex>
#include <algorithm>
#include <array>

t_child symbol_master::child_;
symbol_master::t_master_info symbol_master::masters_;
std::unordered_map<string, bool> symbol_master::override_switch_;

symbol_master::symbol_master()
{
    init_child(this->child_);
}

void symbol_master::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(vgg.empty());
    abstract_root::change(sketch, vgg);
    mask m_mask;

    assert(sketch.at("_class").get<string>() == "symbolMaster");

    string symbol_id;
    unique_ptr<symbol_master_attr> master_attr(new symbol_master_attr);
    vgg["overflow"] = 2;

    try 
    {
        vgg["class"] = string("symbolMaster");

        if (get_json_value(sketch, "hasBackgroundColor", false))
        {
            auto it = sketch.find("backgroundColor");
            if (it != sketch.end())
            {
                nlohmann::json color;
                color_change::change(*it, color);
                auto fill = fill_change::construct_from_color(std::move(color));

                assert(vgg.at("style").at("fills").empty());
                vgg.at("style").at("fills").emplace_back(std::move(fill));
            }
        }

        master_attr->include_background_color_in_instance_ = get_json_value(sketch, "includeBackgroundColorInInstance", true);
        master_attr->allow_override_ = get_json_value(sketch, "allowsOverrides", false);
        symbol_id = sketch.at("symbolID").get<string>();

        auto it = sketch.find("overrideProperties");
        if (it != sketch.end())
        {
            for (auto &item : *it)
            {
                try 
                {
                    assert(symbol_master::override_switch_.find(item.at("overrideName").get<string>()) == symbol_master::override_switch_.end());
                    symbol_master::override_switch_[item.at("overrideName").get<string>()] = item.at("canOverride").get<bool>();
                }
                catch(...)
                {
                    check::ins_.add_error("failed to change symbol-master.overrideProperties");
                }
            }
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

    if (!symbol_id.empty())
    {
        master_attr->master_data_ = vgg;
        symbol_master::masters_[symbol_id] = std::move(master_attr);
    }
}

void symbol_master::clear_master() 
{ 
    symbol_master::override_switch_.clear(); 
    symbol_master::masters_.clear(); 
}
