#ifndef bd_sketch_extract
#define bd_sketch_extract
#include <vector>
#include <map>
#include "./src/basic/define.h"
#include "zip.h"

using std::vector;
using std::map;

//处理 sketch 文件的解压
class extract
{
public:
    typedef map<string/*文件名*/, vector<char>/*文件内容*/> t_extract_result;

public:
    /**
     * 设置待解压的 sketch 文件
     * 
     * @param content 待解压的文件内容
     * @param len 待解压的文件长度(byte)
     * @param result 解压结果
     * 
     * @exception sketch_exception
    */
    static void set_file(const char* content, const size_t len, t_extract_result &result);

public:
    static const string _document_file_name;
    static const string _meta_file_name;
    static const string _user_file_name;
    static const string _pages_dir_name;
    static const string _images_dir_name;
};

#endif