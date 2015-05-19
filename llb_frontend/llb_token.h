#pragma once

#include <assert.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

#include "llb_error.h"
#include "llb_token_types.h"

struct token_t {

    token_t( token_type_t type )
        : type_( type )
        , line_( 0 )
        , column_( 0 )
    {
    }

    token_type_t type_;

    const std::string & get_string() const {
        switch (type_) {
        case (tok_lit_string):
        case (tok_identifier):
            break;
        default:
            assert(!"internal error");
        }
        return value_.string_;
    }

    const int get_int() const {
        switch (type_) {
        case (tok_lit_integer):
            break;
        default:
            assert(!"internal error");
        }
        return value_.int_;
    }

    const float get_float() const {
        switch (type_) {
        case (tok_lit_float):
            break;
        default:
            assert(!"internal error");
        }
        return value_.float_;
    }

    void fail(const char * msg) {
        throw llb_fail_t(msg, line_, column_);
    }

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

    uint32_t line_;
    uint32_t column_;
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

    token_t & pop(token_type_t type);

    token_t & previous() {
        if (index_ <= 0)
            assert(!"internal error");
        return list_[index_ - 1];

    }

    bool found( token_type_t type ) {
        if ( head().type_ != type )
            return false;
        index_++;
        assert (index_ < list_.size());
        return true;
    }
};
