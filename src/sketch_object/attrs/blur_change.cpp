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

#include "./blur_change.h"
#include "src/sketch_object/check.hpp"
#include "src/sketch_object/utils/point_string_change.h"
#include "src/basic/get_json_value.hpp"

void blur_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    assert(sketch.at("_class").get<string>() == "blur");
    vgg["class"] = string("blur");

    int type = get_json_value(sketch, "type", 0);
    check::ins_.check_range(type, 0, 3, 0, "invalid blur type");
    vgg["type"] = type;

    vgg["isEnabled"] = get_json_value(sketch, "isEnabled", false);
    vgg["radius"] = get_json_value(sketch, "radius", 0.0);
    vgg["motionAngle"] = get_json_value(sketch, "motionAngle", 0.0);
    vgg["saturation"] = get_json_value(sketch, "saturation", 0.0);

    try
    {
        point_string_change::change(sketch.at("center"), vgg["center"]);
    }
    catch (...)
    {
        assert(false);
        vgg["center"].emplace_back(0);
        vgg["center"].emplace_back(0);
    }
}