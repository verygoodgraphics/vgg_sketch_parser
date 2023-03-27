#ifndef bd_sketch_rectangle
#define bd_sketch_rectangle
#include "src/sketch_object/objects/abstract_shape.h"

class rectangle : public abstract_shape
{
public:
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg)
    {
        assert(sketch.at("_class").get<string>() == "rectangle");
        abstract_shape::change(sketch, vgg);
        
        /*
        sketch中未处理的属性包括:
        fixedRadius
        hasConvertedToNewRoundCorners
        needsConvertionToNewRoundCorners
        */
    }
};

#endif