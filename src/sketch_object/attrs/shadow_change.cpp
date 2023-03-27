#include "./shadow_change.h"
#include "src/sketch_object/attrs/color_change.h"
#include "src/sketch_object/attrs/context_settings_change.h"

void shadow_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    try 
    {
        assert(sketch.at("_class").get<string>() == "shadow" || sketch.at("_class").get<string>() == "innerShadow");

        vgg["class"] = string("shadow");
        vgg["isEnabled"] = sketch.at("isEnabled").get<bool>();
        color_change::change(sketch.at("color"), vgg["color"]);
        vgg["offsetX"] = sketch.at("offsetX").get<double>();
        vgg["offsetY"] = sketch.at("offsetY").get<double>();
        vgg["blur"] = sketch.at("blurRadius").get<double>();
        vgg["spread"] = sketch.at("spread").get<double>();
        context_settings_change::change(sketch.at("contextSettings"), vgg["contextSettings"]);
        vgg["inner"] = sketch.at("_class").get<string>() != "shadow";
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to change shadow");
    }
}
