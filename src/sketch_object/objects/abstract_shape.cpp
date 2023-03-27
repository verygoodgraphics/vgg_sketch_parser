#include "./abstract_shape.h"
#include "src/sketch_object/attrs/rect_change.h"
#include "src/sketch_object/utils/point_string_change.h"
#include "src/sketch_object/check.hpp"

void abstract_shape::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(vgg.empty());
    abstract_layer::change(sketch, vgg);

    try 
    {
        vgg["class"] = "path";

        nlohmann::json contour;
        contour["class"] = "contour";
        contour["closed"] = sketch.at("isClosed").get<bool>();
        abstract_shape::curve_point_change(sketch.at("points"), sketch.at("frame"), contour["points"]);

        nlohmann::json sub_shape;
        sub_shape["class"] = string("subshape");
        sub_shape["subGeometry"] = std::move(contour);
        sub_shape["booleanOperation"] = this->boolean_operation_;

        nlohmann::json shape;
        shape["class"] = string("shape");
        shape["subshapes"].emplace_back(std::move(sub_shape));
        
        try 
        {
            shape["windingRule"] = sketch.at("style").at("windingRule");
        }
        catch(...)
        {
            shape["windingRule"] = 1;
        }
        range_check(shape["windingRule"].get<int>(), 0, 1, "invalid winding rule");

        vgg["shape"] = std::move(shape);
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("abstract shape analyze fail");
    }

    /*
    sketch中未处理的属性包括: 
    edited
    pointRadiusBehaviour
    */
}

void abstract_shape::curve_point_change(const nlohmann::json &sketch_points, 
    const nlohmann::json &sketch_frame, nlohmann::json &vgg)
{
    vgg.clear();
    assert(sketch_points.is_array());
    assert(sketch_frame.at("_class") == "rect");
    
    auto add_point = [](double x, double y, double width, double height, 
        const nlohmann::json &sketch_point, nlohmann::json &vgg_point)
    {
        assert(vgg_point.empty());

        double point[2] = {};
        point_string_change::change(sketch_point, point);

        point[0] = point_string_change::calc_real_coordinate(point[0], x, width);
        point[1] = point_string_change::calc_real_coordinate(point[1], y, height);

        point[0] = rect_change::coordinate_x_change(point[0]);
        point[1] = rect_change::coordinate_y_change(point[1]);

        vgg_point.emplace_back(point[0]);
        vgg_point.emplace_back(point[1]);
    };

    try 
    {
        //备注: 在旋转矩阵里已经考虑了移动, 所以 bounds 的 x y 固定为0, 所以此处不用 frame 的 x y, 而是(0, 0)
        const double x = 0;
        const double y = 0;
        const double height = sketch_frame.at("height").get<double>();
        const double width = sketch_frame.at("width").get<double>();

        nlohmann::json point;
        nlohmann::json tem;

        for (auto &item : sketch_points)
        {
            assert(item.at("_class").get<string>() == "curvePoint");

            point["class"] = string("pointAttr");
            point["radius"] = item.at("cornerRadius").get<double>();

            auto it = item.find("cornerStyle");
            if (it != item.end())
            {
                point["cornerStyle"] = *it;
                range_check(point["cornerStyle"].get<int>(), 0, 3, "invalid corner style");
            }

            if (item.at("hasCurveFrom").get<bool>())
            {
                add_point(x, y, width, height, item.at("curveFrom"), tem);
                point["curveFrom"] = std::move(tem);
            }

            if (item.at("hasCurveTo").get<bool>())
            {
                add_point(x, y, width, height, item.at("curveTo"), tem);
                point["curveTo"] = std::move(tem);
            }

            add_point(x, y, width, height, item.at("point"), tem);
            point["point"] = std::move(tem);

            vgg.emplace_back(std::move(point));
        }
    }
    catch (sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze curve point");
    }

    /*
    sketch中未处理的属性包括: 
    cornerStyle: 已讨论, 不处理
    */
}