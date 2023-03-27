#include "./border_change.h"
#include "src/sketch_object/check.hpp"
#include "src/sketch_object/attrs/color_change.h"
#include "src/sketch_object/attrs/gradient_change.h"
#include "src/sketch_object/attrs/context_settings_change.h"

void border_change::change(const nlohmann::json &sketch_border,
    const nlohmann::json *sketch_border_option, nlohmann::json &vgg)
{
    vgg.clear();

    try 
    {
        assert(sketch_border.at("_class").get<string>() == "border");

        vgg["class"] = "border";
        vgg["isEnabled"] = sketch_border.at("isEnabled").get<bool>();
        color_change::change(sketch_border.at("color"), vgg["color"]);

        auto fill_type = border_change::fill_type_change(sketch_border.at("fillType").get<int>());
        vgg["fillType"] = fill_type;

        vgg["position"] = sketch_border.at("position").get<int>();
        range_check(vgg["position"].get<int>(), 0, 2, "invalid border position");

        vgg["thickness"] = sketch_border.at("thickness").get<double>();

        //备注: 在 sketch-schema 1.0 中, contextSettings 和 gradient 是可选的
        {
            auto it = sketch_border.find("contextSettings");
            if (it != sketch_border.end())
            {
                context_settings_change::change(sketch_border.at("contextSettings"), vgg["contextSettings"]);
            }
            else
            {
                context_settings_change::get_default(vgg["contextSettings"]);
            }

            it = sketch_border.find("gradient");
            if (it != sketch_border.end())
            {
                gradient_change::change(sketch_border.at("gradient"), vgg["gradient"]);
            }
            else if (1 == fill_type) 
            {
                throw sketch_exception("fill type is gradient, but gradient is not exists");
            }
        }

        vgg["style"] = 0;
        vgg["dashedPattern"] = nlohmann::json::array();
        vgg["dashedOffset"] = 0.0;

        // 以下的默认值采用 ai 的标准
        vgg["lineCapStyle"] = 0;
        vgg["lineJoinStyle"] = 0;
        vgg["miterLimit"] = 4.0;
        vgg["flat"] = 0.0;
        
        if (sketch_border_option)
        {
            assert(sketch_border_option->at("_class").get<string>() == "borderOptions");
            if (sketch_border_option->at("isEnabled").get<bool>())
            {
                auto &dash_pattern = sketch_border_option->at("dashPattern");
                assert(dash_pattern.is_array());

                for (auto &item : dash_pattern)
                {
                    vgg["dashedPattern"].emplace_back(item.get<double>());
                }
                vgg["style"] = dash_pattern.empty() ? 0 : 2;

                vgg["lineCapStyle"] = sketch_border_option->at("lineCapStyle").get<int>();
                range_check(vgg["lineCapStyle"].get<int>(), 0, 2, "invalid line cap");

                vgg["lineJoinStyle"] = sketch_border_option->at("lineJoinStyle").get<int>();
                range_check(vgg["lineJoinStyle"].get<int>(), 0, 2, "invalid line join");
            }
        }
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to change border");
    }
}

int border_change::fill_type_change(int sketch_fill_type)
{
    if (!sketch_fill_type || 1 == sketch_fill_type)
    {
        return sketch_fill_type;
    }
    else if (4 == sketch_fill_type)
    {
        return 2;
    }

    assert(false);
    throw sketch_exception("invalid fill type");
    return -1;
}
