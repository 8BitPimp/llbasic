#include "llb_test.h"

static
test_t tests[] = {

    {
        SOURCE_LOC,
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        [](module_list_t&modules) {
            modules.new_module("test", "identifier \t\n ident");
        },        
        [](module_list_t&modules, pt_t&pt, llb_fail_t&e) {
            auto & tl = *modules.list_[0]->tokens_;
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
        SOURCE_LOC,
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        [](module_list_t&modules) {
            modules.new_module("test", "1337");
        },
        [](module_list_t&modules, pt_t&pt, llb_fail_t&e) {
            auto & tl = *modules.list_[0]->tokens_;
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
        SOURCE_LOC,
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        [](module_list_t&modules) {
            modules.new_module("test", "3.14 ");
        },
        [](module_list_t&modules, pt_t&pt, llb_fail_t&e) {
            auto & tl = *modules.list_[0]->tokens_;
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
        SOURCE_LOC,
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        [](module_list_t&modules) {
            modules.new_module("test", "0xcafe12 ");
        },
        [](module_list_t&modules, pt_t&pt, llb_fail_t&e) {
            auto & tl = *modules.list_[0]->tokens_;
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
        SOURCE_LOC,
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        [](module_list_t&modules) {
            modules.new_module("test", "0b0101 ");
        },
        [](module_list_t&modules, pt_t&pt, llb_fail_t&e) {
            auto & tl = *modules.list_[0]->tokens_;
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
        SOURCE_LOC,
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        [](module_list_t&modules) {
            modules.new_module("test", "\"hello world\" ");
        },
        [](module_list_t&modules, pt_t&pt, llb_fail_t&e) {
            auto & tl = *modules.list_[0]->tokens_;
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
        SOURCE_LOC,
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        [](module_list_t&modules) {
            modules.new_module("test", "1234 0x1234 0b111 99.12 0 0.0 \"greets\" ");
        },
        [](module_list_t&modules, pt_t&pt, llb_fail_t&e) {
            auto & tl = *modules.list_[0]->tokens_;
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
        SOURCE_LOC,
        test_t::e_stage_lexer,
        test_t::e_expect_lexer_fail,
        [](module_list_t&modules) {
            modules.new_module("test", "0.12.34 ");
        },
        [](module_list_t&modules, pt_t&pt, llb_fail_t&e) {
            return true;
        }
    },

    {
        SOURCE_LOC,
        test_t::e_stage_lexer,
        test_t::e_expect_lexer_fail,
        [](module_list_t&modules) {
            modules.new_module("test", "0x12.34 ");
        },
        [](module_list_t&modules, pt_t&pt, llb_fail_t&e) {
            return true;
        }
    },

    {
        SOURCE_LOC,
        test_t::e_stage_lexer,
        test_t::e_expect_lexer_fail,
        [](module_list_t&modules) {
            modules.new_module("test", "\"multi line\n string\n");
        },
        [](module_list_t&modules, pt_t&pt, llb_fail_t&e) {
            return true;
        }
    },

    {
        SOURCE_LOC,
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        [](module_list_t&modules) {
            modules.new_module("test", "; comment\n token\n");
        },
        [](module_list_t&modules, pt_t&pt, llb_fail_t&e) {
            auto & tl = *modules.list_[0]->tokens_;
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
        SOURCE_LOC,
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        [](module_list_t&modules) {
            modules.new_module("test", " token ; comment\n token\n");
        },
        [](module_list_t&modules, pt_t&pt, llb_fail_t&e) {
            auto & tl = *modules.list_[0]->tokens_;
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
        SOURCE_LOC,
        test_t::e_stage_lexer,
        test_t::e_expect_pass,
        [](module_list_t&modules) {
            modules.new_module("test", "a\n  b\n   0\n   \"hi\"");
        },
        [](module_list_t&modules, pt_t&pt, llb_fail_t&e) {
            auto & tl = *modules.list_[0]->tokens_;
            try {
                token_t t0 = tl.pop(tok_identifier);
                if (t0.pos_.line_ != 0 || t0.pos_.column_ != 1) return false;
                tl.pop(tok_eol);

                token_t t1 = tl.pop(tok_identifier);
                if (t1.pos_.line_ != 1 || t1.pos_.column_ != 4) return false;
                tl.pop(tok_eol);

                token_t t2 = tl.pop(tok_lit_integer);
                if (t2.pos_.line_ != 2 || t2.pos_.column_ != 5) return false;
                tl.pop(tok_eol);

                token_t t3 = tl.pop(tok_lit_string);
                if (t3.pos_.line_ != 3 || t3.pos_.column_ != 8) return false;
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
