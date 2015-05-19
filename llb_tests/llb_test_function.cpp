#include "llb_test.h"
#include "llb_pt_util.h"

static
test_t tests[] = {

// basic function
{
test_t::e_stage_pt,
test_t::e_expect_pass,
R"(
function name:none()
end
)",
[](llb_fail_t&e, token_list_t&tl, pt_t&pt) {
        if (!pt.top()->is_a<pt_module_t>())
            return false;
        pt_module_t * mod = pt.top()->upcast<pt_module_t>();
        if (mod == nullptr)
            return false;
        if (mod->functions_.size() != 1)
            return false;
        return true;
    }
},

// single argument
{
test_t::e_stage_pt,
test_t::e_expect_pass,
R"(
function name:int( a1:int )
end
)",
[](llb_fail_t&e, token_list_t&tl, pt_t&pt) { return true; }
},

// multiple arguments
{
test_t::e_stage_pt,
test_t::e_expect_pass,
R"(
function name:int( a1:int, a2:int )
end
)",
[](llb_fail_t&e, token_list_t&tl, pt_t&pt) { return true; }
},

// argument name collision
{
test_t::e_stage_pt,
test_t::e_expect_parse_fail,
R"(
function name:string( a1:int, a1:int )
end
)",
[](llb_fail_t&e, token_list_t&tl, pt_t&pt) { return true; }
},

};

void glob_test_function(test_list_t&list) {

    static const char category[] = "function";

    for (uint32_t i = 0; i < array_size(tests); ++i) {
        tests[i].category_ = category;
        tests[i].index_ = i;
        list.add(tests + i);
    }
}
