#pragma once

#include <stdint.h>
#include <string>

#include "llb_token_types.h"
#include "llb_module.h"

class llb_token_list_t;
struct llb_token_t;

class llb_lexer_t {
protected:
    shared_llb_module_t module_;
    uint32_t line_;
    uint32_t column_;
    const char * stream_;

    char next( );

    char peek( ) const {
        return *stream_;
    }

    char peek( uint32_t i ) const {
        return stream_[i];
    }

    void skip(uint32_t i) {
        stream_ += i;
    }

    bool at_eof( ) const {
        return peek() == '\0';
    }

    void skip_whitespace( );

    llb_token_type_t identify_alpha( std::string & str );

    bool found( const char * str );

    void eat_alpha( );
    void eat_number( );
    void eat_string( );
    bool eat_special( );
    void eat_comment( );

    llb_token_t new_token(llb_token_type_t type);

    void fail(std::string str) {
        throw llb_fail_t(str, line_, column_);
    }

    void push_token(const llb_token_t & token);

public:

    llb_lexer_t(shared_llb_module_t module);
    
    bool run(struct llb_fail_t &);
};
