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
        
        //备注: 确保 color 的存在性, 当 fillType 不在预期内时, 强制使用 color
        try 
        {
            color_change::change(sketch.at("color"), vgg["color"]);
        }       
        catch(...)
        {
            color_change::get_default(vgg["color"]);
        }

        int fill_type = border_change::fill_type_change(sketch.at("fillType"));
        vgg["fillType"] = fill_type;

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

            // 备注: 存在一个用例, 其 isEnabled 为 false, 其 fillType 为渐变, 但 gradient 不存在
            // else if (1 == fill_type) 
            // {
            //     throw sketch_exception("fill type is gradient, but gradient is not exists");
            // }
        }

        //pattern
        if (vgg["fillType"].get<int>() == 2)
        {
            auto &pattern = vgg["pattern"];
            pattern["class"] = "pattern";
            
            auto &instance = pattern["instance"];
            instance["class"] = string("pattern_image");

            instance["fillType"] = get_json_value(sketch, "patternFillType", 0);
            range_check(instance["fillType"].get<int>(), 0, 3, "invalid pattern fill type");

            instance["imageTileMirrored"] = false;
            instance["imageTileScale"] = get_json_value(sketch, "patternTileScale", 0.0);

            auto it_image = sketch.find("image");
            if (it_image == sketch.end())
            {
                //szn_todo
                //备注: image 在 sketch 中不是必选项, 但目前的处理方式下, 其是必须的
                throw sketch_exception("although image is not required in sketch, it is currently required in vgg-sketch-parser");
            }

            string file_name;
            bitmap::get_image_file_name(*it_image, file_name);
            instance["imageFileName"] = file_name;
        }

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