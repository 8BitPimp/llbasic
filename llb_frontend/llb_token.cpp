#include "llb_string.h"
#include "llb_token.h"

struct map_tok_name_t {

    token_type_t type_;
    const char * name_;
};

static
map_tok_name_t map_tok_name[tok_count__] = {

    tok_empty,          "tok_empty",

    tok_eof,            "eof",
    tok_eol,            "eol",

    tok_chr_dot,        ".",
    tok_chr_colon,      ":",
    tok_chr_comma,      ",",
    tok_chr_assign,     "=",
    tok_chr_paren_l,    "(",
    tok_chr_paren_r,    ")",

    tok_identifier,     "identifier",

    tok_lit_integer,    "int",
    tok_lit_float,      "float",
    tok_lit_string,     "string",

    tok_key_global,     "global",
    tok_key_local,      "local",

    tok_key_type,       "type",
    tok_key_function,   "function",
    tok_key_end,        "end",

    tok_key_true,       "true",
    tok_key_false,      "false",

    tok_key_if,         "if",
    tok_key_else,       "else",
    tok_key_while,      "while",
    tok_key_for,        "for",
    tok_key_break,      "break",
    tok_key_continue,   "continue",
    tok_key_return,     "return",
    tok_key_leave,      "leave",
    tok_key_new,        "new",

    tok_op_lt,          "<",
    tok_op_ltequ,       "<=",
    tok_op_gt,          ">",
    tok_op_gtequ,       ">=",
    tok_op_equ,         "==",
    tok_op_nequ,        "!=",

    tok_op_add,         "+",
    tok_op_sub,         "-",
    tok_op_mul,         "*",
    tok_op_div,         "/",
    tok_op_mod,         "%",
    tok_op_shl,         "<<",
    tok_op_shr,         ">>",

    tok_op_and,         "&",
    tok_op_or,          "|",

    tok_ury_not,        "!",
    tok_ury_neg,        "-",
};

namespace {

    template <typename type_t, int size>
    static
        uint32_t array_size(type_t(&list)[size]) {
        return size;
    }

} /* namespace {} */

token_t & token_list_t::pop(token_type_t type) {

    if (peek(0).type_ != type) {
        const char * name = token_t::get_type_symbol(type);
        std::string msg = llb_string_t::format("expected '%0'", { name });
        throw llb_fail_t(msg, peek(0));
    }
    return pop();
}

const char * token_t::get_type_symbol(token_type_t type) {

    for (uint32_t i = 0; i < array_size(map_tok_name); ++i)
        if (map_tok_name[i].type_ == type)
            return map_tok_name[i].name_;

    assert(!"token without lookup entry");
    return nullptr;
}
