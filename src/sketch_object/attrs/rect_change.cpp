﻿/*
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

#include "./rect_change.h"
#include "src/basic/rect.h"
#include "src/basic/get_json_value.hpp"
#include "src/sketch_object/check.hpp"

namespace ublas = boost::numeric::ublas;

void rect_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    try
    {
        assert(sketch.at("_class").get<string>() == "rect");

        vgg["constrainProportions"] = get_json_value(sketch, "constrainProportions", false);
        vgg["height"] = sketch.at("height").get<double>();
        vgg["width"] = sketch.at("width").get<double>();
        vgg["x"] = rect_change::coordinate_x_change(sketch.at("x").get<double>());
        vgg["y"] = rect_change::coordinate_y_change(sketch.at("y").get<double>());
        vgg["class"] = string("rect");
    }
    catch (...)
    {
        // throw sketch_exception("analyze sketch rect fail");

        // 脏代码, 若真运行到此, 那么 bounds 可能要自己算
        vgg["constrainProportions"] = false;
        vgg["height"] = 1.0;
        vgg["width"] = 1.0;
        vgg["x"] = 1.0;
        vgg["y"] = 1.0;
        vgg["class"] = string("rect");
        check::ins_.add_error("analyze sketch rect fail");
    }
}

void rect_change::matrix_calc(double *matrix, bool flip_h, bool flip_v, 
    double rotation, double x, double y, double width, double height)
{
    ublas::matrix<double> a(3, 3, 1);
    a(0, 0) = 1;
    a(0, 1) = 0;
    a(0, 2) = 0;
    a(1, 0) = 0;
    a(1, 1) = 1;
    a(1, 2) = 0;
    a(2, 0) = 0;
    a(2, 1) = 0;
    a(2, 2) = 1;

    /*
    备注: 
        1.这里必须把旋转放在前面, 理由是旋转的角度的正负的定义问题
        2.在 sketch 中, 经过水平翻转或垂直翻转后的 obj, 旋转30°, 和一个没有经过翻转的 obj 旋转 30°, 它们的旋转方向是不一样的
    */
    if (rotation)
    {
        a = ublas::matrix<double>(ublas::prod(a, rect_change::rotation(rotation, width, height)));
    }

    if (flip_h)
    {
        a = ublas::matrix<double>(ublas::prod(a, rect_change::flip(width, true)));
    }

    if (flip_v)
    {
        a = ublas::matrix<double>(ublas::prod(a, rect_change::flip(height, false)));
    }
    
    matrix[0] = a(0, 0);
    matrix[1] = a(0, 1);
    matrix[2] = a(1, 0);
    matrix[3] = a(1, 1);
    matrix[4] = a(2, 0) + x;
    matrix[5] = a(2, 1) + y;
}

void rect_change::form_matrix(const double *matrix, nlohmann::json &json_out)
{
    json_out.clear();

    for (int i = 0; i < 6; ++i)
    {
        json_out.emplace_back(matrix[i]);
    }
}

void rect_change::get_default_matrix(nlohmann::json &json_out)
{
    double matrix[6] = { 1, 0, 0, 1, 0, 0 };
    rect_change::form_matrix(matrix, json_out);
}

void rect_change::calc_frame(const double* matrix, const nlohmann::json &bounds, nlohmann::json &frame)
{
    double width = bounds.at("width").get<double>();
    double height = bounds.at("height").get<double>();

    rect frame_rect(matrix, width, height);

    frame.clear();
    frame["class"] = "rect";
    frame["constrainProportions"] = bounds.at("constrainProportions");
    frame["height"] = frame_rect.height();
    frame["width"] = frame_rect.width();
    frame["x"] = frame_rect.x1_;
    frame["y"] = frame_rect.y1_;
}

double rect_change::coordinate_x_change(double x)
{
    return x;
}

double rect_change::coordinate_y_change(double y)
{
    double tem = y;
    return tem ? -tem : 0;
}

ublas::matrix<double> rect_change::rotation(double angle, double width, double height)
{
    double center_x = width / 2.0;
    double center_y = -height / 2.0;
    double cos_theta = cos(angle / 180.0 * 3.1415926);
    double sin_theta = sin(angle / 180.0 * 3.1415926);

    ublas::matrix<double> a(3, 3, 1);
    a(0, 0) = 1;
    a(0, 1) = 0;
    a(0, 2) = center_x;
    a(1, 0) = 0;
    a(1, 1) = 1;
    a(1, 2) = center_y;
    a(2, 0) = 0;
    a(2, 1) = 0;
    a(2, 2) = 1;

    ublas::matrix<double> b(3, 3, 1);
    b(0, 0) = cos_theta;
    b(0, 1) = -sin_theta;
    b(0, 2) = 0;
    b(1, 0) = sin_theta;
    b(1, 1) = cos_theta;
    b(1, 2) = 0;
    b(2, 0) = 0;
    b(2, 1) = 0;
    b(2, 2) = 1;

    ublas::matrix<double> c(3, 3, 1);
    c(0, 0) = 1;
    c(0, 1) = 0;
    c(0, 2) = -center_x;
    c(1, 0) = 0;
    c(1, 1) = 1;
    c(1, 2) = -center_y;
    c(2, 0) = 0;
    c(2, 1) = 0;
    c(2, 2) = 1;

    auto tem = ublas::matrix<double>(ublas::prod(a, b));
    auto result = ublas::matrix<double>(ublas::prod(tem, c));

    //备注: 由于矩阵表示方式的区别, 故而进行以下转换
    std::swap(result(0, 1), result(1, 0));
    std::swap(result(0, 2), result(2, 0));
    std::swap(result(1, 2), result(2, 1));

    return result;
}

ublas::matrix<double> rect_change::flip(double length, bool horizontal)
{
    ublas::matrix<double> a(3, 3, 1);

    if (horizontal)
    {
        a(0, 0) = -1;
        a(0, 1) = 0;
        a(0, 2) = 0;
        a(1, 0) = 0;
        a(1, 1) = 1;
        a(1, 2) = 0;
        a(2, 0) = length;
        a(2, 1) = 0;
        a(2, 2) = 1;
    }
    else 
    {
        a(0, 0) = 1;
        a(0, 1) = 0;
        a(0, 2) = 0;
        a(1, 0) = 0;
        a(1, 1) = -1;
        a(1, 2) = 0;
        a(2, 0) = 0;
        a(2, 1) = -length;
        a(2, 2) = 1;
    }

    return a;
}

void rect_change::from_rect(const rect &frame, nlohmann::json& json_out)
{
    json_out.clear();
    json_out["class"] = "rect";
    json_out["constrainProportions"] = false;
    json_out["height"] = frame.height();
    json_out["width"] = frame.width();
    json_out["x"] = frame.x1_;
    json_out["y"] = frame.y1_;
}