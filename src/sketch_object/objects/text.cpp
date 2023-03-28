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

void text::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(sketch.at("_class").get<string>() == "text");
    abstract_layer::change(sketch, vgg);

    try
    {
        vgg["class"] = string("text");
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
                throw sketch_exception("invalid text behaviour");
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
}

void text::attributed_string_change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(sketch.at("_class").get<string>() == string("attributedString"));
    assert(vgg.at("class") == "text");

    try
    {
        vgg["content"] = sketch.at("string").get<string>();
        
        //确保有序
        vector<const nlohmann::json*> order_attributes;
        for (auto &item : sketch.at("attributes"))
        {
            order_attributes.emplace_back(&item);
        }
        std::sort(order_attributes.begin(), order_attributes.end(), 
            [](const nlohmann::json *item_0, const nlohmann::json *item_1)
        {
            return item_0->at("location").get<int>() < item_1->at("location").get<int>();
        });

        for (auto &item : order_attributes)
        {
            nlohmann::json vgg_font_attr;            
            string_attribute_change(*item, vgg_font_attr);
            vgg["attr"].emplace_back(std::move(vgg_font_attr));
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
        vgg["length"] = sketch.at("length").get<int>();
        assert(vgg["length"].get<int>());

        /*
        bold: 不进行设置, 会影响字体名称
        italic: 不进行设置, 会影响字体名称
        borderColor: 不进行设置, sketch 中没有针对单个文字进行该属性设置的接口
        borderSize: 不进行设置, sketch 中没有针对单个文字进行该属性设置的接口
        subFamilyName: 不进行设置
        */

        //这部分是 sketch 没有对应的属性, 采用默认值
        context_settings_change::get_default(vgg["contextSettings"]);
        vgg["kerning"] = false;
        vgg["horizontalScale"] = 1.0;
        vgg["verticalScale"] = 1.0;
        vgg["proportionalSpacing"] = 1.0;
        vgg["rotate"] = 0.0;

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

        text::font_descriptor_change(attributes.at("MSAttributedStringFontAttribute"), vgg);

        it = attributes.find("MSAttributedStringColorAttribute");
        if (it != attributes.end())
        {
            color_change::change(*it, vgg["fillColor"]);
        }
        else
        {
            color_change::get_default(vgg["fillColor"]);
        }

        nlohmann::json null_json;
        it = attributes.find("paragraphStyle");
        paragraph_style_change(it != attributes.end() ? *it : null_json, vgg);

        //不在 sketch-schema 中描述的一些属性, 来自测试总结
        {
            it = attributes.find("underlineStyle");
            if (it != attributes.end())
            {
                vgg["underline"] = it->get<int>();
                range_check(vgg["underline"].get<int>(), 0, 1, "invalid underline");
            }
            else 
            {
                vgg["underline"] = 0;
            }

            it = attributes.find("strikethroughStyle");
            if (it != attributes.end())
            {
                vgg["linethrough"] = static_cast<bool>(it->get<int>());
                range_check(it->get<int>(), 0, 1, "invalid linethrough");
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
                    throw sketch_exception("invalid letter transform");
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
            vgg["horizontalAlignment"] = it->get<int>();
            range_check(vgg["horizontalAlignment"].get<int>(), 0, 4, "invalid horizontal alignment");
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
        vgg["verticalAlignment"] = sketch.at("style").at("textStyle").at("verticalAlignment");
        range_check(vgg["verticalAlignment"].get<int>(), 0, 2, "invalid vertical alignment");
    }
    catch(...)
    {
    }

    //备注: 其他属性不处理
}
