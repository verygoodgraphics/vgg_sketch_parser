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

#include "./artboard.h"
#include "src/sketch_object/attrs/color_change.h"
#include "src/sketch_object/attrs/rect_change.h"
#include "src/sketch_object/mask.h"

t_child artboard::child_;

artboard::artboard()
{
    init_child(this->child_, 1);
}

void artboard::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(vgg.empty());

    abstract_root::change(sketch, vgg);

    mask m_mask;

    assert(sketch.at("_class").get<string>() == "artboard");
    vgg["class"] = "artboard";
    vgg["hasBackgroundColor"] = get_json_value(sketch, "hasBackgroundColor", false);

    try 
    {
        auto it = sketch.find("backgroundColor");
        if (it != sketch.end())
        {
            color_change::change(*it, vgg["backgroundColor"]);
        }
        else if (vgg["hasBackgroundColor"].get<bool>())
        {
            throw sketch_exception("fail to get artboard background color");
        }
        
        nlohmann::json vgg_layer;
        abstract_layer::create_default_layer(vgg_layer);

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
                vgg_layer["childObjects"].emplace_back(std::move(out));
            }
            else 
            {
                assert(name == "slice" || name == "MSImmutableHotspotLayer");
            }
        }

        //artboard的起点固定为(0, 0)
        vgg["frame"].at("x") = 0.0;
        vgg["frame"].at("y") = 0.0;
        rect_change::get_default_matrix(vgg["matrix"]);

        vgg_layer["frame"] = vgg.at("frame");
        vgg_layer["bounds"] = vgg.at("bounds");
        vgg["layers"].emplace_back(std::move(vgg_layer));

        /*
        sketch中未处理的属性包括: 
        includeBackgroundColorInExport
        isFlowHome
        resizesContent
        prototypeViewport
        */
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        assert(false);
        throw sketch_exception("fail to analyze artboard");
    }
}