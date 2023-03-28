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

#include "./rect.h"
#include <algorithm>

rect::rect(const double *a_matrix, double width, double height)
{
    auto calc = [&a_matrix](double x, double y, double &x_out, double &y_out)
    {
        x_out = a_matrix[0] * x + a_matrix[2] * y + a_matrix[4];
        y_out = a_matrix[1] * x + a_matrix[3] * y + a_matrix[5];
    };

    double x[4] = {};
    double y[4] = {};
    calc(0, 0, x[0], y[0]);
    calc(width, 0, x[1], y[1]);
    calc(width, -height, x[2], y[2]);
    calc(0, -height, x[3], y[3]);

    double x_min = *std::min_element(x, x + 4);
    double y_min = *std::min_element(y, y + 4);
    double x_max = *std::max_element(x, x + 4);
    double y_max = *std::max_element(y, y + 4);

    this->set(x_min, y_max, x_max, y_min);
}

rect::rect(double x, double y, double width, double height)
{
    x1_ = x;
    y1_ = y;
    
    x2_ = x1_ + width;
    y2_ = y1_ - height;
}

void rect::set(double xf, double yf, double xs, double ys)
{
    x1_ = std::min(xf, xs);
    y1_ = std::max(yf, ys);

    x2_ = std::max(xf, xs);
    y2_ = std::min(yf, ys);
}

bool rect::is_intersect(const rect &rect_1, const rect &rect_2)
{
    // 矩形相交条件:
    //  重心距离在x轴上的投影长度<两个矩形的在x轴的长度之和/2
    //  重心距离在y轴上的投影长度<两个矩形在y轴上的宽度之和/2

    double center_diff_x = std::abs((rect_1.x1_ + rect_1.x2_) / 2.0  - (rect_2.x1_ + rect_2.x2_) / 2.0);
    double center_diff_y = std::abs((rect_1.y1_ + rect_1.y2_) / 2.0  - (rect_2.y1_ + rect_2.y2_) / 2.0);

    if (center_diff_x <= (rect_1.width() + rect_2.width()) / 2 &&
        center_diff_y <= (rect_1.height() + rect_2.height()) / 2)
    {
        return true;
    }

    return false;
}

rect rect::united(const rect &rect_1, const rect &rect_2)
{
    rect out;

    out.x1_ = std::min(rect_1.x1_, rect_2.x1_);
    out.x2_ = std::max(rect_1.x2_, rect_2.x2_);

    out.y1_ = std::max(rect_1.y1_, rect_2.y1_);
    out.y2_ = std::min(rect_1.y2_, rect_2.y2_);

    return out;
}