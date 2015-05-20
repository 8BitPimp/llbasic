#include "llb_test.h"

static
test_t tests[] = {

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        "identifier \t\n ident",
        [](llb_fail_t&e, token_list_t&tl, pt_t&pt) {
            try{
                tl.pop(tok_identifier);
                tl.pop(tok_eol);
                tl.pop(tok_identifier);
                tl.pop(tok_eol);
                tl.pop(tok_eof);
            }
            catch (llb_fail_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        "1337 ",
        [](llb_fail_t&e, token_list_t&tl, pt_t&pt) {
            try{
                token_t t = tl.pop(tok_lit_integer);
                if (t.get_int() != 1337)
                    return false;
            }
            catch (llb_fail_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        "3.14 ",
        [](llb_fail_t&e, token_list_t&tl, pt_t&pt) {
            try{
                token_t t = tl.pop(tok_lit_float);
                if (!floats_equal( t.get_float(), 3.14f))
                    return false;
            }
            catch (llb_fail_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        "0xcafe12 ",
        [](llb_fail_t&e, token_list_t&tl, pt_t&pt) {
            try{
                token_t t = tl.pop(tok_lit_integer);
                if (t.get_int() != 0xcafe12)
                    return false;
            }
            catch (llb_fail_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        "0b0101 ",
        [](llb_fail_t&e, token_list_t&tl, pt_t&pt) {
            try{
                token_t t = tl.pop(tok_lit_integer);
                if (t.get_int() != 5)
                    return false;
            }
            catch (llb_fail_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        "\"hello world\" ",
        [](llb_fail_t&e, token_list_t&tl, pt_t&pt) {
            try{
                token_t t = tl.pop(tok_lit_string);
                if (t.get_string().empty())
                    return false;
                if (t.get_string() != "hello world")
                    return false;
            }
            catch (llb_fail_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        "1234 0x1234 0b111 99.12 0 0.0 \"greets\" ",
        [](llb_fail_t&e, token_list_t&tl, pt_t&pt) {
            try{
                token_t t0 = tl.pop(tok_lit_integer);
                if (t0.get_int() != 1234) return false;

                token_t t1 = tl.pop(tok_lit_integer);
                if (t1.get_int() != 0x1234) return false;

                token_t t2 = tl.pop(tok_lit_integer);
                if (t2.get_int() != 7) return false;

                token_t t3 = tl.pop(tok_lit_float);
                if (!floats_equal(t3.get_float(), 99.12f)) return false;

                token_t t4 = tl.pop(tok_lit_integer);
                if (t4.get_int() != 0) return false;

                token_t t5 = tl.pop(tok_lit_float);
                if (!floats_equal(t5.get_float(), 0.f)) return false;

                token_t t6 = tl.pop(tok_lit_string);
                if (t6.get_string() != "greets") return false;
            }
            catch (llb_fail_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_lexer_fail,
        "0.12.34 ",
        [](llb_fail_t&e, token_list_t&tl, pt_t&pt) {
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_lexer_fail,
        "0x12.34 ",
        [](llb_fail_t&e, token_list_t&tl, pt_t&pt) {
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_lexer_fail,
        "\"multi line\n string\n",
        [](llb_fail_t&e, token_list_t&tl, pt_t&pt) {
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        "; comment\n token\n",
        [](llb_fail_t&e, token_list_t&tl, pt_t&pt) {

            try {
                tl.pop(tok_eol);
                tl.pop(tok_identifier);
                tl.pop(tok_eol);
                tl.pop(tok_eol);
                if (!tl.at_eof())
                    return false;
            }
            catch (llb_fail_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        " token ; comment\n token\n",
        [](llb_fail_t&e, token_list_t&tl, pt_t&pt) {

            try {
                tl.pop(tok_identifier);
                tl.pop(tok_eol);
                tl.pop(tok_identifier);
                tl.pop(tok_eol);
                tl.pop(tok_eol);
                if (!tl.at_eof())
                    return false;
            }
            catch (llb_fail_t &) {
                return false;
            }
            return true;
        }
    },

    {
        test_t::e_stage_lexer,
            test_t::e_expect_pass,
            "a\n  b\n   0\n   \"hi\"",
            [](llb_fail_t&e, token_list_t&tl, pt_t&pt) {

            try {
                token_t t0 = tl.pop(tok_identifier);
                if (t0.line_ != 0 || t0.column_ != 1) return false;
                tl.pop(tok_eol);

                token_t t1 = tl.pop(tok_identifier);
                if (t1.line_ != 1 || t1.column_ != 4) return false;
                tl.pop(tok_eol);

                token_t t2 = tl.pop(tok_lit_integer);
                if (t2.line_ != 2 || t2.column_ != 5) return false;
                tl.pop(tok_eol);

                token_t t3 = tl.pop(tok_lit_string);
                if (t3.line_ != 3 || t3.column_ != 8) return false;
                tl.pop(tok_eol);
            }
            catch (llb_fail_t &) {
                return false;
            }
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
