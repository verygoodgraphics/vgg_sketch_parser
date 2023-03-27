#include "./abstract_group.h"
#include "src/sketch_object/mask.h"

t_child abstract_group::child_;

abstract_group::abstract_group()
{
    init_child(child_, 2);
}

void abstract_group::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    abstract_layer::change(sketch, vgg);

    assert(sketch.at("_class").get<string>() == "group");
   
    mask m_mask;

    try 
    {
        vgg["class"] = "group";
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
    }
    catch (sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze group");
    }

    /*
    sketch中未处理的属性包括:
    hasClickThrough
    groupLayout
    */
}