// llbasic/llb_frontend
#include "llb_error.h"
#include "llb_token.h"
#include "llb_lexer.h"
#include "llb_pt.h"
#include "llb_parser.h"
#include "llb_backend_cpp.h"

// llbasic/llb_passes
#include "llb_pass_printer.h"

const char source1[] = R"(
global x.int = 3 + 4
function main( x.int, y.int )
    local x.float = 3
    x = x * 3 + 2 * 4
    return x + 3
end
global l2.int=3+2, i3.float, i4.float=12*3
function y.int( )
    local x.int=3, y.float=5
end
)";

int main( ) {

    exception_t except;

    token_list_t tokens;
    lexer_t lexer( source1, sizeof( source1 ), tokens );
    if (! lexer.run( except ) ) {
        return -1;
    }

    pt_t parse_tree;
    parser_t parser( tokens, parse_tree );
    if (! parser.run( except ) ) {
        return -2;
    }

    pt_pass_printer_t printer;
    parse_tree.visit( printer );

    llb_backend_cpp_t backend_cpp;
    parse_tree.visit( backend_cpp );

    getchar();
    return 0;
}
