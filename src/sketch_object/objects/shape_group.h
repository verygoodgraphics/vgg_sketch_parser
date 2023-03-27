#ifndef bd_sketch_shape_group
#define bd_sketch_shape_group
#include "src/sketch_object/objects/abstract_layer.h"

//shape-group, 其与 abstract_group 的关系, 参考 abstract_group.h 中的说明
class shape_group : public abstract_layer
{
public:
    shape_group();

public:
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg);

private:
    static t_child child_;
};

#endif