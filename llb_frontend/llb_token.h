#pragma once

#include <assert.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

#include "llb_error.h"

enum token_type_t {

    tok_empty = 0,

    tok_eof,
    tok_eol,

    tok_chr_dot,
    tok_chr_comma,
    tok_chr_equal,
    tok_chr_paren_l,
    tok_chr_paren_r,

    tok_identifier,

    tok_lit_integer,
    tok_lit_float,
    tok_lit_string,

    tok_key_global,
    tok_key_local,

    tok_key_none,
    tok_key_bool,
    tok_key_int,
    tok_key_float,
    tok_key_string,
    tok_key_func,

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
};

struct token_t {

    token_t( token_type_t type )
        : type_( type )
    {
    }

    token_type_t type_;

    struct value_t {

        value_t()
            : int_(0)
            , float_(0.f)
            , string_()
        {
        }

        int32_t int_;
        float float_;
        std::string string_;

    } value_;

};

class token_list_t {
protected:
    token_t & head( ) {
        return list_[ index_ ];
    }

    uint32_t index_;
    std::vector<token_t> list_;

public:
    token_list_t()
        : index_( 0 )
        , list_()
    {
    }

    void push( const token_t & t ) {
        list_.push_back( t );
    }

    bool at_eof( ) const {
        return list_[index_].type_ == tok_eof;
    }

    token_t & peek( int i ) {
        assert (index_ + i < list_.size());
        return list_[index_+i];
    }

    token_t & pop( ) {
        token_t & tok = head();
        if (tok.type_ != tok_eof)
            index_++;
        assert (index_ < list_.size());
        return tok;
    }

    token_t & pop( token_type_t type ) {
        if (peek(0).type_ != type)
            throw exception_t("unexpected token");
        return pop();
    }

    bool found( token_type_t type ) {
        if ( head().type_ != type )
            return false;
        index_++;
        assert (index_ < list_.size());
        return true;
    }
};
