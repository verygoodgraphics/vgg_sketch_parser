#ifndef bd_sketch_define
#define bd_sketch_define
#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <map>

using std::string;
using std::vector;
using std::unique_ptr;
using std::map;

//异常类
class sketch_exception
{
public:
    sketch_exception(const char* p_buf_c) : str_(p_buf_c) {}

public:
    const char* get() { return this->str_.c_str(); }

private:
    //存储了异常信息
    string str_;    
};

#ifndef NDEBUG
#define SZN_DEBUG
#endif

#endif