#ifndef bd_sketch_symbol_master
#define bd_sketch_symbol_master
#include "src/sketch_object/objects/abstract_root.h"

class symbol_master : public abstract_root
{
public:
    symbol_master();

public:    
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg);

public:
    /**
     * 校验输入的字符串是否符合相关 vgg-format
     * 
     * @param str 被校验的字符串
     * 
     * @exception sketch_exception
     * 
     * @note 仅在校验失败时, 抛出 sketch_exception 异常
    */
    static void override_name_check(const string &str);

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
    static void override_properties_change(const nlohmann::json &sketch, nlohmann::json &vgg);

private:
    static t_child child_;
};

#endif