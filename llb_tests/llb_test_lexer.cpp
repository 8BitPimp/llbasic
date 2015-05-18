#include "llb_test.h"

static
test_t tests[] = {

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        "identifier \t\n ident",
        [](exception_t&e, token_list_t&tl, pt_t&pt) {
            try{
                tl.pop(tok_identifier);
                tl.pop(tok_eol);
                tl.pop(tok_identifier);
                tl.pop(tok_eol);
                tl.pop(tok_eof);
            }
            catch (exception_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        "1337 ",
        [](exception_t&e, token_list_t&tl, pt_t&pt) {
            try{
                token_t t = tl.pop(tok_lit_integer);
                if (t.value_.int_ != 1337)
                    return false;
            }
            catch (exception_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        "3.14 ",
        [](exception_t&e, token_list_t&tl, pt_t&pt) {
            try{
                token_t t = tl.pop(tok_lit_float);
                if (!floats_equal( t.value_.float_, 3.14f))
                    return false;
            }
            catch (exception_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        "0xcafe12 ",
        [](exception_t&e, token_list_t&tl, pt_t&pt) {
            try{
                token_t t = tl.pop(tok_lit_integer);
                if (t.value_.int_ != 0xcafe12)
                    return false;
            }
            catch (exception_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        "0b0101 ",
        [](exception_t&e, token_list_t&tl, pt_t&pt) {
            try{
                token_t t = tl.pop(tok_lit_integer);
                if (t.value_.int_ != 5)
                    return false;
            }
            catch (exception_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        "\"hello world\" ",
        [](exception_t&e, token_list_t&tl, pt_t&pt) {
            try{
                token_t t = tl.pop(tok_lit_string);
                if (t.value_.string_.empty())
                    return false;
                if (t.value_.string_ != "hello world")
                    return false;
            }
            catch (exception_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        "1234 0x1234 0b111 99.12 0 0.0 \"greets\" ",
        [](exception_t&e, token_list_t&tl, pt_t&pt) {
            try{
                token_t t0 = tl.pop(tok_lit_integer);
                if (t0.value_.int_ != 1234) return false;

                token_t t1 = tl.pop(tok_lit_integer);
                if (t1.value_.int_ != 0x1234) return false;

                token_t t2 = tl.pop(tok_lit_integer);
                if (t2.value_.int_ != 7) return false;

                token_t t3 = tl.pop(tok_lit_float);
                if (!floats_equal(t3.value_.float_, 99.12f)) return false;

                token_t t4 = tl.pop(tok_lit_integer);
                if (t4.value_.int_ != 0) return false;

                token_t t5 = tl.pop(tok_lit_float);
                if (!floats_equal(t5.value_.float_, 0.f)) return false;

                token_t t6 = tl.pop(tok_lit_string);
                if (t6.value_.string_ != "greets") return false;
                
            }
            catch (exception_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_lexer_fail,
        "0.12.34 ",
        [](exception_t&e, token_list_t&tl, pt_t&pt) {
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_lexer_fail,
        "0x12.34 ",
        [](exception_t&e, token_list_t&tl, pt_t&pt) {
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_lexer_fail,
        "\"multi line\n string\n",
        [](exception_t&e, token_list_t&tl, pt_t&pt) {
            return true;
        }
    }

};

void glob_test_lexer(test_list_t&list) {

    static const char category[] = "lexer";

    for (uint32_t i = 0; i < array_size(tests); ++i) {
        tests[i].category_ = category;
        tests[i].index_ = i;
        list.add(tests + i);
    }
}
