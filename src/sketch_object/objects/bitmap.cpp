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

#include "./bitmap.h"
#include "boost/algorithm/string.hpp"
#include "src/analyze_sketch_file/analyze_sketch_file.h"
#include "src/sketch_object/attrs/color_control_change.h"
#include "src/sketch_object/check.hpp"

void bitmap::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(sketch.at("_class").get<string>() == "bitmap");
    abstract_layer::change(sketch, vgg);

    vgg["class"] = string("image");
    vgg["fillReplacesImage"] = get_json_value(sketch, "fillReplacesImage", false);
    
    try 
    {
        string file_name;
        bitmap::get_image_file_name(sketch.at("image"), file_name);
        vgg["imageFileName"] = file_name;

        /*
        sketch中未处理的属性包括:
        intendedDPI
        clippingMask
        */
    }
    catch(sketch_exception &e)
    {
        //throw e;
        check::ins_.add_error(e.get());
        vgg["imageFileName"] = string("image not found");
    }
    catch(...)
    {
        //assert(false);
        //throw sketch_exception("fail to analyze bitmap");

        check::ins_.add_error("fail to analyze bitmap");
        vgg["imageFileName"] = string("image not found"); 
    }

    //colorControls 在 sketch-schema 1.0 中是可选的
    try
    {
        nlohmann::json image_adjust;
        image_adjust["class"] = std::string("imageAdjust");
        color_control_change::change(sketch.at("style").at("colorControls"), image_adjust["instance"]);

        vgg["imageAdjust"] = std::move(image_adjust);
    }
    catch(...)
    {
    }
}

void bitmap::get_image_file_name(const nlohmann::json &sketch, string &out)
{
    out.clear();

    try 
    {
        string class_name = sketch.at("_class").get<string>();
        assert(class_name == "MSJSONFileReference" || class_name == "MSJSONOriginalDataReference");

        if (class_name == "MSJSONFileReference")
        {
            if (sketch.at("_ref_class") == "MSImageData")
            {
                out = sketch.at("_ref").get<string>();
                assert(boost::starts_with(out, extract::_images_dir_name));
                boost::replace_all(out, extract::_images_dir_name, analyze_sketch_file::_image_dir_name);
            }
            else 
            {
                //szn_todo
                throw sketch_exception("now only can deal MSImageData in file-ref");
            }
        }
        else 
        {
            //szn_todo
            throw sketch_exception("can not analyze data-ref");
        }
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to get image file name");
    }
    
    //assert(boost::starts_with(out, analyze_sketch_file::_image_dir_name) && std::filesystem::is_regular_file(out));
}
