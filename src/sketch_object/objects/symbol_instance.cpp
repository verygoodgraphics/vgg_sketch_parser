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

#include "./symbol_instance.h"
#include "./symbol_master.h"
#include "src/sketch_object/attrs/file_ref_change.h"
#include "src/sketch_object/attrs/data_ref_change.h"

void symbol_instance::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(sketch.at("_class").get<string>() == "symbolInstance");
    abstract_layer::change(sketch, vgg);

    vgg["class"] = string("symbolInstance");

    vgg["overrideValues"] = nlohmann::json::array();
    auto it = sketch.find("overrideValues");
    if (it != sketch.end())
    {
        symbol_instance::override_values_change(*it, vgg["overrideValues"]);
    }

    get_json_value<string>(sketch, "symbolID", vgg["symbolID"], "fail to get symbol instance attr: symbolID");

    /*
    sketch中未处理的属性包括:
    scale: 无需处理, scale 变化的时候, frame 也会变化
    verticalSpacing
    horizontalSpacing
    preservesSpaceWhenHidden
    */
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