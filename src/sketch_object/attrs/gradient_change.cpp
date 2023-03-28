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

#include "./gradient_change.h"
#include "src/sketch_object/check.hpp"
#include "src/sketch_object/utils/point_string_change.h"
#include "src/sketch_object/attrs/color_change.h"
#include "src/basic/get_json_value.hpp"

void gradient_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    try 
    {
        assert(sketch.at("_class").get<string>() == "gradient");

        vgg["class"] = string("gradient");

        auto &gradient = vgg["instance"];
        point_string_change::change(sketch.at("from"), gradient["from"]);
        point_string_change::change(sketch.at("to"), gradient["to"]);
        gradient_change::gradient_stops_change(sketch.at("stops"), gradient["stops"]);
        gradient["invert"] = false;

        int type = get_json_value(sketch, "gradientType", 0);
        switch (type)
        {
            case 0:
            {
                gradient["class"] = string("gradientLinear");
                break;
            }

            case 1:
            {
                gradient["class"] = string("gradientRadial");
                gradient["elipseLength"] = sketch.at("elipseLength").get<double>();
                break;
            }

            case 2:
            {
                gradient["class"] = string("gradientAngular");
                gradient["rotation"] = 0.0;
                break;
            }
            
            default:
            {
                throw sketch_exception("invalid gradient type");
            }
        }
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("gradient change fail");
    }
}

void gradient_change::gradient_stops_change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();
    assert(sketch.is_array());

    nlohmann::json tem;

    for (auto &item : sketch)
    {
        assert(item.at("_class").get<string>() == "gradientStop");
        tem["class"] = std::string("gradientStop");
        tem["midPoint"] = 0.5;

        tem["position"] = get_json_value(item, "position", 0.0);
        range_check(tem["position"].get<double>(), 0.0, 1.0, "invalid gradient stop position");

        try 
        {
            color_change::change(item.at("color"), tem["color"]);
        }
        catch(...)
        {
            color_change::get_default(tem["color"]);
        }

        vgg.emplace_back(std::move(tem));
    }
}
