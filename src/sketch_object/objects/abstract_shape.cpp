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

#include "./abstract_shape.h"
#include "src/sketch_object/attrs/rect_change.h"
#include "src/sketch_object/utils/point_string_change.h"
#include "src/sketch_object/check.hpp"

void abstract_shape::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    assert(vgg.empty());
    abstract_layer::change(sketch, vgg);
    vgg["class"] = "path";

    try 
    {
        nlohmann::json contour;
        contour["class"] = "contour";
        contour["closed"] = get_json_value(sketch, "isClosed", false);

        try
        {
            abstract_shape::curve_point_change(sketch.at("points"), sketch.at("frame"), contour["points"]);
        }
        catch(...)
        {
            contour["points"] = nlohmann::json::array();
            check::ins_.add_error("failed to change points");
        }
        
        nlohmann::json sub_shape;
        sub_shape["class"] = string("subshape");
        sub_shape["subGeometry"] = std::move(contour);
        sub_shape["booleanOperation"] = this->boolean_operation_;

        nlohmann::json shape;
        shape["class"] = string("shape");
        shape["subshapes"].emplace_back(std::move(sub_shape));
        
        int winding_rule = 1;
        try 
        {
            winding_rule = sketch.at("style").at("windingRule").get<int>();
        }
        catch(...)
        {
        }
        check::ins_.check_range(winding_rule, 0, 1, 1, "invalid winding rule");
        shape["windingRule"] = winding_rule;

        vgg["shape"] = std::move(shape);
    
        /*
        sketch中未处理的属性包括: 
        edited
        pointRadiusBehaviour
        */
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        assert(false);
        throw sketch_exception("fail to analyze abstract shape");
    }       
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
        const double width = sketch_frame.at("width").get<double>();
        const double height = sketch_frame.at("height").get<double>();

        nlohmann::json point;
        nlohmann::json tem;

        for (auto &item : sketch_points)
        {
            assert(item.at("_class").get<string>() == "curvePoint");

            point["class"] = string("pointAttr");

            auto it = item.find("cornerStyle");
            if (it != item.end())
            {
                int corner_style = it->get<int>();
                check::ins_.check_range(corner_style, 0, 3, 0, "invalid corner style");
                point["cornerStyle"] = corner_style;
            }

            // sketch 中, 若一个点有 curve from 或 curve to, 则该点的 radius 不生效, 所以进行手动忽略
            bool ignore_radius = false;

            if (get_json_value(item, "hasCurveFrom", false))
            {
                add_point(x, y, width, height, item.at("curveFrom"), tem);
                point["curveFrom"] = std::move(tem);
                ignore_radius = true;
            }

            if (get_json_value(item, "hasCurveTo", false))
            {
                add_point(x, y, width, height, item.at("curveTo"), tem);
                point["curveTo"] = std::move(tem);
                ignore_radius = true;
            }

            if (!ignore_radius)
            {
                point["radius"] = get_json_value(item, "cornerRadius", 0.0);
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
    curveMode: 已讨论, 不处理
    */
}