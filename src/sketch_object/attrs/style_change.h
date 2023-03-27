#ifndef bd_sketch_style_change
#define bd_sketch_style_change
#include "src/basic/define.h"
#include "nlohmann/json.hpp"

class style_change
{
public:
    /**
     * 将 sketch 的输入, 转为符合 vgg-format 的输出
     * 
     * @param sketch sketch 的输入
     * @param vgg 符合 vgg-format 的输出
     * @param context_setting 从 sketch 中解析出的 context setting
     * 
     * @exception sketch_exception
     * 
     * @note 会对 vgg 先进行 clear
    */
    static void change
    (
        const nlohmann::json &sketch, 
        nlohmann::json &vgg, 
        nlohmann::json &context_setting
    );

    /**
     * 获取默认的 style
     * 
     * @param out 保存默认的 style
    */
    static void get_default(nlohmann::json &out);
};

#endif