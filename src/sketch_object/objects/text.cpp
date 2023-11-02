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

#include "./text.h"
#include "src/sketch_object/attrs/color_change.h"
#include "src/sketch_object/attrs/context_settings_change.h"
#include "src/sketch_object/check.hpp"
#include "src/sketch_object/attrs/fill_change.h"

void text::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(sketch.at("_class").get<string>() == "text");
    abstract_layer::change(sketch, vgg);
    vgg["keepShapeWhenResize"] = true;

    try
    {
        vgg["class"] = string("text");

        //备注: 当前要求 attributedString 必须存在
        text::attributed_string_change(sketch.at("attributedString"), vgg);
        
        if (get_json_value(sketch, "automaticallyDrawOnUnderlyingPath", false))
        {
            vgg["textOnPath"]["class"] = string("textOnPath");
        }

        int text_behavious = get_json_value(sketch, "textBehaviour", 0);
        switch (text_behavious)
        {
            case 0:
            {
                text_behavious = 1;
                break;
            }
            case 1:
            {
                text_behavious = 2;
                break;
            }
            case 2:
            {
                text_behavious = 0;
                break;
            }
            default:
            {
                // throw sketch_exception("invalid text behaviour");
                text_behavious = 0;
                check::ins_.add_error("invalid text behaviour");
            }
        }
        vgg["frameMode"] = text_behavious;

        try 
        {
            vgg["verticalAlignment"] = text::chagne_vertical_alignment(sketch.at("style").at("textStyle"));
        }
        catch(...)
        {
            vgg["verticalAlignment"] = 0;
        }

        /*
        备注: 未处理的项包括
        dontSynchroniseWithSymbol
        lineSpacingBehaviour
        glyphBounds: 是文本紧凑矩形的两个对角线的点(以 frame 左上角为(0, 0))
        */
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze text");
    }
}

void text::attributed_string_change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(sketch.at("_class").get<string>() == string("attributedString"));
    assert(vgg.at("class") == "text");

    try
    {
        auto it = sketch.find("string");
        if (it != sketch.end())
        {
            vgg["content"] = sketch.at("string").get<string>();
        }
        else 
        {
            vgg["content"] = string("");
            check::ins_.add_error("failed to get text.context");
        }
        
        //确保有序
        vgg["attr"] = nlohmann::json::array();
        vector<const nlohmann::json*> order_attributes;
        it = sketch.find("attributes");
        if (it != sketch.end())
        {
            for (auto &item : *it)
            {
                order_attributes.emplace_back(&item);
            }

            try 
            {
                std::sort(order_attributes.begin(), order_attributes.end(), 
                    [](const nlohmann::json *item_0, const nlohmann::json *item_1)
                {
                    return item_0->at("location").get<int>() < item_1->at("location").get<int>();
                });                    
            }
            catch(...)
            {
                check::ins_.add_error("failed to sort text.attr");
            }

            for (auto &item : order_attributes)
            {
                //备注: textStyleVerticalAlignmentKey 不用, 用 style.textStyle.verticalAlignment
                nlohmann::json vgg_font_attr = text::create_font_attr(item->at("attributes"));

                try 
                {
                    vgg_font_attr["length"] = item->at("length").get<int>();
                }
                catch(...)
                {
                    check::ins_.add_error("failed to get text.attr.length");
                }

                vgg["attr"].emplace_back(std::move(vgg_font_attr));
            }
        }
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze attributedString");
    }
}

nlohmann::json text::create_font_attr(const nlohmann::json &sketch)
{
    nlohmann::json out;

    /*
    bold: 不进行设置, 会影响字体名称
    italic: 不进行设置, 会影响字体名称
    borderColor: 不进行设置, sketch 中没有针对单个文字进行该属性设置的接口
    borderSize: 不进行设置, sketch 中没有针对单个文字进行该属性设置的接口
    subFamilyName: 不进行设置
    */

    out["class"] = string("fontAttr");
    out["fills"] = text::change_text_color(sketch);
    out["horizontalAlignment"] = text::change_text_horizontal_alignment(sketch);
    out.update(text::change_text_name_size(sketch));
    out["letterSpacing"] = text::change_text_letter_spacing(sketch);
    out["lineSpace"] = text::change_text_line_spacing(sketch, out["size"].get<double>());
    out["textParagraph"] = text::change_text_paragraph(sketch);
    out.update(text::get_fixed_attr());

    //不在 sketch-schema 中描述的一些属性, 来自测试总结
    out["underline"] = text::change_text_underline(sketch);
    out["linethrough"] = text::change_text_line_through(sketch);
    out["lettertransform"] = text::change_text_letter_transform(sketch);
    out["baselineShift"] = text::change_text_baseline_shift(sketch);
    out["baseline"] = text::change_text_baseline(sketch);   

    return out;
}

