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

#ifndef bd_sketch_check
#define bd_sketch_check
#include <vector>
#include <string>
#include "boost/noncopyable.hpp"

/**
 * 校验输入值是否属于指定的范围, 若不属于则触发异常
 * 
 * @param value 被校验的值
 * @param min_value 合法范围的下限(包含)
 * @param max_value 合法范围的上限(包含)
 * @param error_hint 错误信息, 用于生成异常
 * 
 * @exception sketch_exception
*/
// template<typename T>
// void range_check(T value, T min_value, T max_value, const char* error_hint)
// {
//     if (value < min_value || value > max_value)
//     {
//         //throw sketch_exception(error_hint);
//     }
// }

class check : public boost::noncopyable
{
private:
    check() {}

public:
    /**
     * 校验输入值是否属于指定的范围, 若不属于则触发异常
     * 
     * @param value 被校验的值
     * @param min_value 合法范围的下限(包含)
     * @param max_value 合法范围的上限(包含)
     * @param default_value 若被校验的值不在合法范围内, 则被校验的值将被设置为该值
     * @param error_hint 当被校验的值不在合法范围内时, 将被添加的错误信息
    */
    template<typename T>
    void check_range(T &value, T min_value, T max_value, T default_value, const char* error_hint)
    {
        assert(min_value <= max_value);
        if (value < min_value || value > max_value)
        {
            add_error(error_hint);
            value = default_value;
        }
    }

    void add_error(const string &str)
    {
        // 备注: 错误信息不用收集太多, 浪费内存
        if (error_.size() <= 5)
        {
            this->error_.emplace_back(str);
        }
    }

    void reset()
    {
        this->error_.clear();
    }

    const std::vector<std::string> &get_error() const
    {
        return this->error_;
    }

private:
    std::vector<std::string> error_;

public:
    static check ins_;
};

#endif