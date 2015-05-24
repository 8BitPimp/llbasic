// llb_frontend
#include "llb_fail.h"
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

    llb_fail_t fail;
    llb_context_t modules;
    pt_t parse_tree;

    test.init_(modules);

    // skip just to the validator
    if (test.stages_ == test.e_stage_none) {
        return test.validate_(modules, parse_tree, fail);
    }

    for (auto & module : modules.list_) {

        llb_lexer_t lexer(module);
        if (!lexer.run(fail)) {
            return test.expect_ == test.e_expect_lexer_fail;
        }
    }

    // early exit after lexer
    if (test.stages_ == test.e_stage_lexer)
        return test.validate_(modules, parse_tree, fail);


    for (auto & module : modules.list_) {

        llb_parser_t parser(module, parse_tree);
        if (!parser.run(fail)) {
            return test.expect_ == test.e_expect_parse_fail;
        }
    }

    if (test.expect_ != test.e_expect_pass)
        return false;

    return test.validate_(modules, parse_tree, fail);
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
