// llb_frontend
#include "llb_error.h"
#include "llb_token.h"
#include "llb_lexer.h"
#include "llb_pt.h"
#include "llb_parser.h"

// llb_passes
#include "llb_pass_printer.h"

// llb_tests
#include "llb_test.h"

extern void glob_test_global  (test_list_t&list);
extern void glob_test_lexer   (test_list_t&list);
extern void glob_test_import  (test_list_t&list);
extern void glob_test_function(test_list_t&list);
extern void glob_test_string  (test_list_t&list);

bool run_test(test_t & test) {

    llb_fail_t except;
    token_list_t tokens;
    pt_t parse_tree;

    // skip just to the validator
    if (test.stages_ == test.e_stage_none) {
        return test.validate_(except, tokens, parse_tree);
    }

    lexer_t lexer(test.source_, -1, tokens);
    if (!lexer.run(except)) {
        return test.expect_ == test.e_expect_lexer_fail;
    }

    // early exit after lexer
    if (test.stages_ == test.e_stage_lexer)
        return test.validate_(except, tokens, parse_tree);

    parser_t parser(tokens, parse_tree);
    if (!parser.run(except)) {
        return test.expect_ == test.e_expect_parse_fail;
    }

    if (test.expect_ != test.e_expect_pass)
        return false;

    return test.validate_(except, tokens, parse_tree);
}

int main() {

    test_list_t list;

    glob_test_lexer   (list);
    glob_test_global  (list);
    glob_test_import  (list);
    glob_test_function(list);
    glob_test_string  (list);

    uint32_t ran = 0, fail = 0;

    const char * category = nullptr;

    for (auto & glob : list.list_) {

        assert(glob->source_);

        if (category != glob->category_) {
            category = glob->category_;
            printf("\n%s:\n    ", category);
        }
        
        if (!run_test(*glob)) {
            putchar('X');
            fail++;
        }
        else {
            putchar('.');
        }
        ran++;
        
    }

    printf("\n\n%d of %d failed", fail, ran);

    getchar();
    return 0;
}
