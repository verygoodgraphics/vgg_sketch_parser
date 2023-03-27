#include "./extract.h"

const string extract::_document_file_name = "document.json";
const string extract::_meta_file_name = "meta.json";
const string extract::_user_file_name = "user.json";
const string extract::_pages_dir_name = "pages";
const string extract::_images_dir_name = "images";

void extract::set_file(const char* content, const size_t len, t_extract_result &result)
{
    result.clear();

    auto clean = [](zip_t *zip)
    {
        if (zip)
        {
            zip_close(zip);
        }
    };

    unique_ptr<zip_t, decltype(clean)> zip(zip_stream_open(content, len, 0, 'r'), clean);
    auto p_zip = zip.get();

    if (!p_zip)
    {
        throw sketch_exception("zip fail to open stream");
    }

    for (size_t i = 0; i < -1; ++i)
    {
        if (zip_entry_openbyindex(p_zip, i))
        {
            break;
        }

        const char* p_name = zip_entry_name(p_zip);
        if (!p_name)
        {
            throw sketch_exception("zip fail to get entry name");
        }

        string str = p_name;
        if (zip_entry_open(p_zip, str.c_str()))
        {
            throw sketch_exception("zip fail to open entry");
        }

        result.emplace(std::make_pair(str, vector<char>()));

        auto n_size = zip_entry_size(p_zip);
        if (!n_size)
        {
            continue;
        }
        
        auto &vec = result.at(str);
        vec.resize(n_size);

        auto n_read_len = zip_entry_noallocread(p_zip, vec.data(), n_size);
        if (n_read_len != n_size)
        {
            throw sketch_exception("zip entry extract fail");
        }
    }

    assert(result.find(extract::_document_file_name) != result.end());
    assert(result.find(extract::_meta_file_name) != result.end());
    assert(result.find(extract::_user_file_name) != result.end());
}