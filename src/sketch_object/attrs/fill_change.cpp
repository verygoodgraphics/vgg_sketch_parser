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

#include "./fill_change.h"
#include "src/sketch_object/check.hpp"
#include "src/sketch_object/attrs/color_change.h"
#include "src/sketch_object/attrs/border_change.h"
#include "src/sketch_object/attrs/context_settings_change.h"
#include "src/sketch_object/attrs/gradient_change.h"
#include "src/sketch_object/objects/bitmap.h"
#include "src/basic/get_json_value.hpp"
#include <fstream>

void fill_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    try 
    {
        assert(sketch.at("_class").get<string>() == "fill");

        vgg["class"] = "fill";
        vgg["isEnabled"] = get_json_value(sketch, "isEnabled", false);

        int fill_type = border_change::fill_type_change(get_json_value(sketch, "fillType", 0));

        auto it_color = sketch.find("color");
        if (it_color != sketch.end())
        {
            color_change::change(sketch.at("color"), vgg["color"]);
        }

        //备注: 在 sketch-schema 1.0 中, contextSettings 和 gradient 是可选的
        {
            auto it = sketch.find("contextSettings");
            if (it != sketch.end())
            {
                context_settings_change::change(sketch.at("contextSettings"), vgg["contextSettings"]);
            }
            else
            {
                context_settings_change::get_default(vgg["contextSettings"]);
            }

            it = sketch.find("gradient");
            if (it != sketch.end())
            {
                gradient_change::change(sketch.at("gradient"), vgg["gradient"]);
            }
            else if (1 == fill_type) 
            {
                // 备注: 存在一个用例, 其 isEnabled 为 false, 其 fillType 为渐变, 但 gradient 不存在, 此时将其修改为纯色
                // 脏代码: 更好的策略是构造一个默认的 gradient
                // check::ins_.add_error("failed to create fill.gradient");
                fill_type = 0;
            }
        }

        //pattern
        if (fill_type == 2)
        {
            nlohmann::json pattern;
            pattern["class"] = "pattern";
            
            auto &instance = pattern["instance"];
            instance["class"] = string("patternImage");

            int pattern_fill_type = get_json_value(sketch, "patternFillType", 0);
            check::ins_.check_range(pattern_fill_type, 0, 3, 0, "invalid pattern fill type");
            instance["fillType"] = pattern_fill_type;

            instance["imageTileMirrored"] = false;
            //instance["imageTileScale"] = get_json_value(sketch, "patternTileScale", 1.0);

            double scale = get_json_value(sketch, "patternTileScale", 1.0);
            instance["matrix"] = nlohmann::json::array();
            if (0 != pattern_fill_type)
            {
                scale = 1;
            }
            instance["matrix"].emplace_back(scale);
            instance["matrix"].emplace_back(0.0);
            instance["matrix"].emplace_back(0.0);
            instance["matrix"].emplace_back(scale);
            instance["matrix"].emplace_back(0.0);
            instance["matrix"].emplace_back(0.0);            

            auto it_image = sketch.find("image");
            if (it_image == sketch.end())
            {
                //szn_todo
                //备注: image 在 sketch 中不是必选项, 但目前的处理方式下, 其是必须的
                //throw sketch_exception("although image is not required in sketch, it is currently required in vgg-sketch-parser");

                // 备注: 解析不了, 则先用纯色, 比抛异常好
                // 备注: 目前遇到过 pattern 使用 noiseIndex 的, 这些是目前解析不了的 szn_todo
                fill_type = 0;
                //check::ins_.add_error("failed to create fill.pattern");
            }
            else 
            {
                string file_name;
                
                try
                {
                    bitmap::get_image_file_name(*it_image, file_name);
                }
                catch (sketch_exception &e) 
                {
                    check::ins_.add_error(e.get());
                    file_name = "image not found";
                }
                catch (...)
                {
                    check::ins_.add_error("fail to analyze bitmap");
                    file_name = "image not found"; 
                }

                instance["imageFileName"] = file_name;
                vgg["pattern"] = std::move(pattern);
            }
        }

        if (!fill_type && vgg.find("color") == vgg.end())
        {
            color_change::get_default(vgg["color"]);
        }
        vgg["fillType"] = fill_type;

        /*
        备注: 未处理的项包括
        noiseIndex
        noiseIntensity
        */
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to change fill");
    }
}

nlohmann::json fill_change::construct_from_color(nlohmann::json &&color)
{
    nlohmann::json result = nlohmann::json::object();
    result["class"] = "fill";
    result["isEnabled"] = true;
    result["color"] = std::move(color);
    result["fillType"] = 0;
    context_settings_change::get_default(result["contextSettings"]);
    return result;
}
