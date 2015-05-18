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
    uint32_t column_;

    std::vector<const char*> line_table_;

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

    token_t new_token(token_type_t type) {
        token_t tok(type);
        tok.line_ = line_;
        tok.column_ = column_;
        return tok;
    }

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

    std::string get_line(uint32_t line);

    bool run( struct exception_t & );
};
