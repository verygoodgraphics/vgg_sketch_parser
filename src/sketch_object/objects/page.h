#ifndef bd_sketch_page
#define bd_sketch_page
#include "src/sketch_object/objects/abstract_root.h"

class page : public abstract_root
{
public:
    page();

public:
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg);

private:
    static t_child child_;

    //充当默认 artboard 的 id
    static size_t default_artboard_id_;

private:
    /**
     * 创建具有默认属性的 artboard
     * 
     * @param out 保存输出的数据
     * 
     * @note 
     *  会对 out 首先进行 clear
     *  bounds frame layers 不会进行设置
    */
    static void create_default_artboard(nlohmann::json &out);    
};

#endif