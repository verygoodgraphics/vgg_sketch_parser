#include "./symbol_master.h"
#include "src/sketch_object/attrs/color_change.h"
#include "src/sketch_object/mask.h"
#include <regex>
#include <algorithm>
#include <array>

t_child symbol_master::child_;

symbol_master::symbol_master()
{
    init_child(this->child_, 1);
}

void symbol_master::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(vgg.empty());
    abstract_root::change(sketch, vgg);
    mask m_mask;

    try
    {
        assert(sketch.at("_class").get<string>() == "symbolMaster");

        vgg["class"] = string("symbolMaster");
        vgg["hasBackgroundColor"] = sketch.at("hasBackgroundColor").get<bool>();
        color_change::change(sketch.at("backgroundColor"), vgg["backgroundColor"]);
        vgg["includeBackgroundColorInInstance"] = sketch.at("includeBackgroundColorInInstance").get<bool>();
        vgg["symbolID"] = sketch.at("symbolID").get<string>();
        vgg["allowsOverrides"] = sketch.at("allowsOverrides").get<bool>();
        symbol_master::override_properties_change(sketch.at("overrideProperties"), vgg["overrideProperties"]);
        vgg["childObjects"] = nlohmann::json::array();
        
        auto &layers = sketch.at("layers");
        for (auto &item : layers)
        {
            string name = item.at("_class").get<string>();
            auto it = child_.find(name);
            if (it != child_.end())
            {
                nlohmann::json out;
                it->second->change(item, out);

                m_mask.deal_mask(*it->second, out);

                assert(!out.empty());
                vgg["childObjects"].emplace_back(std::move(out));
            }
            else 
            {
                assert(name == "slice" || name == "MSImmutableHotspotLayer");
            }
        }

        /*
        sketch中未处理的属性包括:
        includeBackgroundColorInExport
        isFlowHome
        resizesContent
        presetDictionary
        */
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze symbol master");
    }
}

void symbol_master::override_name_check(const string &str)
{
    //from override-name.yaml
    const static std::array<std::regex, 4> a_re = 
    { 
        std::regex("[0-9A-F]{8}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{12}((_stringValue$)|\\/)"),
        std::regex("[0-9A-F]{8}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{12}((_symbolID$)|\\/)"),
        std::regex("[0-9A-F]{8}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{12}((_image$)|\\/)"),
        std::regex("[0-9A-F]{8}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{4}\\-[0-9A-F]{12}((_layerStyle$)|\\/)")
    };

    if (!std::any_of(a_re.begin(), a_re.end(), [&str](const std::regex &re)
    {
        return std::regex_match(str, re);
    }))
    {
        throw sketch_exception("fail to match override name");
    }
}

void symbol_master::override_properties_change(const nlohmann::json &sketch, nlohmann::json &vgg)
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
            assert(item.at("_class").get<string>() == "MSImmutableOverrideProperty");
            
            nlohmann::json vgg_item;
            vgg_item["class"] = string("MSImmutableOverrideProperty");
            vgg_item["canOverride"] = item.at("canOverride").get<bool>();
            vgg_item["overrideName"] = item.at("overrideName");
            symbol_master::override_name_check(vgg_item["overrideName"].get<string>());

            vgg.emplace_back(std::move(vgg_item));
        }
    }
    catch (...)
    {
        throw sketch_exception("fail to analyze override properties");
    }
}