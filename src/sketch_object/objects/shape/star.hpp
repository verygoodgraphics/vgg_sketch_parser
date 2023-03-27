#ifndef bd_sketch_star
#define bd_sketch_star
#include "src/sketch_object/objects/abstract_shape.h"

class star : public abstract_shape
{
public:
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg)
    {
        assert(sketch.at("_class").get<string>() == "star");
        abstract_shape::change(sketch, vgg);
        
        /*
        sketch中未处理的属性包括:
        numberOfPoints: 无需处理
        radius: 无需处理
        */
    }
};

#endif