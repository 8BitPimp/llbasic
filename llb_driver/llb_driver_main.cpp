// llb_util
#include "llb_string.h"

// llb_frontend
#include "llb_fail.h"
#include "llb_module.h"
#include "llb_token.h"
#include "llb_lexer.h"
#include "llb_pt.h"
#include "llb_parser.h"
#include "llb_backend_cpp.h"

// llb_passes
#include "llb_pass_symbolic_link.h"
#include "llb_pass_printer.h"

const char module1[] = R"(

global x:int = 3 + 4

function main:int( x:int, y:int )
    local x:int = 3
    x = func( l + y ) * 3 + 2.1 * 4
    return x + 3
end

global i2:int=3-2, i3:float, i4:float=12*3

function a:int( )
    local x:int=3, x:float=5
end
)";

const char module2[] = R"(

function main:int()
    return 0
end
)";

void on_fail( const llb_fail_t & fail ) {

    shared_module_t module = fail.location_.module_.lock();

    std::string module_name;
    if (module)
        module_name = module->name_;

    std::string msg = llb_string_t::format(
        "error:\n . %0\n . %3 %1,%2\n", {
        fail.msg_.c_str(),
        fail.location_.line_,
        fail.location_.column_,
        module_name });
    printf("%s", msg.c_str());

    if (module) {

        std::string line = fail.location_.get_line();
        if (!line.empty()) {
            printf(" . \"%s\"\n", line.c_str());

            for (uint32_t i = 0; i <= fail.location_.column_+3; ++i)
                putchar(' ');
            putchar('^');
            printf("\n");
        }
    }

    getchar();
    exit(0);
}

int main( ) {

    llb_fail_t fail;
    pt_t parse_tree;

    module_list_t modules;
    modules.new_module("module1", module1);
    modules.new_module("module2", module2);

    // loop over all modules
    for (auto & module : modules.list_) {

        // perform lexical analysis
        lexer_t lexer(module);
        if (!lexer.run(fail)) {
            on_fail(fail);
            return -1;
        }

        // construct a parse tree
        parser_t parser(module, parse_tree);
        if (!parser.run(fail)) {
            on_fail(fail);
            return -2;
        }
    }

    // run the symbolic linker pass
    pt_pass_symbolic_link_t linker;
    if (!linker.run(parse_tree, fail)) {
        on_fail(fail);
    }

    // run the ast printer pass
    pt_pass_printer_t printer;
    if (!printer.run(parse_tree, fail)) {
        on_fail(fail);
    }
    
#if 0
    // run the c++ codegen backend
    llb_backend_cpp_t backend_cpp;
    parse_tree.visit( backend_cpp );
#endif

    getchar();
    return 0;
}
