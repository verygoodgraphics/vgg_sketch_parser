#include "./bitmap.h"
#include "boost/algorithm/string.hpp"
#include "src/analyze_sketch_file/analyze_sketch_file.h"

void bitmap::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(sketch.at("_class").get<string>() == "bitmap");

    abstract_layer::change(sketch, vgg);

    try 
    {
        vgg["class"] = string("image");
        vgg["fillReplacesImage"] = sketch.at("fillReplacesImage").get<bool>();
        
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
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze bitmap");
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
                boost::replace_all(out, extract::_images_dir_name, analyze_sketch_file::_out_dir_name);
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
    
    assert(boost::starts_with(out, analyze_sketch_file::_out_dir_name) && std::filesystem::is_regular_file(out));
}
