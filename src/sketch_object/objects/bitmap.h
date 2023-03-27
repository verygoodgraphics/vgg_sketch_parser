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

#ifndef bd_sketch_bitmap
#define bd_sketch_bitmap
#include "src/sketch_object/objects/abstract_layer.h"

class bitmap : public abstract_layer
{
public:    
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg);

public:
    /**
     * 从输入的 json 中, 获取 image 的文件名
     * 
     * @param sketch 输入的 json, 是 file-ref.schema.yaml 或 data-ref.schema.yaml
     * @param out 输出的 image 的文件名
     * 
     * @exception sketch_exception
    */
    static void get_image_file_name(const nlohmann::json &sketch, string &out);
};

#endif