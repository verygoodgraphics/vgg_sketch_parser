#include "./context_settings_change.h"
#include "src/sketch_object/check.hpp"

void context_settings_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    try 
    {
        assert(sketch.at("_class").get<string>() == "graphicsContextSettings");

        vgg["class"] = string("graphicsContextSettings");

        vgg["blendMode"] = sketch.at("blendMode");
        range_check(vgg["blendMode"].get<int>(), 0, 17, "invalid blend mode");

        vgg["opacity"] = sketch.at("opacity");
        range_check(vgg["opacity"].get<double>(), 0.0, 1.0, "invalid opacity");

        vgg["isolateBlending"] = false;
        vgg["transparencyKnockoutGroup"] = 0;
    }
    catch (sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("context-settings change fail");
    }
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