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

#include "./shape_group.h"
#include "src/sketch_object/check.hpp"

t_child shape_group::child_;

shape_group::shape_group()
{
    init_child(child_, 2);
}

void shape_group::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    abstract_layer::change(sketch, vgg);
    assert(sketch.at("_class").get<string>() == "shapeGroup");

    vgg["class"] = string("path");

    auto &shape = vgg["shape"];
    shape["class"] = string("shape");
    get_json_value<int>(sketch, "windingRule", shape["windingRule"], "fail to get shape group winding rule");
    range_check(shape["windingRule"].get<int>(), 0, 1, "invalid winding rule");

    auto layers = get_json_item(sketch, "layers", "fail to get shape group layers");
    for (auto &item : *layers)
    {
        string name;
        get_json_value<string>(item, "_class", name, "fail to get shape_group child attr: _class");

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

    /*
    sketch中未处理的属性包括:
    hasClickThrough
    groupLayout
    */

}
