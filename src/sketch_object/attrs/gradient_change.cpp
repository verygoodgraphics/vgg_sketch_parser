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

        //脏代码        
        try
        {
            point_string_change::change(sketch.at("from"), gradient["from"]);
        }
        catch(...)
        {
            gradient["from"].emplace_back(0.0);
            gradient["from"].emplace_back(0.0);

            // 备注: 碰到过 gradient isEnable 为 false 时, 缺少 from 的情况
            // check::ins_.add_error("failed to get gradient.from");
        }
        try 
        {
            point_string_change::change(sketch.at("to"), gradient["to"]);
        }
        catch(...)
        {
            gradient["to"].emplace_back(1.0);
            gradient["to"].emplace_back(1.0);
            check::ins_.add_error("failed to get gradient.to");            
        }

        auto it = sketch.find("stops");
        if (it != sketch.end())
        {
            gradient_change::gradient_stops_change(sketch.at("stops"), gradient["stops"]);
        }
        else
        {
            nlohmann::json stop_0;
            nlohmann::json stop_1;
            gradient_change::create_default_stops(stop_0);
            gradient_change::create_default_stops(stop_1);
            gradient["stops"] = nlohmann::json::array();
            gradient["stops"].emplace_back(std::move(stop_0));
            gradient["stops"].emplace_back(std::move(stop_1));
            check::ins_.add_error("failed to get gradient.stops");
        }
        
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

                //备注: 该值理论上为 1 时, sketch 原始文件里存的是 0, 就很诡异
                double elipse_length = get_json_value(sketch, "elipseLength", 1.0);
                elipse_length = elipse_length == 0 ? 1 : elipse_length;
                gradient["elipseLength"] = elipse_length;
                break;
            }

            case 2:
            {
                gradient["class"] = string("gradientAngular");
                // gradient["rotation"] = 0.0;
                gradient["elipseLength"] = 1;
                
                // 手动修改 from 和 to
                gradient["from"][0] = 0.5;
                gradient["from"][1] = 0.5;
                
                // 备注: 此处 to 的 x 不重要, 固定设为 1
                //gradient["to"][0] = bound_width >= bound_height ? 1.0 : (bound_height / bound_width);
                gradient["to"][0] = 1.0;
                gradient["to"][1] = 0.5;
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

        double position = get_json_value(item, "position", 0.0);
        check::ins_.check_range(position, 0.0, 1.0, 0.0, "invalid gradient stop position");
        tem["position"] = position;

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

void gradient_change::create_default_stops(nlohmann::json &out)
{
    out.clear();
    out["class"] = "gradientStop";
    color_change::get_default(out["color"]);
    out["position"] = 0.0;
    out["midPoint"] = 0.5;
}
