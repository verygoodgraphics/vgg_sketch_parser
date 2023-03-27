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
    try
    {
        this->id_ = sketch.at("do_objectID").get<string>();
        vgg["id"] = this->id_;
        assert(!this->id_.empty());

        this->boolean_operation_ = sketch.at("booleanOperation").get<int>();
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
            rect_change::change(sketch.at("frame"), vgg["bounds"]);
            
            double matrix[6] = {};
            rect_change::matrix_calc(matrix, sketch.at("isFlippedHorizontal").get<bool>(),
                sketch.at("isFlippedVertical").get<bool>(), sketch.at("rotation").get<double>(),
                vgg["bounds"].at("x").get<double>(), vgg["bounds"].at("y").get<double>(),
                vgg["bounds"].at("width").get<double>(), vgg["bounds"].at("height").get<double>());
            rect_change::form_matrix(matrix, vgg["matrix"]);

            rect_change::calc_frame(matrix, vgg["bounds"], vgg["frame"]);

            //备注: bounds的起点固定为(0, 0)
            vgg["bounds"].at("x") = 0.0;
            vgg["bounds"].at("y") = 0.0;
        }

        vgg["isLocked"] = sketch.at("isLocked").get<bool>();
        vgg["visible"] = sketch.at("isVisible").get<bool>();
        vgg["name"] = sketch.at("name").get<string>();

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
        if (sketch.at("hasClippingMask").get<bool>())
        {
            this->mask_type_ = sketch.at("clippingMaskMode").get<int>() + 1;
        }
        else 
        {
            this->mask_type_ = 0;
        }
        this->break_mask_chain_ = sketch.at("shouldBreakMaskChain").get<bool>();

        //这两个属性由子类自行设置
        vgg["alphaMaskBy"] = nlohmann::json::array();
        vgg["outlineMaskBy"] = nlohmann::json::array();

        vgg["isMask"] = static_cast<bool>(this->mask_type_);
    }
    catch (sketch_exception &e)
    {
        throw e;
    }
    catch (...)
    {
        assert(false);
        throw sketch_exception("fail to change abstract layer");
    }

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
