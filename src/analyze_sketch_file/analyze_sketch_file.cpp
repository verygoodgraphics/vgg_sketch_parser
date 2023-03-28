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

#include <fstream>
#include "boost/format.hpp"
#include "./analyze_sketch_file.h"
#include "src/sketch_object/objects/page.h"
#include "boost/algorithm/string.hpp"

const char* analyze_sketch_file::_out_dir_name = "out";
// optional<valijson::Schema> analyze_sketch_file::_document_schema;
// optional<valijson::Schema> analyze_sketch_file::_meta_schema;
// optional<valijson::Schema> analyze_sketch_file::_user_schema;
// optional<valijson::Schema> analyze_sketch_file::_page_schema;

/*
void analyze_sketch_file::init()
{
    if (analyze_sketch_file::_document_schema)
    {
        return;
    }

    auto load_schema = [](const char* path, optional<valijson::Schema> &schema)
    {
        assert(!schema);
        schema.emplace();

        do
        {
            nlohmann::json value;
            if (!valijson::utils::loadDocument(path, value))
            {
                break;
            }

            valijson::SchemaParser parser;
            valijson::adapters::NlohmannJsonAdapter schemaDocumentAdapter(value);
            try
            {
                parser.populateSchema(schemaDocumentAdapter, *schema);
            }
            catch (...)
            {
                break;
            }

            return;
        } while (false);

        std::string str = (boost::format("%1%: load fail") % path).str();
        throw sketch_exception(str.c_str());
    };

    load_schema("./dist/document.schema.json", analyze_sketch_file::_document_schema);
    load_schema("./dist/meta.schema.json", analyze_sketch_file::_meta_schema);
    load_schema("./dist/user.schema.json", analyze_sketch_file::_user_schema);
    load_schema("./dist/page.schema.json", analyze_sketch_file::_page_schema);
}

bool analyze_sketch_file::json_schema_validate(const valijson::Schema &schema, const nlohmann::json &data)
{
#ifdef JSON_VALIDATE
    try 
    {
        valijson::Validator valid;
        valijson::adapters::NlohmannJsonAdapter targetDocumentAdapter(data);
        return valid.validate(schema, targetDocumentAdapter, nullptr);
    }
    catch(...)
    {
        return false;
    }

#else 
    return true;
#endif
}
*/

void analyze_sketch_file::deal_meta(const extract::t_extract_result& sketch_file_info, 
    std::vector<string> &vec_page_file_name)
{
    vec_page_file_name.clear();

    auto it_meta = sketch_file_info.find(extract::_meta_file_name);
    if (it_meta == sketch_file_info.end())
    {
        throw sketch_exception("fail to get meta file");
    }

    nlohmann::json meta_json = nlohmann::json::parse(it_meta->second.begin(), it_meta->second.end());

    //validate
    // assert(analyze_sketch_file::_meta_schema);
    // if (!analyze_sketch_file::json_schema_validate(*analyze_sketch_file::_meta_schema, meta_json))
    // {
    //     std::string str = (boost::format("%1% fail to validate") % it_meta->first).str();
    //     throw sketch_exception(str.c_str());
    // }

    //119: Sketch 55.2 - 57.1
    // if (get_json_value(meta_json, "version", 0) < 119)
    // {
    //     throw sketch_exception("The version of sketch for a given file is too low, only 55.2 and above are supported");
    // }

    try 
    {
        auto &page_info = meta_json.at("pagesAndArtboards");
        for (auto &item : page_info.items())
        {
            vec_page_file_name.emplace_back((boost::format("%1%/%2%.json") % extract::_pages_dir_name % item.key()).str());
        }
    }
    catch(...)
    {
        string str = (boost::format("fail to analyze %1%") % extract::_meta_file_name).str();
        throw sketch_exception(str.c_str());
    }
}

void analyze_sketch_file::deal_page(const extract::t_extract_result &sketch_file_info,
    const std::vector<string> &vec_page_file_name, nlohmann::json &json_out)
{
    assert(!json_out.empty());
    page page_obj;

    for (auto &file_name : vec_page_file_name)
    {
        auto it_page = sketch_file_info.find(file_name);
        if (it_page == sketch_file_info.end())
        {
            string str = (boost::format("in extract files, can not find %1%") % file_name).str();
            throw sketch_exception(str.c_str());
        }

        nlohmann::json page_json = nlohmann::json::parse(it_page->second.begin(), it_page->second.end());
        
        //validate
        // assert(analyze_sketch_file::_page_schema);
        // if (!analyze_sketch_file::json_schema_validate(*analyze_sketch_file::_page_schema, page_json))
        // {
        //     std::string str = (boost::format("%1% fail to validate") % it_page->first).str();
        //     throw sketch_exception(str.c_str());
        // }

        page_obj.change(page_json, json_out);
    }
}

void analyze_sketch_file::analyze(const void* content, const size_t len,
    const char* name, nlohmann::json &json_out)
{
    //analyze_sketch_file::init();

    extract::t_extract_result sketch_file_info;
    extract::set_file(static_cast<const char*>(content), len, sketch_file_info);

    //将图片进行存储
    do
    {
        try 
        {
            std::filesystem::create_directory(analyze_sketch_file::_out_dir_name);
        }
        catch(...)
        {
        }

        for (auto &item : sketch_file_info)
        {
            if (boost::starts_with(item.first, extract::_images_dir_name))
            {
                string str = item.first;
                std::filesystem::path m_path(str);
                str = analyze_sketch_file::_out_dir_name + string("/") + m_path.filename().string();

                std::ofstream ofs(str, std::ios_base::binary);
                if (!ofs.is_open())
                {
                    throw sketch_exception((boost::format("fail to create %1%") % str).str().c_str());
                }

                ofs.write(item.second.data(), item.second.size());
            }
        }

    } while (false);

    json_out.clear();
    json_out["fileType"] = 1;
    json_out["fileName"] = string(name);
    json_out["artboard"] = nlohmann::json::array();
    json_out["symbolMaster"] = nlohmann::json::array();

    if (sketch_file_info.empty())
    {
        assert(false);
        return;
    }

    vector<string> vec_page_file_path;
    analyze_sketch_file::deal_meta(sketch_file_info, vec_page_file_path);
    analyze_sketch_file::deal_page(sketch_file_info, vec_page_file_path, json_out);
}