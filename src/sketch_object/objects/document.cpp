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

#include "document.h"
#include "text.h"
#include "symbol_master.h"
#include "../attrs/style_change.h"
#include "src/basic/get_json_value.hpp"
#include "src/sketch_object/check.hpp"

unordered_map<string, int> document::text_vertical_align_;

nlohmann::json document::change(const nlohmann::json &sketch)
{
    document::text_vertical_align_.clear();

    nlohmann::json out = nlohmann::json::array();

    auto it_foreign_symbols = sketch.find("foreignSymbols");
    if (it_foreign_symbols != sketch.end())
    {
        symbol_master master;
        for (auto &item : *it_foreign_symbols)
        {
            auto it = item.find("symbolMaster");
            if (it == item.end() || get_json_value(*it, "_class", std::string("")) != "symbolMaster")
            {
                assert(false);
                continue;
            }
            else 
            {
                nlohmann::json tmp;
                master.change(*it, tmp);
                out.emplace_back(std::move(tmp));
            }
        }
    }

    auto add_ref_style = [&out](const nlohmann::json &obj, bool nest)
    {
        try 
        {
            nlohmann::json ref_style;
            ref_style["class"] = "referencedStyle";

            if (nest)
            {
                ref_style["id"] = obj.at("localSharedStyle").at("do_objectID");
                style_change::change(obj.at("localSharedStyle").at("value"), ref_style["style"], ref_style["contextSettings"], &ref_style["fontAttr"]);
                document::text_vertical_align_[ref_style["id"].get<string>()] = text::chagne_vertical_alignment(obj.at("localSharedStyle").at("value").at("textStyle"));
            }
            else 
            {
                ref_style["id"] = obj.at("do_objectID");
                style_change::change(obj.at("value"), ref_style["style"], ref_style["contextSettings"], &ref_style["fontAttr"]);
                document::text_vertical_align_[ref_style["id"].get<string>()] = text::chagne_vertical_alignment(obj.at("value").at("textStyle"));
            }

            if (ref_style["fontAttr"] == nlohmann::json())
            {
                ref_style.erase("fontAttr");
            }

            out.emplace_back(std::move(ref_style));
        }
        catch(...)
        {
            check::ins_.add_error("failed to change shared style");
        }
    };

    {
        string names[] = { "foreignLayerStyles", "foreignTextStyles" };
        for (auto &name : names)
        {
            for (auto &obj : sketch.value(name, nlohmann::json::array()))
            {
                add_ref_style(obj, true);
            }
        }
    }
    {
        string names[] = { "layerStyles", "layerTextStyles" };
        for (auto &name : names)
        {
            try 
            {
                for (auto &obj : sketch.at(name).at("objects"))
                {
                    add_ref_style(obj, false);
                }
            }
            catch(...)
            {
            }
        }
    }

    return out;
}

int document::get_text_vertical_align(const string &ref_id)
{
    auto it = document::text_vertical_align_.find(ref_id);
    if (it == document::text_vertical_align_.end())
    {
        return 0;
    }

    return it->second;
}