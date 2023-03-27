#include "./color_change.h"
#include "src/sketch_object/check.hpp"

void color_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    try 
    {
        assert(sketch.at("_class").get<string>() == "color");

        vgg["alpha"] = sketch.at("alpha");
        vgg["red"] = sketch.at("red");
        vgg["green"] = sketch.at("green");
        vgg["blue"] = sketch.at("blue");
        for (auto &item : vgg)
        {
            range_check(item.get<double>(), 0.0, 1.0, "color value invalid range");
        }

        vgg["class"] = "color";
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze color");
    }

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
