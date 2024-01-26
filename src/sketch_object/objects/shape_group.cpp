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

#include "./shape_group.h"
#include "src/sketch_object/check.hpp"

t_child shape_group::child_;

shape_group::shape_group()
{
    init_child(child_);
}

void update_mark_type(nlohmann::json &obj, int start_mark_type, int end_mark_type)
{
    if (obj["class"] != "path")
    {
        return;
    }

    for (auto &child : obj["shape"]["subshapes"])
    {
        auto type = child["subGeometry"]["class"].get<std::string>();
        if (type == "path")
        {
            update_mark_type(child["subGeometry"], start_mark_type, end_mark_type);
        }
        else if (type == "contour") 
        {
            auto &points = child["subGeometry"]["points"];
            if (!points.empty())
            {
                points.front()["markType"] = start_mark_type;
                points.back()["markType"] = end_mark_type;
            }
        }
    }
}

void shape_group::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    abstract_layer::change(sketch, vgg);
    assert(sketch.at("_class").get<string>() == "shapeGroup");

    try 
    {
        vgg["class"] = string("path");

        auto &shape = vgg["shape"];
        shape["class"] = string("shape");

        int winding_rule = get_json_value(sketch, "windingRule", 1);
        check::ins_.check_range(winding_rule, 0, 1, 1, "invalid winding rule");
        shape["windingRule"] = winding_rule;
        shape["subshapes"] = nlohmann::json::array();

        auto &layers = sketch.at("layers");
        for (auto &item : layers)
        {
            string name = item.at("_class").get<string>();
            auto it = child_.find(name);
            
            if (it != child_.end())
            {
                nlohmann::json out;
                it->second->change(item, out);
                assert(!out.empty());

                nlohmann::json sub_shape;
                sub_shape["class"] = string("subshape");
                sub_shape["subGeometry"] = std::move(out);
                sub_shape["booleanOperation"] = it->second->boolean_operation_;

                shape["subshapes"].emplace_back(std::move(sub_shape));
            }
            else 
            {
                assert(name == "slice" || name == "MSImmutableHotspotLayer");
            }
        }

        update_mark_type(vgg, 
            get_json_value(sketch.at("style"), "startMarkerType", 0),
            get_json_value(sketch.at("style"), "endMarkerType", 0));
            
        /*
        sketch中未处理的属性包括:
        hasClickThrough
        groupLayout
        */
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        assert(false);
        throw sketch_exception("fail to analyze shape group");
    }
}
