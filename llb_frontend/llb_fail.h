#pragma once

#include <string>
#include <stdint.h>

#include "llb_forward.h"
#include "llb_location.h"

struct llb_fail_t {

    llb_fail_t();

    llb_fail_t(
        const std::string & str, 
        llb_token_t & tok);

    llb_fail_t(
        const std::string & str, 
        llb_location_t & pos);

    llb_fail_t(
        const std::string & str,
        uint32_t line,
        uint32_t column);

    std::string msg_;
    llb_location_t location_;
};
