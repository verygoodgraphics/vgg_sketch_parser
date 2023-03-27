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

    if (obj_json.find("childObjects") != obj_json.end())
    {
        this->add_parent_mask_to_child(obj_json);
    }
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
    };

    for (auto &item : parent.at("childObjects"))
    {
        add(parent.at("outlineMaskBy"), item.at("outlineMaskBy"));
        add(parent.at("alphaMaskBy"), item.at("alphaMaskBy"));
    }
}
