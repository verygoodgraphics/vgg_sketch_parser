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

#include "./abstract_group.h"
#include "src/sketch_object/mask.h"

t_child abstract_group::child_;

abstract_group::abstract_group()
{
    init_child(child_);
}

void abstract_group::change(const nlohmann::json &sketch, nlohmann::json &vgg)
{
    abstract_layer::change(sketch, vgg);
    assert(sketch.at("_class").get<string>() == "group");

    mask m_mask;

    vgg["class"] = "group";
    vgg["childObjects"] = nlohmann::json::array();

    try 
    {
        auto &layers = sketch.at("layers");
        for (auto &item : layers)
        {
            string name = item.at("_class");
            auto it = child_.find(name);
            
            if (it != child_.end())
            {
                nlohmann::json out;
                it->second->change(item, out);
                
                m_mask.deal_mask(*it->second, out);

                assert(!out.empty());
                vgg["childObjects"].emplace_back(std::move(out));
            }
            else 
            {
                assert(name == "slice" || name == "MSImmutableHotspotLayer");
            }
        }

        /*
        sketch中未处理的属性包括:
        hasClickThrough
        groupLayout
        */
    }
    catch(sketch_exception &e)
    {
        throw e;
    }
    catch(...)
    {
        assert(false);
        throw sketch_exception("fail to analyze group");
    }
}