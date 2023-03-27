#ifndef bd_sketch_artboard
#define bd_sketch_artboard
#include "src/sketch_object/objects/abstract_root.h"

class artboard : public abstract_root
{
public:
    artboard();

public:    
    virtual void change(const nlohmann::json &sketch, nlohmann::json &vgg);

private:
    static t_child child_;
};

#endif