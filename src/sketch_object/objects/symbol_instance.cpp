#include "./symbol_instance.h"
#include "./symbol_master.h"
#include "src/sketch_object/attrs/file_ref_change.h"
#include "src/sketch_object/attrs/data_ref_change.h"

void symbol_instance::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(sketch.at("_class").get<string>() == "symbolInstance");
    abstract_layer::change(sketch, vgg);

    try 
    {
        vgg["class"] = string("symbolInstance");
        symbol_instance::override_values_change(sketch.at("overrideValues"), vgg["overrideValues"]);
        vgg["symbolID"] = sketch.at("symbolID").get<string>();

        /*
        sketch中未处理的属性包括:
        scale: 无需处理, scale 变化的时候, frame 也会变化
        verticalSpacing
        horizontalSpacing
        preservesSpaceWhenHidden
        */
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze symbol instance");
    }
}

void symbol_instance::override_values_change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg = nlohmann::json::array();

    try 
    {
        if (!sketch.is_array())
        {
            throw;
        }

        for (auto &item : sketch)
        {
            assert(item.at("_class") == "overrideValue");

            nlohmann::json vgg_item;
            vgg_item["class"] = "overrideValue";
            vgg_item["overrideName"] = item.at("overrideName");
            symbol_master::override_name_check(vgg_item["overrideName"].get<string>());

            auto &value = item.at("value");
            if (value.is_string())
            {
                vgg_item["value"] = value;
            }
            else if (value.at("_class").get<string>() == "MSJSONOriginalDataReference")
            {
                data_ref_change::change(value, vgg_item["value"]);
            }
            else if (value.at("_class").get<string>() == "MSJSONFileReference")
            {
                file_ref_change::change(value, vgg_item["value"]);
            }
            else 
            {
                throw sketch_exception("invalid value in symbol instance");
            }

            vgg.emplace_back(std::move(vgg_item));
        }
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze symbol instance");
    }

    /*
    sketch中未处理的属性包括:
    do_objectID
    */
}