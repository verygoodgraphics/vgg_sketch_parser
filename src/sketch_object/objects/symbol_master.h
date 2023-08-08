/*
MIT License

Copyright (c) 2023 Very Good Graphics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef bd_sketch_symbol_master
#define bd_sketch_symbol_master
#include "src/sketch_object/objects/abstract_root.h"

struct symbol_master_attr
{
    // 是否将背景色包含在子项中
    bool include_background_color_in_instance_ = false;

    // 是否支持属性覆盖
    bool allow_override_ = false;

    // symbol master 解析后的 json
    nlohmann::json master_data_;
};

class symbol_master : public abstract_root
{
public:
    symbol_master();

    typedef std::unordered_map<std::string /*symbol id*/, unique_ptr<symbol_master_attr>> t_master_info;

public:    
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg);

public:
    static const t_master_info& get_master() { return symbol_master::masters_; }
    static void clear_master();
    static const std::unordered_map<string, bool>& get_override_switch() { return symbol_master::override_switch_; }

private:
    static t_child child_;

    // symbol master 集合
    static t_master_info masters_;

    // symbol master 中禁止属性覆盖的项集合
    // 备注: 不在该项中的 overrideName, 其默认值为 true
    // 备注: 这里取巧了, 因为 symbol master 里的 symbol instance 可以被覆盖为其他 instance, 所以这里做了一个集合
    // <overrideName, canOverride>
    static std::unordered_map<string, bool> override_switch_;
};

#endif