#ifndef bd_rect
#define bd_rect
#include <algorithm>

/*
该类构造和操作的矩形的特征:
    1.矩形不是倾斜的,即其水平方向和X轴平行
    2.x轴向右递增, y轴向上递增
*/
class rect
{
public:
    rect() {}

    //功能: 由左上角坐标、宽高, 进行构造
    rect(double x, double y, double width, double height);

    /*
    功能: 根据对象的宽、高、旋转矩阵, 计算对象的frame, 依据是 PLRM.pdf 4.3.3 章节
    形参:
        @a_matrix: [a b c d tx ty]
        @width: 原始宽度
        @height: 原始高度
    备注: 计算的点是 (0, 0), (width, 0), (width, -height), (0, -height)
    */
    rect(const double *a_matrix, double width, double height);

public:
    //左上角的点
    double x1_ = 0;
    double y1_ = 0;

    //右下角的点
    double x2_ = 0;
    double y2_ = 0;

public:
    //(xf, yf), (xs, ys)是一组矩形对角线上的点, 有两种组合方式
    void set(double xf, double yf, double xs, double ys);

    double width() const { return std::abs(x2_ - x1_); }
    double height() const { return std::abs(y2_ - y1_); }

public:
    //判断两个矩形是否有交集(含刚好贴在一起的情况)
    static bool is_intersect(const rect &rect_1, const rect &rect_2);

    //求两个矩形的并集
    static rect united(const rect &rect_1, const rect &rect_2);
};

#endif