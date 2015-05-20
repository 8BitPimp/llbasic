#pragma once
#include <functional>
#include <vector>

// llb_frontend
#include "llb_error.h"
#include "llb_token.h"
#include "llb_lexer.h"
#include "llb_pt.h"
#include "llb_parser.h"

struct test_t {
    
    enum {

        e_stage_none ,
        e_stage_lexer,
        e_stage_pt
    }
    stages_;

    enum {

        e_expect_pass,
        e_expect_lexer_fail,
        e_expect_parse_fail,
    }
    expect_;

    const char * source_;

    std::function<bool(llb_fail_t&, token_list_t&, pt_t&)> validate_;

    const char * category_;
    uint32_t index_;
};

struct test_list_t {

    test_list_t()
        : list_()
    {
    }

    void add(test_t * list) {

        list_.push_back(list);
    }

    std::vector<test_t*> list_;
};

template <typename type_t, int size>
static
uint32_t array_size(type_t(&list)[size]) {
    return size;
}

static
bool floats_equal(float in, float cmp) {
    const float epsilon = .0001f;
    const float dif = (in<cmp) ? cmp-in : in-cmp;
    return dif < epsilon;
}
