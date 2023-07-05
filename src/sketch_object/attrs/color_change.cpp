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

#include "./color_change.h"
#include "src/sketch_object/check.hpp"
#include "src/basic/get_json_value.hpp"

void color_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    assert(sketch.at("_class").get<string>() == "color");

    auto get = [](const nlohmann::json &sketch, const char *key, double default_value)
    {
        double value = get_json_value(sketch, key, 1.0);
        check::ins_.check_range(value, 0.0, 1.0, default_value, "color value invalid range");
        return value;
    };
    
    vgg["class"] = "color";
    vgg["alpha"] = get(sketch, "alpha", 1.0);
    vgg["red"] = get(sketch, "red", 0.0);
    vgg["green"] = get(sketch, "green", 0.0);
    vgg["blue"] = get(sketch, "blue", 0.0);

    //备注: sketch 中的 swatchID 没有处理
}

void color_change::get_default(nlohmann::json &out)
{
    out.clear();
    out["alpha"] = 1.0;
    out["red"] = 0.0;
    out["green"] = 0.0;
    out["blue"] = 0.0;
    out["class"] = "color";
}
