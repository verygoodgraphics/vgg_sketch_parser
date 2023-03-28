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

#ifndef bd_sketch_extract
#define bd_sketch_extract
#include <vector>
#include <map>
#include "./src/basic/define.h"
#include "zip.h"

using std::vector;
using std::map;

//处理 sketch 文件的解压
class extract
{
public:
    typedef map<string/*文件名*/, vector<char>/*文件内容*/> t_extract_result;

public:
    /**
     * 设置待解压的 sketch 文件
     * 
     * @param content 待解压的文件内容
     * @param len 待解压的文件长度(byte)
     * @param result 解压结果
     * 
     * @exception sketch_exception
    */
    static void set_file(const char* content, const size_t len, t_extract_result &result);

public:
    static const string _document_file_name;
    static const string _meta_file_name;
    static const string _user_file_name;
    static const string _pages_dir_name;
    static const string _images_dir_name;
};

#endif