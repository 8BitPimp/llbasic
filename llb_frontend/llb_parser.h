#pragma once

#include <stdint.h>

// forward
struct token_t;
class token_list_t;
struct expr_info_t;
struct pt_node_t;
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

    void parse_expr_reduce( expr_info_t &, int32_t prec );
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

    void parse_type();
    void parse_field();

    void parse_module();

    void fail(std::string str, const pt_node_t & node);
    void fail(std::string str, const token_t & tok);

public:
    parser_t( token_list_t & list, pt_t & ast )
        : list_( list )
        , pt_( ast )
    {
    }

    bool run(struct llb_fail_t &);
};
