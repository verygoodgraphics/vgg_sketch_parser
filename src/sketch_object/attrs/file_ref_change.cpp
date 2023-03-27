#include "./file_ref_change.h"

void file_ref_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    try 
    {
        assert(sketch.at("_class").get<string>() == "MSJSONFileReference");
        vgg["class"] = string("MSJSONFileReference");

        string ref_class = sketch.at("_ref_class").get<string>();
        if (ref_class != "MSImageData" && ref_class != "MSImmutablePage" && ref_class != "MSPatch")
        {
            throw sketch_exception("invalid ref class in file ref");
        }
        vgg["_ref_class"] = ref_class;
        vgg["_ref"] = sketch.at("_ref").get<string>();
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze file ref");
    }
}