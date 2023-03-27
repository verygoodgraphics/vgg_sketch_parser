#ifndef bd_sketch_context_settings_change
#define bd_sketch_context_settings_change
#include "src/basic/define.h"
#include "nlohmann/json.hpp"

class context_settings_change
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

    /**
     * 获取默认的 context setting
     * 
     * @param out 保存默认的 context setting
    */
    static void get_default(nlohmann::json &out);
};

#endif