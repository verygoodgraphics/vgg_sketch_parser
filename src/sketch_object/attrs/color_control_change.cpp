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

#include "./color_control_change.h"
#include "src/sketch_object/check.hpp"
#include "src/basic/get_json_value.hpp"

void color_control_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    assert(sketch.at("_class").get<string>() == "colorControls");

    vgg["brightness"] = get_json_value(sketch, "brightness", 0.0);
    vgg["contrast"] = get_json_value(sketch, "contrast", 0.0);
    vgg["hue"] = get_json_value(sketch, "hue", 0.0);
    vgg["saturation"] = get_json_value(sketch, "saturation", 0.0);

    for (auto &item : vgg)
    {
        range_check(item.get<double>(), -100.0, 100.0, "invalid color control");
    }

    vgg["class"] = string("colorControls");
    vgg["isEnabled"] = get_json_value(sketch, "isEnabled", false);
}
