# vgg_sketch_parser

* This project focuses on converting sketch files to json data in vgg-format.
* Supported version of sketch：Sketch 55.2 ~ latest.



## How to use

```c++
#include <iostream>
#include <cassert>
#include <fstream>
#include <filesystem>
#include "src/analyze_sketch_file/analyze_sketch_file.h"

int main(int, char **)
{
    const char *file_name = "now_file.sketch";
    auto size = std::filesystem::file_size(file_name);
    std::vector<char> file_buf(size);

    std::ifstream ifs(file_name, std::ios_base::binary);
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
```



## Related projects

* vgg-format：To be completed
* [vgg-contrib](https://github.com/verygoodgraphics/vgg_contrib)