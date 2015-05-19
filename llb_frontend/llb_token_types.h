#pragma once

enum token_type_t {

    tok_empty = 0,

    tok_eof,            // '\0'
    tok_eol,            // '\n'

    tok_chr_dot,        // .
    tok_chr_colon,      // :
    tok_chr_comma,      // ,
    tok_chr_assign,     // =
    tok_chr_paren_l,    // (
    tok_chr_paren_r,    // )

    tok_identifier,

    tok_lit_integer,
    tok_lit_float,
    tok_lit_string,

    tok_key_global,
    tok_key_local,

    tok_key_type,
    tok_key_function,
    tok_key_end,

    tok_key_true,
    tok_key_false,

    tok_key_if,
    tok_key_else,
    tok_key_while,
    tok_key_for,
    tok_key_break,
    tok_key_continue,
    tok_key_return,
    tok_key_leave,
    tok_key_new,

    tok_op_lt,
    tok_op_ltequ,
    tok_op_gt,
    tok_op_gtequ,
    tok_op_equ,
    tok_op_nequ,

    tok_op_add,
    tok_op_sub,
    tok_op_mul,
    tok_op_div,
    tok_op_mod,
    tok_op_shl,
    tok_op_shr,

    tok_op_and,
    tok_op_or,

    tok_ury_not,
    tok_ury_neg,

    tok_count__
};
