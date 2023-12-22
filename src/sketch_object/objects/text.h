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

#ifndef bd_sketch_text
#define bd_sketch_text
#include "src/sketch_object/objects/abstract_layer.h"

class text : public abstract_layer
{
public:    
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg);

public:
    // 构建 font-attr
    static nlohmann::json create_font_attr(const nlohmann::json &sketch);

    // 提取 verticalAlignment
    static int chagne_vertical_alignment(const nlohmann::json &sketch);

    // 提取 alignment 信息, 返回 horizontalAlignment
    static int change_text_horizontal_alignment(const nlohmann::json &sketch);

private:
    // 提取 color 信息, 返回 font-attr.fills
    static nlohmann::json change_text_color(const nlohmann::json &sketch);

    // 提取 font-attr.name 和 font-attr.size
    static nlohmann::json change_text_name_size(const nlohmann::json &sketch);

    // 提取 font-attr.letterSpacing
    static double change_text_letter_spacing(const nlohmann::json &sketch);

    // 提取 font-attr.lineSpace
    static double change_text_line_spacing(const nlohmann::json &sketch, double font_size);

    // 提取 font-attr.underline
    static int change_text_underline(const nlohmann::json &sketch);

    // 提取 font-attr.linethrough
    static bool change_text_line_through(const nlohmann::json &sketch);

    // 提取 font-attr.lettertransform
    static int change_text_letter_transform(const nlohmann::json &sketch);

    // 提取 font-attr.baselineShift
    static double change_text_baseline_shift(const nlohmann::json &sketch);

    // 提取 font-attr.baseline
    static int change_text_baseline(const nlohmann::json &sketch);

    // 提取 font-attr.textParagraph
    static nlohmann::json change_text_paragraph(const nlohmann::json &sketch);

    // 获取 font-attr 中, 固定的属性值
    static nlohmann::json get_fixed_attr();

private:
    /**
     * 将 sketch 的输入, 转为符合 vgg-format 的输出
     * 
     * @param sketch sketch 的输入 (attributed-string.schema.yaml)
     * @param vgg 符合 vgg-format 的输出
     * 
     * @exception sketch_exception
     * 
     * @note 不会对 vgg 进行 clear
    */
    static void attributed_string_change(const nlohmann::json &sketch, nlohmann::json &vgg);
};

#endif
