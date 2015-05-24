#pragma once

#include <stdint.h>

#include "llb_forward.h"

class llb_parser_t {
protected:
    shared_llb_module_t module_;
    llb_token_list_t & list_;
    pt_t & pt_;

    void parse_var_decl();
    void parse_function();
    void parse_function_decl();

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

    void parse_external();

    void parse_type();
    void parse_field();

    void parse_module();

    void fail(std::string str, const pt_node_t & node);
    void fail(std::string str, const llb_token_t & tok);

public:
    llb_parser_t(shared_llb_module_t module, pt_t & ast);

    bool run(struct llb_fail_t &);
};
