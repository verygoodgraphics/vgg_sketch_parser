#ifndef bd_sketch_symbol_instance
#define bd_sketch_symbol_instance
#include "src/sketch_object/objects/abstract_layer.h"

class symbol_instance : public abstract_layer
{
public:
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg);

private:
    /**
     * 将 sketch 的输入, 转为符合 vgg-format 的输出
     * 
     * @param sketch sketch 的输入
     * @param vgg 符合 vgg-format 的输出
     * 
     * @exception sketch_exception
     * 
     * @note 会对 vgg 先进行 clear
    */
    static void override_values_change(const nlohmann::json &sketch, nlohmann::json &vgg);
};

#endif