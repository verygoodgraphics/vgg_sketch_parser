#ifndef bd_sketch_triangle
#define bd_sketch_triangle
#include "src/sketch_object/objects/abstract_shape.h"

class triangle : public abstract_shape
{
public:
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg)
    {
        assert(sketch.at("_class").get<string>() == "triangle");
        abstract_shape::change(sketch, vgg);
        
        /*
        sketch中未处理的属性包括:
        isEquilateral: 无需处理
        */
    }
};

#endif