#include <iostream>
#include <cassert>
#include <fstream>
#include "./src/extract/extract.h"
#include "src/analyze_sketch_file/analyze_sketch_file.h"
#include <regex>
#include <filesystem>

//only for test
int main(int, char **)
{
    std::filesystem::remove_all("./del");
    //const char* p_file_name = "X:/sketch-samples/realcases/c38cfdf314e8408b.sketch";
    //const char* p_file_name = "X:/sketch-samples/blurs.sketch";
    //const char* p_file_name = "Y:/Desktop/57.1.sketch";
    const char* p_file_name = "Y:/Desktop/88.1.sketch";
    //const char* p_file_name = "Y:/Desktop/c38cfdf314e8408b.sketch";
    zip_extract(p_file_name, "./del", nullptr, nullptr);

    auto size = std::filesystem::file_size(p_file_name);
    std::vector<char> file_buf(size);
    std::ifstream ifs(p_file_name, std::ios_base::binary);
    ifs.read(file_buf.data(), size);
    assert(ifs.gcount() == size);

    nlohmann::json json_out;
    analyze_sketch_file::analyze(file_buf.data(), size, "hello-sketch", json_out);

    auto str = json_out.dump();

    std::ofstream ofs("out.json");
    ofs.write(str.c_str(), str.size());
    ofs.close();

    std::cout << "finish" << std::endl;
}