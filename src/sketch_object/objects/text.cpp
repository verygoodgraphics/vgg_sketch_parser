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

// 当前对象解析出的基础属性（脏代码）
// nlohmann::json _text_basic_attr;

void text::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(sketch.at("_class").get<string>() == "text");
    abstract_layer::change(sketch, vgg);
    //_text_basic_attr = vgg;

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

        text::text_style_change(sketch, vgg);

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

    // 对象自身的填充设置到每个文字上了, 没必要再保留该属性的值了(目前这件事没做了)
    // vgg["style"]["fills"] = nlohmann::json::array();
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
                nlohmann::json vgg_font_attr;            
                string_attribute_change(*item, vgg_font_attr);
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

void text::string_attribute_change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(vgg.is_null());
    assert(sketch.at("_class").get<string>() == "stringAttribute");

    try
    {
        vgg["class"] = string("fontAttr");

        auto it_length = sketch.find("length");
        if (it_length != sketch.end())
        {
            vgg["length"] = sketch.at("length").get<int>();
        }
        else 
        {
            vgg["length"] = 1;
            check::ins_.add_error("failed to get text.attr.length");
        }
        //assert(vgg["length"].get<int>());

        /*
        bold: 不进行设置, 会影响字体名称
        italic: 不进行设置, 会影响字体名称
        borderColor: 不进行设置, sketch 中没有针对单个文字进行该属性设置的接口
        borderSize: 不进行设置, sketch 中没有针对单个文字进行该属性设置的接口
        subFamilyName: 不进行设置
        */

        //这部分是 sketch 没有对应的属性, 采用默认值
        //context_settings_change::get_default(vgg["contextSettings"]);
        vgg["kerning"] = false;
        vgg["horizontalScale"] = 1.0;
        vgg["verticalScale"] = 1.0;
        vgg["proportionalSpacing"] = 0.0;
        vgg["rotate"] = 0.0;

        // 备注: attributes 用到的地方太多了, 先强制其存在, 不行的话后续改
        auto &attributes = sketch.at("attributes");
        auto it = attributes.find("kerning");
        if (it != attributes.end())
        {
            vgg["letterSpacing"] = it->get<double>();
        }
        else 
        {
            vgg["letterSpacing"] = 0.0;
        }

        //备注: textStyleVerticalAlignmentKey 不用, 用 style.textStyle.verticalAlignment

        try 
        {
            text::font_descriptor_change(attributes.at("MSAttributedStringFontAttribute"), vgg);
        }
        catch(...)
        {
            //脏代码
            // vgg["name"] = "";
            vgg["size"] = 1.0;
            check::ins_.add_error("failed to get text.font-descriptor");
        }

        it = attributes.find("MSAttributedStringColorAttribute");
        {
            nlohmann::json color;
            if (it != attributes.end())
            {
                color_change::change(*it, color);
            }
            else
            {
                color_change::get_default(color);
            }         

            //脏代码
            nlohmann::json fill;
            fill["class"] = "fill";
            fill["isEnabled"] = true;
            fill["fillType"] = 0;
            fill["color"] = std::move(color);
            context_settings_change::get_default(fill["contextSettings"]);
            vgg["fills"].emplace_back(std::move(fill));
            
            vgg["fillUseType"] = 1;
        }

        // 内外 fill 拟合的版本
        /*
        {
            nlohmann::json color;
            if (it != attributes.end())
            {
                color_change::change(*it, color);
            }
            else
            {
                color_change::get_default(color);
            }

            const auto &global_fills = _text_basic_attr["style"]["fills"];
            bool only_have_one_color_fill = false;
            for (auto &item : global_fills)
            {
                if (!item["isEnabled"].get<bool>())
                {
                    continue;
                }

                if (item["fillType"] != 0)
                {
                    only_have_one_color_fill = false;
                    break;
                }
                
                if (only_have_one_color_fill)
                {
                    only_have_one_color_fill = false;
                    break;
                }

                only_have_one_color_fill = true;
            }                
            if (global_fills.empty())
            {
                //脏代码
                nlohmann::json fill;
                fill["class"] = "fill";
                fill["isEnabled"] = true;
                fill["fillType"] = 0;
                fill["color"] = std::move(color);
                context_settings_change::get_default(fill["contextSettings"]);

                // text 对象本身没有填充, 则使用文字对象的颜色
                vgg["fills"].emplace_back(std::move(fill));
            }
            else if (only_have_one_color_fill) 
            {
                // 仅当对象本身的填充为纯色, 且对象本身的有效填充数目为1, 此时需要将对象本身填充的透明度和文字颜色的透明度叠加
                for (auto &item : global_fills)
                {
                    vgg["fills"].emplace_back(item);
                    auto &tmp = vgg["fills"].back()["color"]["alpha"];
                    tmp = tmp.get<double>() * color["alpha"].get<double>();
                }
            }
            else 
            {
                // 当对象本身存在多个填充时, 忽略文字自身的颜色
                for (auto &item : global_fills)
                {
                    vgg["fills"].emplace_back(item);
                }
            }
        }
        */

        nlohmann::json null_json;
        it = attributes.find("paragraphStyle");
        paragraph_style_change(it != attributes.end() ? *it : null_json, vgg);

        //不在 sketch-schema 中描述的一些属性, 来自测试总结
        {
            it = attributes.find("underlineStyle");
            if (it != attributes.end())
            {
                int underline = it->get<int>();
                check::ins_.check_range(underline, 0, 1, 0, "invalid underline");
                vgg["underline"] = underline;
            }
            else 
            {
                vgg["underline"] = 0;
            }

            it = attributes.find("strikethroughStyle");
            if (it != attributes.end())
            {
                int linethrough = it->get<int>();
                check::ins_.check_range(linethrough, 0, 1, 0, "invalid linethrough");
                vgg["linethrough"] = static_cast<bool>(linethrough);
            }
            else 
            {
                vgg["linethrough"] = false;
            }

            it = attributes.find("MSAttributedStringTextTransformAttribute");
            if (it != attributes.end())
            {
                int value = it->get<int>();
                if (!value)
                {
                    vgg["lettertransform"] = 0;
                }
                else if (1 == value)
                {
                    vgg["lettertransform"] = 2;
                }
                else if (2 == value)
                {
                    vgg["lettertransform"] = 3;
                }
                else 
                {
                    //throw sketch_exception("invalid letter transform");
                    check::ins_.add_error("invalid letter transform");
                    vgg["lettertransform"] = 0;
                }
            }
            else 
            {
                vgg["lettertransform"] = 0;
            }
    
            it = attributes.find("baselineOffset");
            if (it != attributes.end())
            {
                vgg["baselineShift"] = it->get<double>();
            }
            else 
            {
                vgg["baselineShift"] = 0.0;
            }

            //sketch 中的上标、下标可以分多个层次, 与 harry 讨论后, 暂定先不兼容
            vgg["baseline"] = 0;
            it = attributes.find("NSSuperScript");
            if (it != attributes.end())
            {
                int value = it->get<int>();
                if (value > 0)
                {
                    vgg["baseline"] = 1;
                }
                else if (value < 0)
                {
                    vgg["baseline"] = 2;
                }
            }
        }
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze font attr");
    }
}

