#ifndef bd_sketch_blur_change
#define bd_sketch_blur_change
#include "src/basic/define.h"
#include "nlohmann/json.hpp"

class blur_change
{
public:
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
    static void change(const nlohmann::json &sketch, nlohmann::json &vgg);
};

#endif