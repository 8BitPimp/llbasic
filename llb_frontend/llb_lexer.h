#pragma once

#include <stdint.h>
#include <string>
#include "llb_token.h"

class token_list_t;

class lexer_t {
protected:
    const char * stream_;
    uint32_t size_;
    token_list_t & tokens_;
    uint32_t line_;

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

    token_type_t identify_alpha( std::string & str );

    bool found( const char * str );

    void eat_alpha( );
    void eat_number( );
    void eat_string( );
    bool eat_special( );
    void eat_comment( );

public:

    lexer_t( const char * stream,
             uint32_t size,
             token_list_t & tokens )
        : stream_( stream )
        , size_( size )
        , tokens_( tokens )
        , line_( 0 )
    {
    }

    bool run( struct exception_t & );
};
