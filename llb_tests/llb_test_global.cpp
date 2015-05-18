#include "llb_test.h"
#include "llb_pt_util.h"

static
test_t tests[] = {

{
    test_t::e_stage_pt,
    test_t::e_expect_pass,
    "global x.int",
    [](exception_t&e, token_list_t&tl, pt_t&pt) { return true; }
}

};

void glob_test_global(test_list_t&list) {

    static const char category[] = "global";

    for (uint32_t i = 0; i < array_size(tests); ++i) {
        tests[i].category_ = category;
        tests[i].index_ = i;
        list.add(tests + i);
    }
}
