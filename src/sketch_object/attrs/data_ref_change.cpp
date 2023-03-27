#include "./data_ref_change.h"

void data_ref_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    try 
    {
        assert(sketch.at("_class").get<string>() == "MSJSONOriginalDataReference");
        vgg["class"] = string("MSJSONOriginalDataReference");

        string ref_class = sketch.at("_ref_class").get<string>();
        if (ref_class != "MSImageData" && ref_class != "MSFontData")
        {
            throw sketch_exception("invalid ref class in data ref");
        }
        vgg["_ref_class"] = ref_class;

        vgg["_ref"] = sketch.at("_ref").get<string>();
        vgg["data"]["_data"] = sketch.at("data").at("_data").get<string>();
        vgg["sha1"]["_data"] = sketch.at("sha1").at("_data").get<string>();
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze data ref");
    }
}