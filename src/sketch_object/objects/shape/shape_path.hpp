#ifndef bd_sketch_shape_path
#define bd_sketch_shape_path
#include "src/sketch_object/objects/abstract_shape.h"

class shape_path : public abstract_shape
{
public:
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg)
    {
        assert(sketch.at("_class").get<string>() == "shapePath");
        abstract_shape::change(sketch, vgg);
    }
};

#endif