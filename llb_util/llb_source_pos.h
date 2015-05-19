#pragma once

#include <stdint.h>

struct source_pos_t {

    source_pos_t(uint32_t l, uint32_t c)
        : line_(l)
        , column_(c)
    {
    }

    uint32_t line_, column_;
};
