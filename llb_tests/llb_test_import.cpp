#include "llb_test.h"

static
test_t tests[] = {

// check the import keyword is understood
{
    test_t::e_stage_pt,
    test_t::e_expect_pass,
    "import \"test\"",
    [](llb_fail_t&e, token_list_t&tl, pt_t&pt) { return true; }
}

};

void glob_test_import(test_list_t&list) {

    static const char category[] = "import";

    for (uint32_t i = 0; i < array_size(tests); ++i) {
        tests[i].category_ = category;
        tests[i].index_ = i;
        list.add(tests + i);
    }
}
