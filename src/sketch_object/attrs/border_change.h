#ifndef bd_sketch_border_change
#define bd_sketch_border_change
#include "src/basic/define.h"
#include "nlohmann/json.hpp"

class border_change
{
public:
    /**
     * 将 sketch 的输入, 转为符合 vgg-format 的输出
     * 
     * @param sketch_border sketch border
     * @param sketch_border_option sketch border option, 可以为 nullptr
     * @param vgg 符合 vgg-format 的输出
     * 
     * @exception sketch_exception
     * 
     * @note 会对 vgg 先进行 clear
    */
    static void change
    (
        const nlohmann::json &sketch_border, 
        const nlohmann::json *sketch_border_option, 
        nlohmann::json &vgg
    );

public:
    /**
     * 将 sketch 的 fill type 转为 vgg-format 的 fill type
     * 
     * @param sketch_fill_type sketch fill type
     * 
     * @return vgg-format fill type
     * 
     * @exception sketch_exception
    */
    static int fill_type_change(int sketch_fill_type);
};

#endif