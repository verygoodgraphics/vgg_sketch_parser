#include "./point_string_change.h"
#include <regex>

void point_string_change::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    vgg.clear();

    double out[2] = {};
    point_string_change::change(sketch, out);

    vgg.emplace_back(out[0]);
    vgg.emplace_back(out[1]);
}

void point_string_change::change(const nlohmann::json &sketch, double *out)
{
    try 
    {
        //备注: 这个 regex 修改自 point-string-schema.yaml
        static std::regex re("^\\{(-?\\d+(.\\d+)?(e-\\d+)?), (-?\\d+(.\\d+)?(e-\\d+)?)\\}$");
        std::smatch sm;

        auto str = sketch.get<string>();
        if (!std::regex_match(str, sm, re) || sm.size() != 7)
        {
            assert(false);
            throw;
        }

        out[0] = strtod(sm[1].str().c_str(), nullptr);
        out[1] = strtod(sm[4].str().c_str(), nullptr);
    }
    catch(...)
    {
        throw sketch_exception("fail to analyze point string");
    }
}

double point_string_change::calc_real_coordinate(const double ratio, const double begin, const double length)
{
    //assert(ratio >= 0 && ratio <= 1 && length >= 0);
    return begin + ratio * length;
}
