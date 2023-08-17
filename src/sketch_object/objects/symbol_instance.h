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

#ifndef bd_sketch_symbol_instance
#define bd_sketch_symbol_instance
#include "src/sketch_object/objects/abstract_layer.h"
#include <functional>

class symbol_instance : public abstract_layer
{
public:
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg);

public:
    // 处理 symbol-instance 的属性覆盖
    // vgg_format: 符合 vgg-format 的对象
    static void deal_override_attr(nlohmann::json &vgg_format);

private:
    // 对 obj 中的所有 symbol_instance, 调用 fun 函数
    static void recursive_deal(nlohmann::json &obj, std::function<void(nlohmann::json &obj)> fun);

    // 处理属性覆盖
    static void override_attr(nlohmann::json &obj);

    // 对象收集
    static void collection_objs(const nlohmann::json &obj);

    static nlohmann::json create_override_value(const vector<string> &obj_id, 
        const string &override_name, nlohmann::json &&override_value);

private:
    // vgg-format 中所有的对象, 用于后续属性覆盖(由于存在 symbol-instance 覆盖, 这样做虽然慢了点, 但是简单很多)
    // 组织方式: <id, json> 
    static std::unordered_map<string, const nlohmann::json*> objs_;
};

#endif