// llb_util
#include "llb_string.h"

// llb_frontend
#include "llb_error.h"
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

)";

struct module_t {

    const char * source_;
    uint32_t size_;

}
module[] = {
    { module1, sizeof( module1 ) },
    { module2, sizeof( module2 ) }
};

void on_fail( const llb_fail_t & fail, const lexer_t & lexer ) {

    std::string msg = llb_string_t::format(
        "error: %0 @ line %1:%2", {
        fail.msg_.c_str(),
        fail.line_,
        fail.column_ });
    printf("%s\n", msg.c_str());

    std::string line = lexer.get_line(fail.line_);
    if (!line.empty()) {
        printf("\"%s\"\n", line.c_str());

        for (uint32_t i = 0; i <= fail.column_; ++i)
            putchar(' ');
        putchar('^');
        printf("\n");
    }

    getchar();
    exit(0);
}

int main( ) {

    llb_fail_t fail;

    // perform lexical analysis
    token_list_t tokens;
    lexer_t lexer(module[0].source_, module[0].size_, tokens);
    if (!lexer.run(fail)) {
        on_fail(fail, lexer);
        return -1;
    }

    // construct a parse tree
    pt_t parse_tree;
    parser_t parser( tokens, parse_tree );
    if (!parser.run(fail)) {
        on_fail(fail, lexer);
        return -2;
    }

    // run the symbolic linker pass
    pt_pass_symbolic_link_t linker;
    if (!linker.run(parse_tree, lexer, fail)) {
        on_fail(fail, lexer);
    }

    // run the ast printer pass
    pt_pass_printer_t printer;
    if (!printer.run(parse_tree, lexer, fail)) {
        on_fail(fail, lexer);
    }
    
    // run the c++ codegen backend
    llb_backend_cpp_t backend_cpp;
    parse_tree.visit( backend_cpp );

    getchar();
    return 0;
}
