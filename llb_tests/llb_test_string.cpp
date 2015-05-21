#include "llb_string.h"
#include "llb_test.h"

static
test_t tests[] = {

    {
        test_t::e_stage_pt,
        test_t::e_expect_pass,
        "",
        [](llb_fail_t&e, token_list_t&tl, pt_t&pt) { 

            std::string str = llb_string_t::format("%0 world %1", { "hello", 1234 });
            return str == "hello world 1234";
        }
    },

    {
        test_t::e_stage_pt,
        test_t::e_expect_pass,
        "",
        [](llb_fail_t&e, token_list_t&tl, pt_t&pt) {

            std::string str = llb_string_t::format("%0 %0", { "jj", 1234 });
            return str == "jj jj";
        }
    },

    {
        test_t::e_stage_pt,
        test_t::e_expect_pass,
        "",
        [](llb_fail_t&e, token_list_t&tl, pt_t&pt) {

            std::string str = llb_string_t::format("%0", { 0.4f });
            return str == "0.4";
        }
    },

};

void glob_test_string(test_list_t&list) {

    static const char category[] = "string";

    for (uint32_t i = 0; i < array_size(tests); ++i) {
        tests[i].category_ = category;
        tests[i].index_ = i;
        list.add(tests + i);
    }
}
