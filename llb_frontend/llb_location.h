#pragma once

#include <string>
#include <stdint.h>

#include "llb_forward.h"

struct llb_location_t {

    llb_location_t();

    llb_location_t(uint32_t l, uint32_t c);

    void set(uint32_t l, uint32_t c, shared_llb_module_t module_);
    
    std::string get_line() const;

    uint32_t line_;
    uint32_t column_;
    weak_llb_module_t module_;
};
