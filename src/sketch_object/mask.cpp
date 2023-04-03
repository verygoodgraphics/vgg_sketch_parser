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

#include "./mask.h"

void mask::deal_mask(const abstract_layer &obj, nlohmann::json &obj_json)
{
    if (obj.mask_type_)
    {
        this->mask_obj_id_ = obj.id_;
        this->mask_obj_type_ = obj.mask_type_;
    }
    else if (obj.break_mask_chain_)
    {
        this->mask_obj_id_.clear();
    }

    if (!mask_obj_id_.empty())
    {
        assert(mask_obj_type_);

        if (mask_obj_type_ == 1)
        {
            obj_json.at("outlineMaskBy").emplace_back(mask_obj_id_);
        }
        else
        {
            assert(mask_obj_type_ == 2);

            nlohmann::json alpha_json;
            alpha_json["class"] = string("alphaMask");
            alpha_json["id"] = mask_obj_id_;
            alpha_json["alphaType"] = 0;
            alpha_json["crop"] = true;

            obj_json.at("alphaMaskBy").emplace_back(std::move(alpha_json));
        }
    }

    // if (obj_json.find("childObjects") != obj_json.end())
    // {
    //     this->add_parent_mask_to_child(obj_json);
    // }
}

void mask::add_parent_mask_to_child(nlohmann::json &parent)
{
    auto add = [](const nlohmann::json &in, nlohmann::json &out)
    {
        assert(in.is_array() && out.is_array());

        if (in.empty())
        {
            return;
        }

        //确保 mask 按照堆栈顺序存储, 效率比较低
        /*
        auto old = std::move(out);
        assert(out.empty());

        for (auto &item : in)
        {
            out.emplace_back(item);
        }

        for (auto &item : old)
        {
            out.emplace_back(std::move(item));
        }
        */

        //无视 mask 的堆栈顺序, 效率比较高
        for (auto &item : in)
        {
            out.emplace_back(item);
        }
    };

    auto it = parent.find("childObjects");
    if (it == parent.end())
    {
        return;
    }

    for (auto &item : *it)
    {
        add(parent.at("outlineMaskBy"), item.at("outlineMaskBy"));
        add(parent.at("alphaMaskBy"), item.at("alphaMaskBy"));

        add_parent_mask_to_child(item);
    }
}