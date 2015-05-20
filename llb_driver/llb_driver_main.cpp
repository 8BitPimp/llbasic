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
#include "llb_pass_printer.h"

const char source1[] = R"(

global x:int = 3 + 4

function main:int( x:int, y:int )
    local x:int = 3
    x = func( l + y ) * 3 + 2.1 * 4
    return x + 3
end

global l2:int=3-2, i3:float, i4:float=12*3

function y:int( )
    local x:int=3, y:float=5
end
)";

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

        for (uint32_t i = 0; i < fail.column_; ++i)
            putchar(' ');
        putchar('^');
        printf("\n");
    }

    getchar();
    exit(0);
}

int main( ) {

    llb_fail_t except;

    token_list_t tokens;
    lexer_t lexer( source1, sizeof( source1 ), tokens );
    if (! lexer.run( except ) ) {

        on_fail(except, lexer);
        return -1;
    }

    pt_t parse_tree;
    parser_t parser( tokens, parse_tree );
    if (! parser.run( except ) ) {

        on_fail(except, lexer);
        return -2;
    }

    pt_pass_printer_t printer;
    parse_tree.visit( printer );

    llb_backend_cpp_t backend_cpp;
    parse_tree.visit( backend_cpp );

    getchar();
    return 0;
}
