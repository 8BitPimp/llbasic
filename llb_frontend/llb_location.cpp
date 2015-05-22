#include "llb_location.h"
#include "llb_module.h"

location_t::location_t()
    : line_(0)
    , column_(0)
    , module_()
{
}

location_t::location_t(uint32_t l, uint32_t c)
    : line_(l)
    , column_(c)
{
}

void location_t::set(uint32_t l, uint32_t c, shared_module_t module) {
    line_ = l;
    column_ = c;
    module_ = module;
}

std::string location_t::get_line() const {

    shared_module_t module = module_.lock();
    if (!module)
        return std::string();

    return module->get_line(line_);
}
