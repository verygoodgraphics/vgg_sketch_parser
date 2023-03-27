#include "./artboard.h"
#include "src/sketch_object/attrs/color_change.h"
#include "src/sketch_object/attrs/rect_change.h"
#include "src/sketch_object/mask.h"

t_child artboard::child_;

artboard::artboard()
{
    init_child(this->child_, 1);
}

void artboard::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(vgg.empty());

    abstract_root::change(sketch, vgg);

    mask m_mask;

    try 
    {
        assert(sketch.at("_class").get<string>() == "artboard");
        vgg["class"] = "artboard";
        vgg["hasBackgroundColor"] = sketch.at("hasBackgroundColor").get<bool>();
        color_change::change(sketch.at("backgroundColor"), vgg["backgroundColor"]);
        
        nlohmann::json vgg_layer;
        abstract_layer::create_default_layer(vgg_layer);

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
                vgg_layer["childObjects"].emplace_back(std::move(out));
            }
            else 
            {
                assert(name == "slice" || name == "MSImmutableHotspotLayer");
            }
        }

        //artboard的起点固定为(0, 0)
        vgg["frame"].at("x") = 0.0;
        vgg["frame"].at("y") = 0.0;
        rect_change::get_default_matrix(vgg["matrix"]);

        vgg_layer["frame"] = vgg.at("frame");
        vgg_layer["bounds"] = vgg.at("bounds");
        vgg["layers"].emplace_back(std::move(vgg_layer));
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to ananlyze artboard");
    }

    /*
    sketch中未处理的属性包括: 
    includeBackgroundColorInExport
    isFlowHome
    resizesContent
    prototypeViewport
    */
}