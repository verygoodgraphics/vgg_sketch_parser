#ifndef bd_sketch_text
#define bd_sketch_text
#include "src/sketch_object/objects/abstract_layer.h"

class text : public abstract_layer
{
public:    
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg);

private:
    /**
     * 将 sketch 的输入, 转为符合 vgg-format 的输出
     * 
     * @param sketch sketch 的输入 (attributed-string.schema.yaml)
     * @param vgg 符合 vgg-format 的输出
     * 
     * @exception sketch_exception
     * 
     * @note 不会对 vgg 进行 clear
    */
    static void attributed_string_change(const nlohmann::json &sketch, nlohmann::json &vgg);

    /**
     * 将 sketch 的输入, 转为符合 vgg-format 的输出
     * 
     * @param sketch sketch 的输入 (string-attribute.schema.yaml)
     * @param vgg 符合 vgg-format 的输出
     * 
     * @exception sketch_exception
     * 
     * @note 不会对 vgg 进行 clear
    */
    static void string_attribute_change(const nlohmann::json &sketch, nlohmann::json &vgg);

    /**
     * 将 sketch 的输入, 转为符合 vgg-format 的输出
     * 
     * @param sketch sketch 的输入 (font-descriptor.schema.yaml)
     * @param vgg 符合 vgg-format 的输出
     * 
     * @exception sketch_exception
     * 
     * @note 不会对 vgg 进行 clear
    */
    static void font_descriptor_change(const nlohmann::json &sketch, nlohmann::json &vgg);

    /**
     * 将 sketch 的输入, 转为符合 vgg-format 的输出
     * 
     * @param sketch sketch 的输入 (paragraph-style.schema.yaml 或 空json)
     * @param vgg 符合 vgg-format 的输出
     * 
     * @exception sketch_exception
     * 
     * @note 不会对 vgg 进行 clear
    */
    static void paragraph_style_change(const nlohmann::json &sketch, nlohmann::json &vgg);

    /**
     * 将 sketch 的输入, 转为符合 vgg-format 的输出
     * 
     * @param sketch sketch 的输入
     * @param vgg 符合 vgg-format 的输出
     * 
     * @exception sketch_exception
     * 
     * @note 不会对 vgg 进行 clear
    */
    static void text_style_change(const nlohmann::json &sketch, nlohmann::json &vgg);
};

#endif
