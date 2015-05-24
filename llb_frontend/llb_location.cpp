#include "llb_location.h"
#include "llb_module.h"

llb_location_t::llb_location_t()
    : line_(0)
    , column_(0)
    , module_()
{
}

llb_location_t::llb_location_t(uint32_t l, uint32_t c)
    : line_(l)
    , column_(c)
{
}

void llb_location_t::set(uint32_t l, uint32_t c, shared_llb_module_t module) {
    line_ = l;
    column_ = c;
    module_ = module;
}

std::string llb_location_t::get_line() const {

    shared_llb_module_t module = module_.lock();
    if (!module)
        return std::string();

    return module->get_line(line_);
}
