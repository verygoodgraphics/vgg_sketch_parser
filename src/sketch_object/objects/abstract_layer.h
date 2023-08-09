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

#ifndef bd_sketch_abstract_object
#define bd_sketch_abstract_object
#include "src/basic/define.h"
#include "nlohmann/json.hpp"
#include "src/basic/get_json_value.hpp"

class abstract_layer
{
public:
    virtual ~abstract_layer() {}

public:
    //当前对象的id
    string id_;

    //是否打破 mask chain
    bool break_mask_chain_ = false;

    /*
    蒙版类型:
    0: 当前对象不是蒙版
    1: 当前对象是轮廓蒙版类型
    2: 当前对象是透明度蒙版类型
    */
    int mask_type_ = 0;

    /*
    对象的bool操作, 取值意义参考 vgg-schema
    备注: sketch-schema 1.0 的取值不对, 经测试其取值与 sketch-schema 3.0 保持一致
    */
    int boolean_operation_ = -1;

public:
    /**
     * 将 sketch 的输入, 转为符合 vgg-format 的输出
     * 
     * @param sketch sketch 的输入
     * @param vgg 符合 vgg-format 的输出
     * 
     * @exception sketch_exception
     * 
     * @note 不会对 json_vgg 进行 clear
    */
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg);
};

//用于构建子项
typedef map<string, unique_ptr<abstract_layer>> t_child;

// 初始化子项
void init_child(t_child &child);

#endif