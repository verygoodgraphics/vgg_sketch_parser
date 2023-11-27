#include <iostream>
#include <fstream>
#include <memory>
#include <filesystem>
#include "../src/analyze_sketch_file/analyze_sketch_file.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("need two param: first param is input sketch filepath, second param is output directory(must already exists)\n");
        return -1;
    }

    std::string input_file = argv[1];
    std::string output_dir = argv[2];

    if (!std::filesystem::exists(input_file))
    {
        printf("error: input file not exists\n");
        return -1;
    }

    if (!std::filesystem::is_directory(output_dir))
    {
        printf("error: invalid output directory\n");
        return -1;
    }

    auto file_size = std::filesystem::file_size(input_file);
    if (file_size > 1024 * 1024 * 1024)
    {
        printf("error: input file too big\n");
        return -1;
    }
    
    std::unique_ptr<char []> buf(new char[file_size]);

    std::ifstream ifs;
    ifs.open(input_file, std::ios_base::binary);
    if (!ifs.is_open())
    {
        printf("error: fail to open input file\n");
        return -1;
    }
    ifs.read(buf.get(), file_size);
    if (ifs.gcount() != file_size)
    {
        printf("error: fail to read input file\n");
        return -1;
    }
    ifs.close();

    nlohmann::json json_out;
    map<string, vector<char>> file_info;
    try 
    {
        analyze_sketch_file::analyze(buf.get(), file_size, "hello-sketch", json_out, file_info);
    }
    catch(sketch_exception &e)
    {
        printf("parser-error: %s\n", e.get());
        return -1;
    }    

    std::string output_json_name = std::filesystem::path(output_dir).append("design.json").string();
    std::string output_img_dir_name = std::filesystem::path(output_dir).append("resources").string();

    std::ofstream ofs(output_json_name);
    if (ofs.is_open())
    {
        auto str = json_out.dump();
        ofs.write(str.c_str(), str.size());
        ofs.close();
    }
    else 
    {
        printf("error: failed to open design.json\n");
        return -1;        
    }

    try 
    {
        std::filesystem::remove_all(output_img_dir_name);
        std::filesystem::create_directories(output_img_dir_name);
    }
    catch(...)
    {

    }

    for (auto &item : file_info)
    {
        auto str = std::filesystem::path(output_img_dir_name).append(
            std::filesystem::path(item.first).filename().string()).string();

        std::ofstream ofs(str, std::ios_base::binary);
        if (!ofs.is_open())
        {
            printf("error: failed to create image\n");
            return -1;
        }
        ofs.write(item.second.data(), item.second.size());
    }

    printf("success\n");
    return 0;
}