nlohmann::json text::change_text_color(const nlohmann::json &sketch)
{
    nlohmann::json color;

    auto it = sketch.find("MSAttributedStringColorAttribute");
    if (it != sketch.end())
    {
        color_change::change(*it, color);
    }
    else
    {
        color_change::get_default(color);
    }

    nlohmann::json out;
    out.emplace_back(fill_change::construct_from_color(std::move(color)));
    return out;
}

int text::change_text_horizontal_alignment(const nlohmann::json &sketch)
{
    int result = 0;

    try 
    {
        result = sketch.at("paragraphStyle").at("alignment").get<int>();
        check::ins_.check_range(result, 0, 4, 0, "invalid horizontal alignment");
    }
    catch(...)
    {
        // 备注: 此处无需报警
    }

    return result;
}

nlohmann::json text::change_text_name_size(const nlohmann::json &sketch)
{
    nlohmann::json out;

    try 
    {
        const auto &obj = sketch.at("MSAttributedStringFontAttribute").at("attributes");
        out["name"] = obj.at("name").get<string>();
        out["size"] = obj.at("size").get<double>();
    }
    catch(...)
    {
        out["size"] = 12;
        check::ins_.add_error("failed to analyze text name and size");
    }

    return out;
}

double text::change_text_letter_spacing(const nlohmann::json &sketch)
{
    try 
    {
        return sketch.at("kerning").get<double>();
    }
    catch(...)
    {
        // 无需报警
    }

    return 0.0;
}

double text::change_text_line_spacing(const nlohmann::json &sketch, double font_size)
{
    // 1.25 这个值源于测试
    // 对于字号 14 的结果是 18, 对于字号 6 的结果是 7, 说明 sketch 在 XX.5 上策略不一致, 此处将忽略这种细微的情况
    double result = round(font_size * 1.25);

    // 经测试, sketch 中 调整行距会使得 maximumLineHeight minimumLineHeight 为同一值
    try 
    {
        result = sketch.at("paragraphStyle").at("maximumLineHeight").get<double>();
    }
    catch(...)
    {
        // 无需报警
    }

    return result;
}

int text::change_text_underline(const nlohmann::json &sketch)
{
    int result = 0;

    try 
    {
        result = sketch.at("underlineStyle").get<int>();
        check::ins_.check_range(result, 0, 1, 0, "invalid underline");
    }
    catch(...)
    {
        // 无需报警
    }

    return result;
}

bool text::change_text_line_through(const nlohmann::json &sketch)
{
    bool result = false;

    try 
    {
        int line_through = sketch.at("strikethroughStyle").get<int>();
        check::ins_.check_range(line_through, 0, 1, 0, "invalid linethrough");
        result = static_cast<bool>(line_through);
    }
    catch(...)
    {
        // 无需报警
    }

    return result;
}

int text::change_text_letter_transform(const nlohmann::json &sketch)
{
    int result = 0;

    try 
    {
        result = sketch.at("MSAttributedStringTextTransformAttribute").get<int>();

        if (result == 1 || result == 2)
        {
            ++result;
        }
        else if (result != 0)
        {
            check::ins_.add_error("invalid letter transform");
            result = 0;
        }
    }
    catch(...)
    {
        // 无需报警
    }

    return result;
}

double text::change_text_baseline_shift(const nlohmann::json &sketch)
{
    double result = 0;

    try 
    {
        result = sketch.at("baselineOffset").get<double>();
    }
    catch(...)
    {
        // 无需报警
    }

    return result;
}

int text::change_text_baseline(const nlohmann::json &sketch)
{
    //sketch 中的上标、下标可以分多个层次, 与 harry 讨论后, 暂定先不兼容
    int result = 0;

    try 
    {
        result = sketch.at("NSSuperScript").get<int>();

        if (result > 0)
        {
            result = 1;
        }
        else if (result < 0)
        {
            result = 2;
        }
    }
    catch(...)
    {
        // 无需报警
    }

    return result;
}

nlohmann::json text::change_text_paragraph(const nlohmann::json &sketch)
{
    double result = 0;

    try 
    {
        result = sketch.at("paragraphStyle").at("paragraphSpacing").get<double>();
    }
    catch(...)
    {
        // 无需报警
    }

    nlohmann::json out;
    out["class"] = string("textParagraph");
    out["paragraphSpacing"] = result;

    return out;
}

int text::chagne_vertical_alignment(const nlohmann::json &sketch)
{
    int result = 0;

    try 
    {
        result = sketch.at("verticalAlignment").get<int>();
        check::ins_.check_range(result, 0, 2, 0, "invalid vertical alignment");
    }
    catch(...)
    {
        // 无需报警
    }

    return result;
}

nlohmann::json text::get_fixed_attr()
{
    nlohmann::json out;
    out["fillUseType"] = 1;
    out["kerning"] = false;
    out["horizontalScale"] = 1.0;
    out["verticalScale"] = 1.0;
    out["proportionalSpacing"] = 0.0;
    out["rotate"] = 0.0;  
    
    return out;
}
