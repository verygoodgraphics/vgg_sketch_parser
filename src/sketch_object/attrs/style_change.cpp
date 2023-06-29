﻿/*
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

#include "./style_change.h"
#include "src/sketch_object/attrs/border_change.h"
#include "src/sketch_object/attrs/fill_change.h"
#include "src/sketch_object/attrs/blur_change.h"
#include "src/sketch_object/attrs/shadow_change.h"
#include "src/sketch_object/attrs/context_settings_change.h"

void style_change::change(const nlohmann::json &sketch, nlohmann::json &vgg, nlohmann::json &context_setting,
    double bound_width, double bound_height)
{
    vgg.clear();

    auto attr_change = [&sketch, &vgg](decltype(shadow_change::change)* fun, const char *sketch_key, const char *vgg_key)
    {
        auto it = sketch.find(sketch_key);
        if (it == sketch.end())
        {
            return;
        }

        nlohmann::json out;
        if (it->is_array())
        {
            for (auto &item : *it)
            {
                fun(item, out);
                vgg[vgg_key].emplace_back(std::move(out));
            }
        }
        else 
        {
            fun(*it, out);
            vgg[vgg_key].emplace_back(std::move(out));
        }
    };

    try 
    {
        assert(sketch.at("_class").get<string>() == "style");
        style_change::get_default(vgg);

        //borders
        auto it = sketch.find("borders");
        if (it != sketch.end())
        {
            //borderOptions 在 sketch-schema 1.0 中是可选的
            auto it_border_option = sketch.find("borderOptions");
            const nlohmann::json *border_option = it_border_option == sketch.end() ? nullptr : &*it_border_option;

            for (auto &item : *it)
            {
                nlohmann::json tem;
                border_change::change(item, border_option, tem, bound_width, bound_height);
                
                assert(tem.find("miterLimit") != tem.end());
                tem["miterLimit"] = sketch.at("miterLimit").get<double>();

                vgg["borders"].emplace_back(std::move(tem));
            }
        }

        //blur
        attr_change(blur_change::change, "blur", "blurs");

        //fill
        {
            //attr_change(fill_change::change, "fills", "fills");

            // 脏代码
            auto it = sketch.find("fills");
            if (it != sketch.end())
            {
                nlohmann::json out;
                if (it->is_array())
                {
                    for (auto &item : *it)
                    {
                        fill_change::change(item, out, bound_width, bound_height);
                        vgg["fills"].emplace_back(std::move(out));
                    }
                }
                else
                {
                    fill_change::change(*it, out, bound_width, bound_height);
                    vgg["fills"].emplace_back(std::move(out));
                }
            }
        }

        //shadows 
        attr_change(shadow_change::change, "shadows", "shadows");

        //innerShadows 在 sketch-schema 1.0 中是可选的
        attr_change(shadow_change::change, "innerShadows", "shadows");

        if (sketch.find("contextSettings") != sketch.end())
        {
            context_settings_change::change(sketch.at("contextSettings"), context_setting);
        }
        else 
        {
            context_settings_change::get_default(context_setting);
        }
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to change style");
    }

    /*
    备注: 未处理的项包括
    do_objectID
    startMarkerType
    endMarkerType
    */

    //textStyle: 在 text.cpp 中被处理了
    //windingRule: 在 abstract_shapp.cpp 中被处理了
}

void style_change::get_default(nlohmann::json &out)
{
    out.clear();
    out["class"] = string("style");
    out["borders"] = nlohmann::json::array();
    out["fills"] = nlohmann::json::array();
    out["blurs"] = nlohmann::json::array();
    out["shadows"] = nlohmann::json::array();
}