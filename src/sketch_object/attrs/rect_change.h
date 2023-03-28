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

#ifndef bd_sketch_rect_change
#define bd_sketch_rect_change
#include "src/basic/define.h"
#include "nlohmann/json.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "src/basic/rect.h"

class rect_change
{
public:
    /**
     * 将 sketch 的输入, 转为符合 vgg-format 的输出
     * 
     * @param sketch sketch 的输入
     * @param vgg 符合 vgg-format 的输出
     * 
     * @exception sketch_exception
     * 
     * @note 会对 vgg 先进行 clear
    */
    static void change(const nlohmann::json &sketch, nlohmann::json &vgg);

    /**
     * 由对象的一些基础属性, 获取其旋转矩阵
     * 
     * @param matrix 输出的旋转矩阵 [a, b, c, d, tx, ty]
     * @param flip_h 是否水平翻转
     * @param flip_v 是否垂直翻转
     * @param rotation 旋转角度, 单位: 度, 逆时针为正
     * @param x 左上角 x 坐标
     * @param y 左上角 y 坐标
     * @param width 宽度
     * @param height 高度
    */
    static void matrix_calc(double *matrix, bool flip_h, bool flip_v, 
        double rotation, double x, double y, double width, double height);
    
    /**
     * 由给定的 [a, b, c, d, tx, ty] 组成符合 vgg-format 的 matrix
     * 
     * @param matrix 输入的矩阵的 6 个参数
     * @param json_out 输出的 json, 是个数组类型的值
    */
    static void form_matrix(const double *matrix, nlohmann::json &json_out);

    /**
     * 获取默认的旋转矩阵
     * 
     * @param json_out 输出的旋转矩阵
    */
    static void get_default_matrix(nlohmann::json &json_out);

    /**
     * 由对象的 matrix 和 bounds, 获取对象的 frame
     * 
     * @param matrix 对象的旋转矩阵
     * @param bounds 对象的 bounds
     * @param frame 获取到的 frame
    */
    static void calc_frame(const double* matrix, const nlohmann::json &bounds, nlohmann::json &frame);

    /**
     * 将 sketch 的 x 坐标, 转为 vgg-format 的 x 坐标
     * 
     * @param x sketch 的坐标
     * 
     * @return vgg-format 的坐标
    */
    static double coordinate_x_change(double x);

    /**
     * 将 sketch 的 y 坐标, 转为 vgg-format 的 y 坐标
     * 
     * @param y sketch 的坐标
     * 
     * @return vgg-format 的坐标
    */
    static double coordinate_y_change(double y);    

    /**
     * 从 rect 构造
     * 
     * @param frame rect
     * @param json_out 符合 vgg-format 的输出(rect.yaml)
    */
    static void from_rect(const rect &frame, nlohmann::json& json_out);

private:
    /**
     * 求以中心为旋转点, 旋转指定的角度的旋转矩阵
     * 
     * @param angle 旋转角度, 单位: 度, 逆时针为正
     * @param width 宽度
     * @param height 高度
     * 
     * @return 所求的 3 * 3 的旋转矩阵
     * 
     * @note 
     * 参考: 
     *  https://web.cse.ohio-state.edu/~shen.94/681/Site/Slides_files/transformation_review.pdf
     *  PLRM.pdf
     * 
     * 适用的情况:
     * 
     *  ^ (y)
     *  |
     *  |
     *  |-----------> (x)
     *  |          | 
     *  |  rect    | 
     *  |          | 
     *  |          | 
     *  |----------| 
     *  |           
     *  |
    */
    static boost::numeric::ublas::matrix<double> rotation(double angle, double width, double height);

    /**
     * 求水平翻转或垂直翻转的旋转矩阵
     * 
     * @param length 宽度或高度, 实际意义取决于 horizontal
     * @param horizontal true: 水平翻转, false: 垂直翻转
     * 
     * @return 所求的 3 * 3 的旋转矩阵
    */
    static boost::numeric::ublas::matrix<double> flip(double length, bool horizontal);
};

#endif