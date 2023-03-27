#ifndef bd_sketch_polygon
#define bd_sketch_polygon
#include "src/sketch_object/objects/abstract_shape.h"

class polygon : public abstract_shape
{
public:
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg)
    {
        assert(sketch.at("_class").get<string>() == "polygon");
        abstract_shape::change(sketch, vgg);

        /*
        sketch中未处理的属性包括:
        numberOfPoints: 无需处理
        */
    }
};

#endif