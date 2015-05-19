#pragma once

#include <string>
#include <stdint.h>

struct llb_fail_t {

    llb_fail_t()
        : msg_()
        , line_(0)
        , column_(0)
    {
    }

    llb_fail_t(const std::string & str,
               uint32_t line,
               uint32_t column)
        : msg_(str)
        , line_(line)
        , column_(column)
    {
    }

    std::string msg_;
    uint32_t line_;
    uint32_t column_;
};
