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

#include "./shadow_change.h"
#include "src/sketch_object/attrs/color_change.h"
#include "src/sketch_object/attrs/context_settings_change.h"
#include "src/basic/get_json_value.hpp"
#include "src/sketch_object/check.hpp"

void shadow_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    assert(sketch.at("_class").get<string>() == "shadow" || 
        sketch.at("_class").get<string>() == "innerShadow");

    vgg["class"] = string("shadow");
    vgg["isEnabled"] = get_json_value(sketch, "isEnabled", false);

    try 
    {
        color_change::change(sketch.at("color"), vgg["color"]);
    }
    catch(...)
    {
        color_change::get_default(vgg["color"]);
        check::ins_.add_error("failed to get shadow.color");
    }
    
    vgg["offsetX"] = get_json_value(sketch, "offsetX", 0.0); 
    vgg["offsetY"] = -get_json_value(sketch, "offsetY", 0.0); 
    vgg["blur"] = get_json_value(sketch, "blurRadius", 0.0);
    vgg["spread"] = get_json_value(sketch, "spread", 0.0);

    try 
    {
        context_settings_change::change(sketch.at("contextSettings"), vgg["contextSettings"]);
    }
    catch(...)
    {
        context_settings_change::get_default(vgg["contextSettings"]);
        check::ins_.add_error("failed to get shadow.contextSettings");
    }

    vgg["inner"] = sketch.at("_class").get<string>() != "shadow";
}
