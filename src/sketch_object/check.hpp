#ifndef bd_sketch_check
#define bd_sketch_check
#include "src/basic/define.h"

/**
 * 校验输入值是否属于指定的范围, 若不属于则触发异常
 * 
 * @param value 被校验的值
 * @param min_value 合法范围的下限(包含)
 * @param max_value 合法范围的上限(包含)
 * @param error_hint 错误信息, 用于生成异常
 * 
 * @exception sketch_exception
*/
template<typename T>
void range_check(T value, T min_value, T max_value, const char* error_hint)
{
    if (value < min_value || value > max_value)
    {
        throw sketch_exception(error_hint);
    }
}

#endif