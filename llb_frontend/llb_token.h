#pragma once

#include <assert.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

#include "llb_forward.h"
#include "llb_fail.h"
#include "llb_token_types.h"

struct llb_token_t {

    static const char * get_type_symbol( llb_token_type_t type );

    llb_token_t( llb_token_type_t type )
        : type_( type )
        , pos_()
    {
    }

    llb_token_type_t type_;

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

    void set_string(const std::string & str) {
        switch (type_) {
        case (tok_lit_string) :
        case (tok_identifier) :
            value_.string_ = str;
            break;
        default:
            assert(!"internal error");
        }
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

    void set_int( int32_t in ) {
        switch (type_) {
        case (tok_lit_integer):
            value_.int_ = in;
            break;
        default:
            assert(!"internal error");
        }
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

    void set_float( float in ) {
        switch (type_) {
        case (tok_lit_float) :
            value_.float_ = in;
            break;
        default:
            assert(!"internal error");
        }
    }

    void fail(const char * msg) {
        throw llb_fail_t(msg, pos_);
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

    llb_location_t pos_;
};

class llb_token_list_t {
protected:
    llb_token_t & head( ) {
        return list_[ index_ ];
    }

    uint32_t index_;
    std::vector<llb_token_t> list_;

public:
    llb_token_list_t()
        : index_( 0 )
        , list_()
    {
    }

    void push( const llb_token_t & t ) {
        list_.push_back( t );
    }

    bool at_eof( ) const {
        return list_[index_].type_ == tok_eof;
    }

    llb_token_t & peek( int i ) {
        assert (index_ + i < list_.size());
        return list_[index_+i];
    }

    llb_token_t & pop( ) {
        llb_token_t & tok = head();
        if (tok.type_ != tok_eof)
            index_++;
        assert (index_ < list_.size());
        return tok;
    }

    llb_token_t & pop(llb_token_type_t type);

    llb_token_t & previous() {
        if (index_ <= 0)
            assert(!"internal error");
        return list_[index_ - 1];

    }

    bool found( llb_token_type_t type ) {
        if ( head().type_ != type )
            return false;
        index_++;
        assert (index_ < list_.size());
        return true;
    }
};
