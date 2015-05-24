#include "llb_test.h"

static
test_t tests[] = {

// basic function
{
    SOURCE_LOC,
    test_t::e_stage_pt,
    test_t::e_expect_pass,
    [](llb_context_t&modules) {
        const char * source = 
        R"(
        function name:none()
        end
        )";
        modules.new_module("test", source);
    },
    [](llb_context_t&modules, pt_t&pt, llb_fail_t&e) {
        return true;
    }
},

// single argument
{
    SOURCE_LOC,
    test_t::e_stage_pt,
    test_t::e_expect_pass,
    [](llb_context_t&modules) {
        const char * source =
        R"(
        function name:none( a1:int )
        end
        )";
        modules.new_module("test", source);
    },
    [](llb_context_t&modules, pt_t&pt, llb_fail_t&e) {
        return true;
    }
},

// multiple arguments
{
    SOURCE_LOC,
    test_t::e_stage_pt,
    test_t::e_expect_pass,
    [](llb_context_t&modules) {
        const char * source =
        R"(
        function name:none( a1:int, a2:int )
        end
        )";
        modules.new_module("test", source);
    },
    [](llb_context_t&modules, pt_t&pt, llb_fail_t&e) {
        return true;
    }
},

// argument name collision
{
    SOURCE_LOC,
    test_t::e_stage_pt,
    test_t::e_expect_pass,
    [](llb_context_t&modules) {
        const char * source =
        R"(
        function name:none( a1:int, a1:int )
        end
        )";
        modules.new_module("test", source);
    },
    [](llb_context_t&modules, pt_t&pt, llb_fail_t&e) {
        return true;
    }
},

// argument name collision
{
    SOURCE_LOC,
    test_t::e_stage_pt,
    test_t::e_expect_pass,
    [](llb_context_t&modules) {
        const char * source =
        R"(
        external function func1:none( a:int, b:int )
        external function func2:none( c:int )
        external function func3:none( c:int )
        )";
        modules.new_module("test", source);
    },
    [](llb_context_t&modules, pt_t&pt, llb_fail_t&e) {
        
        return true;
    }
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
