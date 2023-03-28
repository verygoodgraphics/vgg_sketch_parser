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

#ifndef bd_sketch_get_json_value
#define bd_sketch_get_json_value

#include "./define.h"
#include "nlohmann/json.hpp"

/**
 * 获取输入的 json 中的给定 key 的值, 若 key 获取异常, 则返回 default_value
 * 
 * @param json_in 输入的 json
 * @param key 访问的 key 名称
 * @param default_value 默认值
*/
template <typename T>
const T get_json_value(const nlohmann::json &json_in, const char *key, const T &default_value)
{
    try 
    {
        return json_in.at(key).get<T>();
    }
    catch(...)
    {
        return default_value;
    }
}

#endif