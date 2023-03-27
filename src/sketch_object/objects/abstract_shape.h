#ifndef bd_sketch_abstract_shape
#define bd_sketch_abstract_shape
#include "src/sketch_object/objects/abstract_layer.h"

class abstract_shape : public abstract_layer
{
public:
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg);

private:
    /**
     * 将 sketch 的输入, 转为符合 vgg-format 的输出
     * 
     * @param sketch_points sketch 点的集合
     * @param sketch_frame sketch 对象的 frame
     * @param vgg 符合 vgg-format 的输出
     * 
     * @exception sketch_exception
     * 
     * @note 会对 vgg 先进行 clear
    */
    static void curve_point_change
    (
        const nlohmann::json &sketch_points, 
        const nlohmann::json &sketch_frame, 
        nlohmann::json &vgg
    );
};

#endif