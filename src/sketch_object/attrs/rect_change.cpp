#include "./rect_change.h"
#include "src/basic/rect.h"

namespace ublas = boost::numeric::ublas;

void rect_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    try
    {
        assert(sketch.at("_class").get<string>() == "rect");

        vgg["constrainProportions"] = sketch.at("constrainProportions").get<bool>();
        vgg["height"] = sketch.at("height").get<double>();
        vgg["width"] = sketch.at("width").get<double>();
        vgg["x"] = rect_change::coordinate_x_change(sketch.at("x").get<double>());
        vgg["y"] = rect_change::coordinate_y_change(sketch.at("y").get<double>());
        vgg["class"] = string("rect");
    }
    catch (...)
    {
        throw sketch_exception("analyze sketch rect fail");
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

    if (flip_h)
    {
        a = ublas::matrix<double>(ublas::prod(a, rect_change::flip(width, true)));
    }

    if (flip_v)
    {
        a = ublas::matrix<double>(ublas::prod(a, rect_change::flip(height, false)));
    }

    if (rotation)
    {
        a = ublas::matrix<double>(ublas::prod(a, rect_change::rotation(rotation, width, height)));
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
    //备注: 726.0 这个神奇数字, 来自测试
    //return x - 726.0;
    return x;
}

double rect_change::coordinate_y_change(double y)
{
    //备注: 460.0 这个神奇数字, 来自测试
    //double tem = y - 460.0;
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