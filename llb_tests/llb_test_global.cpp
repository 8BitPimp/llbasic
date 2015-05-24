#include "llb_test.h"
#include "llb_pt_util.h"

static
test_t tests[] = {

{
    SOURCE_LOC,
    test_t::e_stage_pt,
    test_t::e_expect_pass,
    [](llb_context_t&modules) {
        modules.new_module("test", "global x:int");
    },
    [](llb_context_t&modules, pt_t&pt, llb_fail_t&e) {
        return true;
    }
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
