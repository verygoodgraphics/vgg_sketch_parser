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

#ifndef bd_sketch_gradient_change
#define bd_sketch_gradient_change
#include "src/basic/define.h"
#include "nlohmann/json.hpp"

class gradient_change
{
public:
    /**
     * 将 sketch 的输入, 转为符合 vgg-format 的输出
     * 
     * @param sketch sketch 的输入
     * @param vgg 符合 vgg-format 的输出
     * @param bound_width bound 的 宽度
     * @param bound_height bound 的 高度
     * 
     * @exception sketch_exception
     * 
     * @note 会对 vgg 先进行 clear
    */
    static void change(const nlohmann::json &sketch, nlohmann::json &vgg,
        double bound_width, double bound_height);

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
    static void gradient_stops_change(const nlohmann::json &sketch, nlohmann::json &vgg);

    // 创建默认的 stops
    static void create_default_stops(nlohmann::json &out);
};

#endif