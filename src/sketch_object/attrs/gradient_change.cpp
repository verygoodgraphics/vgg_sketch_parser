#include "./gradient_change.h"
#include "src/sketch_object/check.hpp"
#include "src/sketch_object/utils/point_string_change.h"
#include "src/sketch_object/attrs/color_change.h"

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

        int type = sketch.at("gradientType").get<int>();
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

    try
    {
        assert(sketch.is_array());

        nlohmann::json tem;

        for (auto &item : sketch)
        {
            assert(item.at("_class").get<string>() == "gradientStop");
            tem["class"] = std::string("gradientStop");
            tem["midPoint"] = 0.5;

            tem["position"] = item.at("position");
            range_check(tem["position"].get<double>(), 0.0, 1.0, "invalid gradient stop position");

            color_change::change(item.at("color"), tem["color"]);

            vgg.emplace_back(std::move(tem));
        }
    }
    catch (sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to change gradient stops");
    }
}
