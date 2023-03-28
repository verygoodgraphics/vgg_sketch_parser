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
