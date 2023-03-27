#ifndef bd_sketch_bitmap
#define bd_sketch_bitmap
#include "src/sketch_object/objects/abstract_layer.h"

class bitmap : public abstract_layer
{
public:    
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg);

public:
    /**
     * 从输入的 json 中, 获取 image 的文件名
     * 
     * @param sketch 输入的 json, 是 file-ref.schema.yaml 或 data-ref.schema.yaml
     * @param out 输出的 image 的文件名
     * 
     * @exception sketch_exception
    */
    static void get_image_file_name(const nlohmann::json &sketch, string &out);
};

#endif