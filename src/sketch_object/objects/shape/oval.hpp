#ifndef bd_sketch_oval
#define bd_sketch_oval
#include "src/sketch_object/objects/abstract_shape.h"

class oval : public abstract_shape
{
public:
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg)
    {
        assert(sketch.at("_class").get<string>() == "oval");
        abstract_shape::change(sketch, vgg);
    }
};

#endif