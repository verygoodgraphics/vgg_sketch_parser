#ifndef bd_sketch_get_json_value
#define bd_sketch_get_json_value

#include "./define.h"
#include "nlohmann/json.hpp"

/**
 * 获取输入的 json 中的给定 key 的值, key 必须存在
 * 
 * @param json_in 输入的 json
 * @param key 访问的 key 名称
 * @param out 输出的 json
 * @param fail_reason 若该字段不为 nullptr 则表示 key 必须存在, 并在访问失败时充当异常原因
 * @param default_value out 的默认值, 仅在 fail_reason 为 nullptr 且 key 访问失败时被使用
 * 
 * @exception sketch_exception
*/
template <typename T>
void json_change(const nlohmann::json &json_in, const char *key, nlohmann::json &out, 
    const char *fail_reason = nullptr, const T &default_value = T())
{
    auto it = json_in.find(key);
    if (it == json_in.end())
    {
        if (fail_reason)
        {
            assert(false);
            throw sketch_exception(fail_reason);
        }
        else 
        {
            out = default_value;
        }
    }
    else 
    {
        out = json_in.at(key).get<T>();
    }
}

#endif