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

#ifndef bd_sketch_mask
#define bd_sketch_mask
#include "src/basic/define.h"
#include "src/sketch_object/objects/abstract_layer.h"

/*
sketch 中的蒙版:
1.蒙版对象的作用效果不会溢出到 group 外
2.设对象 X 自身是一个蒙版, 则 X 与同 group 中对象的关系:
    A.对象 X 下方的蒙版对象, 不会对 X 的显示产生影响
        a.若 X 是轮廓蒙版, 则按照 style 正常显示
        b.若 X 是透明度蒙版, 则其不会进行 fill (border等是生效的)
    B.无论 X 是否打破其下方蒙版对象的 mask-chain, X 上方的对象的蒙版对象就是 X
    C.即该 group 中每个对象的生效蒙版对象只有一个, 而不是像 ai 那样对蒙版求交集
3.若当前 group 外存在一个轮廓蒙版 X , group 中存在一个轮廓蒙版 Y , 对于 group 中位于 Y 之上的对象, 其最终显示区域是对 X Y 求交集
4.shape-group 被认为是一个对象, 就像 ai 中的 compound-path 那样
5.在 page 中, artboard 会中断其堆栈下方的蒙版效果, 并且该蒙版效果不会影响 artboard
*/
class mask
{
public:
    /**
     * 提取蒙版并进行赋值
     * 
     * @param obj 项
     * @param obj_json 项的 json
    */
    void deal_mask
    (
        const abstract_layer &obj, 
        nlohmann::json &obj_json
    );

    //重置蒙版效果
    void reset_mask()
    {
        mask_obj_id_.clear();
        mask_obj_type_ = 0;
    }

private:
    //蒙版对象的id, 非空即有效
    string mask_obj_id_;

    /*
    蒙版对象的蒙版类型:
    1: 当前对象是轮廓蒙版类型
    2: 当前对象是透明度蒙版类型
    */
    int mask_obj_type_ = 0;

public:
    /**
     * 将父对象中的 mask 添加到子项中
     * 
     * @param parent 符合 vgg-format 的对象
    */
    static void add_parent_mask_to_child(nlohmann::json &parent);
};

#endif