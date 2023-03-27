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

void bitmap::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(sketch.at("_class").get<string>() == "bitmap");

    abstract_layer::change(sketch, vgg);

    vgg["class"] = string("image");
    vgg["fillReplacesImage"] = get_json_value(sketch, "fillReplacesImage", false);
    
    string file_name;
    bitmap::get_image_file_name(*get_json_item(sketch, "image", "fail to get bitmap image"), file_name);
    vgg["imageFileName"] = file_name;

    /*
    sketch中未处理的属性包括:
    intendedDPI
    clippingMask
    */
}

void bitmap::get_image_file_name(const nlohmann::json &sketch, string &out)
{
    out.clear();

    string class_name;
    get_json_value<string>(sketch, "_class", class_name, "fail to get bitmap data attr: _class");

    if (class_name == "MSJSONFileReference")
    {
        string ref_class;
        get_json_value<string>(sketch, "_ref_class", ref_class, "fail to get bitmap image data attr: _ref_class");

        if (ref_class == "MSImageData")
        {
            get_json_value<string>(sketch, "_ref", out, "fail to get bitmap data attr: _ref");

            assert(boost::starts_with(out, extract::_images_dir_name));
            boost::replace_all(out, extract::_images_dir_name, analyze_sketch_file::_out_dir_name);
        }
        else 
        {
            //szn_todo
            throw sketch_exception("now only can deal MSImageData in file-ref");
        }
    }
    else if (class_name == "MSJSONOriginalDataReference")
    {
        //szn_todo
        throw sketch_exception("can not analyze data-ref");
    }
    else 
    {
        throw sketch_exception("unknow bitmap data type");
    }

    assert(boost::starts_with(out, analyze_sketch_file::_out_dir_name) && std::filesystem::is_regular_file(out));
}
