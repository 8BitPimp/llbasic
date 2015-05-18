#pragma once

#include <stdint.h>

// forward
class token_list_t;
struct expr_info_t;
class pt_t;

class parser_t {
protected:
    token_list_t & list_;
    pt_t & pt_;

    void parse_var_decl();
    void parse_function();
#if 0
    void parse_type();
#endif

    void parse_expr_reduce( expr_info_t &, uint32_t mark );
    void parse_expr_lhs( expr_info_t & );
    void parse_expr_inner( expr_info_t & );
    void parse_expr();

    void parse_stmt();
    void parse_stmt_assign();
    void parse_stmt_call();
    void parse_stmt_if();
    void parse_stmt_while();
    void parse_stmt_for();
    void parse_stmt_break();
    void parse_stmt_continue();
    void parse_stmt_return();

    void parse_module();

public:
    parser_t( token_list_t & list, pt_t & ast )
        : list_( list )
        , pt_( ast )
    {
    }

    bool run( struct exception_t & );
};
