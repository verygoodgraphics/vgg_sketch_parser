#ifndef bd_sketch_point_string_change
#define bd_sketch_point_string_change
#include "src/basic/define.h"
#include "nlohmann/json.hpp"

class point_string_change
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
     * 提取 sketch 的点, 存于 out 中
     * 
     * @param sketch sketch 的输入
     * @param out 提取结果, 数组第一个元素是 x, 第二个元素是 y
     * 
     * @exception sketch_exception
    */
    static void change(const nlohmann::json &sketch, double *out);

    /**
     * 根据比例、起始坐标、长度, 获取真实坐标
     * 
     * @param ratio 比例
     * @param begin 起始坐标
     * @param length 长度或者宽度
     * 
     * @return 真实坐标, 该坐标是 sketch 坐标系下的坐标
    */
    static double calc_real_coordinate(const double ratio, const double begin, const double length);
};

#endif