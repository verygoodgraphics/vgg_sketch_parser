#include "./color_control_change.h"
#include "src/sketch_object/check.hpp"

void color_control_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    try 
    {
        assert(sketch.at("_class").get<string>() == "colorControls");

        vgg["brightness"] = sketch.at("brightness");
        vgg["contrast"] = sketch.at("contrast");
        vgg["hue"] = sketch.at("hue");
        vgg["saturation"] = sketch.at("saturation");

        for (auto &item : vgg)
        {
            range_check(item.get<double>(), -100.0, 100.0, "invalid color control");
        }

        vgg["class"] = string("colorControls");
        vgg["isEnabled"] = sketch.at("isEnabled").get<bool>();
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to change color control");
    }
}
