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

#include "./context_settings_change.h"
#include "src/sketch_object/check.hpp"
#include "src/basic/get_json_value.hpp"

void context_settings_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    assert(sketch.at("_class").get<string>() == "graphicsContextSettings");

    vgg["class"] = string("graphicsContextSettings");

    int blend_mode = get_json_value(sketch, "blendMode", 0);
    if (blend_mode == 1000)
    {
        // 备注: 存在该值为 1000 的情况
        blend_mode = 0;
    }
    check::ins_.check_range(blend_mode, 0, 17, 0, "invalid blend mode");
    vgg["blendMode"] = blend_mode;

    double opacity = get_json_value(sketch, "opacity", 0.0);
    check::ins_.check_range(opacity, 0.0, 1.0, 1.0, "invalid opacity");
    vgg["opacity"] = opacity;

    vgg["isolateBlending"] = false;
    vgg["transparencyKnockoutGroup"] = 0;
}

void context_settings_change::get_default(nlohmann::json &out)
{
    out.clear();
    out["class"] = string("graphicsContextSettings");
    out["blendMode"] = 0;
    out["opacity"] = 1.0;
    out["isolateBlending"] = false;
    out["transparencyKnockoutGroup"] = 0;
}