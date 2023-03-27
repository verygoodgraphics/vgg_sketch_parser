#ifndef bd_sketch_abstract_group
#define bd_sketch_abstract_group
#include "src/sketch_object/objects/abstract_layer.h"

/*
备注: 在 sketch 中, abstract_group 有两个子类型, group 和 shape-group(即复合对象),
但是根据 vgg-format 的语义, 复合对象不是 group, 所以这里将 abstract_group 和 group 进行等价
*/
class abstract_group : public abstract_layer
{
public:
    abstract_group();    

public:
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg);

private:
    static t_child child_;
};

typedef abstract_group group;

#endif