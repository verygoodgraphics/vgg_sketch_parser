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

#include "./abstract_layer.h"
#include "src/sketch_object/check.hpp"
#include "src/sketch_object/attrs/rect_change.h"
#include "src/sketch_object/attrs/style_change.h"
#include "src/sketch_object/attrs/context_settings_change.h"
#include "src/sketch_object/objects/artboard.h"
#include "src/sketch_object/objects/abstract_group.h"
#include "src/sketch_object/objects/shape_group.h"
#include "src/sketch_object/objects/shape/rectangle.hpp"
#include "src/sketch_object/objects/shape/oval.hpp"
#include "src/sketch_object/objects/shape/polygon.hpp"
#include "src/sketch_object/objects/shape/shape_path.hpp"
#include "src/sketch_object/objects/shape/star.hpp"
#include "src/sketch_object/objects/shape/triangle.hpp"
#include "src/sketch_object/objects/bitmap.h"
#include "src/sketch_object/objects/text.h"
#include "src/sketch_object/objects/symbol_master.h"
#include "src/sketch_object/objects/symbol_instance.h"

void abstract_layer::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    get_json_value<string>(sketch, "do_objectID", vgg["id"], "fail to get object id");
    this->id_ = vgg["id"].get<string>();
    assert(!this->id_.empty());

    this->boolean_operation_ = get_json_value(sketch, "booleanOperation", -1);
    switch (this->boolean_operation_)
    {
        case -1:
        {
            this->boolean_operation_ = 4;
            break;
        }

        case 0:
        case 1:
        case 2:
        case 3:
        {
            ++this->boolean_operation_;
            break;
        }

        default:
        {
            throw sketch_exception("invalid boolean operator");
        }
    }

    //bounds frame matrix
    {
        rect_change::change(*get_json_item(sketch, "frame", "fail to get obj frame"), vgg["bounds"]);

        double matrix[6] = {};
        rect_change::matrix_calc(matrix,
            get_json_value(sketch, "isFlippedHorizontal", false),
            get_json_value(sketch, "isFlippedVertical", false),
            get_json_value(sketch, "rotation", 0.0),
            vgg["bounds"].at("x").get<double>(), 
            vgg["bounds"].at("y").get<double>(),
            vgg["bounds"].at("width").get<double>(), 
            vgg["bounds"].at("height").get<double>());
        rect_change::form_matrix(matrix, vgg["matrix"]);

        rect_change::calc_frame(matrix, vgg["bounds"], vgg["frame"]);

        // 备注: bounds的起点固定为(0, 0)
        vgg["bounds"].at("x") = 0.0;
        vgg["bounds"].at("y") = 0.0;
    }

    vgg["isLocked"] = get_json_value(sketch, "isLocked", false);
    vgg["visible"] = get_json_value(sketch, "isVisible", true);
    vgg["name"] = get_json_value(sketch, "name", string("empty-name"));

    if (sketch.find("style") == sketch.end())
    {
        style_change::get_default(vgg["style"]);
        context_settings_change::get_default(vgg["contextSettings"]);
    }
    else 
    {
        style_change::change(sketch.at("style"), vgg["style"], vgg["contextSettings"]);
    }

    /*
    hasClippingMask
        true: 自身是蒙版
        false: 自身不是蒙版
    clippingMaskMode 仅在 hasClippingMask 为 true 时有效
        0: outline mask
        1: alpha mask
    */
    
    if (get_json_value(sketch, "hasClippingMask", false))
    {
        get_json_value<int, int>(sketch, "clippingMaskMode", this->mask_type_, "fail to get obj clipping mask mode");
        ++this->mask_type_;
    }
    else 
    {
        this->mask_type_ = 0;
    }
    this->break_mask_chain_ = get_json_value(sketch, "shouldBreakMaskChain", false);

    //这两个属性由子类自行设置
    vgg["alphaMaskBy"] = nlohmann::json::array();
    vgg["outlineMaskBy"] = nlohmann::json::array();

    vgg["isMask"] = static_cast<bool>(this->mask_type_);

    /*
    未处理的项:
    exportOptions
    flow
    isFixedToViewport
    isTemplate
    layerListExpandedType
    nameIsFixed
    resizingConstraint
    resizingType
    sharedStyleID
    userInfo
    maintainScrollPosition
    */
}

void abstract_layer::create_default_layer(nlohmann::json &vgg)
{
    vgg.clear();

    vgg["id"] = string("1");
    vgg["name"] = string("layer");
    vgg["isLocked"] = false;
    vgg["visible"] = true;
    context_settings_change::get_default(vgg["contextSettings"]);
    style_change::get_default(vgg["style"]);

    double matrix[] = { 1, 0, 0, 1, 0, 0 };
    rect_change::form_matrix(matrix, vgg["matrix"]);

    vgg["alphaMaskBy"] = nlohmann::json::array();
    vgg["outlineMaskBy"] = nlohmann::json::array();
    vgg["isMask"] = false;

    vgg["class"] = string("layer");
    vgg["childObjects"] = nlohmann::json::array();
}

void init_child(t_child &child, int type)
{
    if (!child.empty())
    {
        return;
    }

    child["oval"].reset(new oval);
    child["polygon"].reset(new polygon);
    child["rectangle"].reset(new rectangle);
    child["shapePath"].reset(new shape_path);
    child["star"].reset(new star);
    child["triangle"].reset(new triangle);

    child["bitmap"].reset(new bitmap);
    child["text"].reset(new text);

    child["group"].reset(new group);
    child["shapeGroup"].reset(new shape_group);
    
    child["symbolInstance"].reset(new symbol_instance);

    //备注: slice.schema.yaml hotspot.schema.yaml 不进行处理

    switch (type)
    {
        case 0: //page
        {
            child["artboard"].reset(new artboard);
            child["symbolMaster"].reset(new symbol_master);
            break;
        }
        case 1: //artboard
        case 2: //group
        {
            break;
        } 
        default:
        {
            assert(false);
            break;
        }
    }
}
