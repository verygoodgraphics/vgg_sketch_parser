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

#ifndef bd_sketch_abstract_group
#define bd_sketch_abstract_group
#include "src/sketch_object/objects/abstract_layer.h"

/*
备注: 在 sketch 中, abstract_group 有两个子类型, group 和 shape-group(即复合对象),
但是根据 vgg-format 的语义, 复合对象不是 group, 所以这里将 abstract_group 和 group 进行等价
*/
class abstract_group : public abstract_layer
{
public:
    abstract_group();    

public:
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg);

private:
    static t_child child_;
};

typedef abstract_group group;

#endif