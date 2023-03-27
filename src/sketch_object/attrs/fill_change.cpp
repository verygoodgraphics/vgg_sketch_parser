#include "./fill_change.h"
#include "src/sketch_object/check.hpp"
#include "src/sketch_object/attrs/color_change.h"
#include "src/sketch_object/attrs/border_change.h"
#include "src/sketch_object/attrs/context_settings_change.h"
#include "src/sketch_object/attrs/gradient_change.h"
#include "src/sketch_object/objects/bitmap.h"
#include <fstream>

void fill_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    try 
    {
        assert(sketch.at("_class").get<string>() == "fill");

        vgg["class"] = "fill";
        vgg["isEnabled"] = sketch.at("isEnabled").get<bool>();
        color_change::change(sketch.at("color"), vgg["color"]);
        
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
            else if (1 == fill_type) 
            {
                throw sketch_exception("fill type is gradient, but gradient is not exists");
            }
        }

        //pattern
        if (vgg["fillType"].get<int>() == 2)
        {
            auto &pattern = vgg["pattern"];
            pattern["class"] = "pattern";
            
            auto &instance = pattern["instance"];
            instance["class"] = string("pattern_image");

            instance["fillType"] = sketch.at("patternFillType");
            range_check(instance["fillType"].get<int>(), 0, 3, "invalid pattern fill type");

            instance["imageTileMirrored"] = false;
            instance["imageTileScale"] = sketch.at("patternTileScale").get<double>();

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