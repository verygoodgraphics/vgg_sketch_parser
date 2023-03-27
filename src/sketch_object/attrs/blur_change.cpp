#include "./blur_change.h"
#include "src/sketch_object/check.hpp"
#include "src/sketch_object/utils/point_string_change.h"

void blur_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    try 
    {
        assert(sketch.at("_class").get<string>() == "blur");

        vgg["class"] = string("blur");

        vgg["type"] = sketch.at("type");
        range_check(vgg["type"].get<int>(), 0, 3, "invalid blur type");

        vgg["isEnabled"] = sketch.at("isEnabled").get<bool>();
        vgg["radius"] = sketch.at("radius").get<double>();
        vgg["motionAngle"] = sketch.at("motionAngle").get<double>();
        point_string_change::change(sketch.at("center"), vgg["center"]);
        vgg["saturation"] = sketch.at("saturation").get<double>();
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to change blur");
    }
}