void text::font_descriptor_change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(vgg.at("class").get<string>() == "fontAttr");
    assert(sketch.at("_class").get<string>() == "fontDescriptor");

    try
    {
        auto &attr = sketch.at("attributes");
        auto it = attr.find("name");
        if (it != attr.end())
        {
            vgg["name"] = it->get<string>();
        }

        vgg["size"] = attr.at("size").get<double>();
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze font descriptor");
    }

    /*
    备注: 未处理的项包括
    attributes 中的 variation
    */
}

void text::paragraph_style_change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(vgg.at("class").get<string>() == "fontAttr");

    auto &text_paragraph = vgg["textParagraph"];

    text_paragraph["class"] = string("textParagraph");
    text_paragraph["paragraphSpacing"] = 0.0;
    vgg["horizontalAlignment"] = 0;
    vgg["lineSpace"] = 24.0;    //该值为默认值

    if (sketch.is_null())
    {
        return;
    }

    assert(sketch.at("_class").get<string>() == "paragraphStyle");

    try
    {
        auto it = sketch.find("paragraphSpacing");
        if (it != sketch.end())
        {
            text_paragraph["paragraphSpacing"] = it->get<double>();
        }

        it = sketch.find("alignment");
        if (it != sketch.end())
        {
            int horizontal_alignment = it->get<int>();
            check::ins_.check_range(horizontal_alignment, 0, 4, 0, "invalid horizontal alignment");
            vgg["horizontalAlignment"] = horizontal_alignment;
        }

        // 经测试, sketch 中 调整行距会使得 maximumLineHeight minimumLineHeight 为同一值
        it = sketch.find("maximumLineHeight");
        if (it != sketch.end())
        {
            assert(abs(sketch.at("maximumLineHeight").get<double>() - sketch.at("minimumLineHeight").get<double>()) < 0.00001);
            vgg["lineSpace"] = it->get<double>();
        }
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze paragraph style");
    }
    
    /*
    备注: 未处理的项包括
    allowsDefaultTighteningForTruncation: sketch 界面上没有能操作这个项的地方
    */ 
}

void text::text_style_change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(vgg.at("class").get<string>() == "text");
    assert(sketch.at("_class").get<string>() == "text");
    
    vgg["verticalAlignment"] = 0;

    try 
    {
        int vertical_alignment = sketch.at("style").at("textStyle").at("verticalAlignment").get<int>();
        check::ins_.check_range(vertical_alignment, 0, 2, 0, "invalid vertical alignment");
        vgg["verticalAlignment"] = vertical_alignment;
    }
    catch(...)
    {
    }

    //备注: 其他属性不处理
}
