#ifndef bd_sketch_analyze_sketch_file
#define bd_sketch_analyze_sketch_file
#include "src/extract/extract.h"
#include "nlohmann/json.hpp"
//#include "valijson_nlohmann_bundled.hpp"
#include <optional>

using std::optional;

class analyze_sketch_file
{
public:
    /**
     * 将输入的 sketch file, 解析为符合 vgg-format 的 json 数据
     * 
     * @param content 待解压的文件内容
     * @param len 待解压的文件长度(byte)
     * @param name 用于标记当前的 sketch 文件, 必须是 utf-8
     * @param json_out 输出的 json 数据
     * 
     * @exception sketch_exception
    */
    static void analyze
    (
        const void* content, 
        const size_t len,
        const char* name,
        nlohmann::json &json_out
    );

public:
    //存储路径的文件夹名称
    static const char* _out_dir_name;

private:
    /**
     * 处理初始化
     * 
     * @exception sketch_exception
    */
    //static void init();

    /**
     * 验证传入的 json 数据是否符合 json-schema 的定义
     * 
     * @param schema json-schema
     * @param data json数据
     * 
     * @note 
     *  1.若没有定义 JSON_VALIDATE 宏, 则直接返回 true
     *  2.当前没有定义 JSON_VALIDATE 宏
     *  3.当前使用的 sketch 是 88.1 其和 sketch-schema 3.0 存在一定的出入
     *  4.sketch-schema 中的一些正则, 无法使用当前的 c++ json-schema 库进行 validate
    static bool json_schema_validate
    (   
        const valijson::Schema &schema, 
        const nlohmann::json &data
    );
    */

    /**
     * 处理 meta 文件
     * 
     * @param sketch_file_info sketch file
     * @param vec_page_file_name 解析出的 page 的文件名
     * 
     * @exception sketch_exception
    */
    static void deal_meta
    (
        const extract::t_extract_result& sketch_file_info,
        std::vector<string> &vec_page_file_name
    );

    /**
     * 处理 page 文件
     * 
     * @param sketch_file_info sketch file
     * @param vec_page_file_name page 的文件名集合
     * @param json_out 输出的 json 信息, 包含了已解析的所有 page
     * 
     * @note 不会对 json_out 进行 clear 操作
     * 
     * @exception sketch_exception
    */
    static void deal_page
    (
        const extract::t_extract_result& sketch_file_info,
        const std::vector<string> &vec_page_file_name,
        nlohmann::json &json_out
    );

// private:
//     static optional<valijson::Schema> _document_schema;
//     static optional<valijson::Schema> _meta_schema;
//     static optional<valijson::Schema> _user_schema;
//     static optional<valijson::Schema> _page_schema;
};

#endif