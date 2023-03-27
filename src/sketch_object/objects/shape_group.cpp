#include "./shape_group.h"
#include "src/sketch_object/check.hpp"

t_child shape_group::child_;

shape_group::shape_group()
{
    init_child(child_, 2);
}

void shape_group::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    abstract_layer::change(sketch, vgg);
    assert(sketch.at("_class").get<string>() == "shapeGroup");

    try 
    {
        vgg["class"] = string("path");

        auto &shape = vgg["shape"];
        shape["class"] = string("shape");
        shape["windingRule"] = sketch.at("windingRule");
        range_check(shape["windingRule"].get<int>(), 0, 1, "invalid winding rule");

        auto &layers = sketch.at("layers");
        for (auto &item : layers)
        {
            string name = item.at("_class").get<string>();
            auto it = child_.find(name);
            if (it != child_.end())
            {
                nlohmann::json out;
                it->second->change(item, out);
                assert(!out.empty());

                nlohmann::json sub_shape;
                sub_shape["class"] = string("subshape");
                sub_shape["subGeometry"] = std::move(out);
                sub_shape["booleanOperation"] = it->second->boolean_operation_;

                shape["subshapes"].emplace_back(std::move(sub_shape));
            }
            else 
            {
                assert(name == "slice" || name == "MSImmutableHotspotLayer");
            }
        }

        /*
        sketch中未处理的属性包括:
        hasClickThrough
        groupLayout
        */
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze shape-group");
    }
}
