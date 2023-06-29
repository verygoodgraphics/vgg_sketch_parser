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

#include <optional>
#include "./page.h"
#include "src/sketch_object/mask.h"
#include "src/sketch_object/attrs/context_settings_change.h"
#include "src/sketch_object/attrs/style_change.h"
#include "src/sketch_object/attrs/rect_change.h"

t_child page::child_;
size_t page::default_artboard_id_ = 1;

page::page()
{
    init_child(this->child_, 0);
}

void page::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    //备注: 无需解析 page 的基础属性
    //abstract_root::change(sketch, vgg);

    //default-artboard 用于收集位于 artboard 以外的对象
    mask default_artboard_mask;
    std::optional<nlohmann::json> default_artboard;
    std::optional<rect> default_artboard_rect;

    assert(sketch.at("_class").get<string>() == "page");
    
    auto &layers = sketch.at("layers");
    for (auto &item : layers)
    {
        string name = item.at("_class").get<string>();
        auto it = child_.find(name);
        
        if (it != child_.end())
        {
            nlohmann::json out;
            it->second->change(item, out);

            if (name == "artboard")
            {
                vgg["artboard"].emplace_back(std::move(out));
                default_artboard_mask.reset_mask();
            }
            else if (name == "symbolMaster")
            {
                vgg["symbolMaster"].emplace_back(std::move(out));
            }
            else 
            {
                if (!default_artboard)
                {
                    default_artboard.emplace(nlohmann::json::object());
                    page::create_default_artboard(*default_artboard);

                    nlohmann::json layer;
                    abstract_layer::create_default_layer(layer);
                    (*default_artboard)["layers"].emplace_back(std::move(layer));
                }

                assert(default_artboard->at("layers").is_array());
                assert(default_artboard->at("layers").size() == 1);
                auto &layer = default_artboard->at("layers").front();

                auto &frame = out.at("frame");
                rect child_rect(frame.at("x").get<double>(), frame.at("y").get<double>(), 
                    frame.at("width").get<double>(), frame.at("height").get<double>());
                if (default_artboard_rect)
                {
                    default_artboard_rect.emplace(rect::united(*default_artboard_rect, child_rect));
                }
                else 
                {
                    default_artboard_rect.emplace(child_rect);
                }
                
                default_artboard_mask.deal_mask(*it->second, out);
                layer["childObjects"].emplace_back(std::move(out));
            }
        }
        else 
        {
            assert(name == "slice" || name == "MSImmutableHotspotLayer");
        }
    }

    if (default_artboard)
    {
        //将 artboard 的起点置为 (0, 0)
        assert(default_artboard_rect);
        double x = default_artboard_rect->x1_;
        double y = default_artboard_rect->y1_;
        default_artboard_rect->set(0, 0, default_artboard_rect->x2_ - x, default_artboard_rect->y2_ - y);

        auto &layer = default_artboard->at("layers").front();
        for (auto &item : layer.at("childObjects"))
        {
            item.at("frame").at("x") = item.at("frame").at("x").get<double>() - x;
            item.at("frame").at("y") = item.at("frame").at("y").get<double>() - y;
            item.at("matrix").at(4)  = item.at("matrix").at(4).get<double>() - x;
            item.at("matrix").at(5)  = item.at("matrix").at(5).get<double>() - y;
        }

        rect_change::from_rect(*default_artboard_rect, (*default_artboard)["frame"]);
        (*default_artboard)["bounds"] = default_artboard->at("frame");
        layer["frame"] = default_artboard->at("frame");
        layer["bounds"] = default_artboard->at("frame");

        vgg["artboard"].emplace_back(std::move(*default_artboard));
    }
}

void page::create_default_artboard(nlohmann::json &out)
{
    out.clear();

    out["id"] = std::to_string(page::default_artboard_id_++);
    out["name"] = string("artboard");
    out["isLocked"] = false;
    out["visible"] = true;
    context_settings_change::get_default(out["contextSettings"]);
    style_change::get_default(out["style"]);
    rect_change::get_default_matrix(out["matrix"]);
    out["alphaMaskBy"] = nlohmann::json::array();
    out["outlineMaskBy"] = nlohmann::json::array();
    //out["isMask"] = false;
    out["maskType"] = 0;

    out["class"] = "artboard";
    out["hasBackgroundColor"] = false;

    out["overflow"] = 1;
    out["style_effect_mask_area"] = 0;    
}
