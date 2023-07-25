#include <iostream>
#include <cassert>
#include <fstream>
#include "../src/extract/extract.h"
#include "../src/analyze_sketch_file/analyze_sketch_file.h"
#include "../src/sketch_object/check.hpp"
#include <regex>
#include <filesystem>
#include "boost/algorithm/string.hpp"
#include <algorithm>
#include <any>

//only for test
int main(int, char **)
{
    if (true)
    {
        std::filesystem::remove_all("./del");
       
        //const char *p_file_name = "Y:\\Desktop\\szn_sketch_test\\88.1.sketch";
        const char *p_file_name = "Y:\\Desktop\\szn_sketch_test\\out.sketch";
        //const char *p_file_name = "./1.sketch";

        zip_extract(p_file_name, "./del", nullptr, nullptr);

        auto size = std::filesystem::file_size(p_file_name);
        std::vector<char> file_buf(size);
        std::ifstream ifs(p_file_name, std::ios_base::binary);
        ifs.read(file_buf.data(), size);
        assert(ifs.gcount() == size);

        nlohmann::json json_out;
        map<string, vector<char>> file_info;
        bool re = analyze_sketch_file::analyze(file_buf.data(), size, "hello-sketch", json_out, file_info);
        assert(re);

        auto str = json_out.dump();

        std::ofstream ofs("out.json");
        ofs.write(str.c_str(), str.size());
        ofs.close();

        std::filesystem::remove_all(analyze_sketch_file::_image_dir_name);
        std::filesystem::create_directories(analyze_sketch_file::_image_dir_name);
        for (auto &item : file_info)
        {
            std::ofstream ofs(item.first, std::ios_base::binary);
            ofs.write(item.second.data(), item.second.size());
        }

        if (!check::ins_.get_error().empty())
        {
            auto tmp = check::ins_.get_error();
            assert(false);
        }
    }
    
    else 
    {
        char *buf = new char[1024 * 1024 * 500];
        std::filesystem::remove_all("./many");

        int count = 0;

        auto it = std::filesystem::recursive_directory_iterator
            ("D:\\work\\project\\github_project\\vgg_sketch_parser\\resource\\sketch-samples\\");
        
        for (std::filesystem::recursive_directory_iterator end; it != end; ++it)
        {
            if (it->is_directory())
            {
                continue;
            }

            if (boost::contains(it->path().string(), "not ok"))
            {
                continue;
            }

            auto name = "./many/" + std::to_string(++count) + "_" + it->path().stem().string();
            
            std::filesystem::create_directories(name);

            std::ifstream ifs(it->path().string(), std::ios_base::binary);
            ifs.read(buf, 1024 * 1024 * 500);

            map<string, vector<char>> file_info;
            nlohmann::json json_out;

            try 
            {
                analyze_sketch_file::analyze(buf, ifs.gcount(), "hello-sketch", json_out, file_info);
            }
            catch (sketch_exception &e)
            {
                string tt = e.get();
                std::cout << it->path().string() << std::endl;
                std::cout << tt << std::endl;
            }

            if (!check::ins_.get_error().empty())
            {
                auto str = it->path().filename().string();
                auto tmp = check::ins_.get_error();
                //assert(false);
                printf("");
            }

            auto str = json_out.dump();

            std::ofstream ofs(name + "/out.json");
            ofs.write(str.c_str(), str.size());
            ofs.close();
        }

        std::cout << count << std::endl;
    }

    std::cout << "finish" << std::endl;
}