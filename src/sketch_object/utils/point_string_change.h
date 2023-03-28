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

#ifndef bd_sketch_point_string_change
#define bd_sketch_point_string_change
#include "src/basic/define.h"
#include "nlohmann/json.hpp"

class point_string_change
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
     * 提取 sketch 的点, 存于 out 中
     * 
     * @param sketch sketch 的输入
     * @param out 提取结果, 数组第一个元素是 x, 第二个元素是 y
     * 
     * @exception sketch_exception
    */
    static void change(const nlohmann::json &sketch, double *out);

    /**
     * 根据比例、起始坐标、长度, 获取真实坐标
     * 
     * @param ratio 比例
     * @param begin 起始坐标
     * @param length 长度或者宽度
     * 
     * @return 真实坐标, 该坐标是 sketch 坐标系下的坐标
    */
    static double calc_real_coordinate(const double ratio, const double begin, const double length);
};

#endif