#include "llb_error.h"
#include "llb_lexer.h"

namespace {

    template <typename type_t>
    bool in_range(type_t x, type_t low, type_t high) {
        return x >= low && x <= high;
    }

    bool is_decimal(char ch) {
        return in_range(ch, '0', '9');
    }

    bool is_number(char ch, int base) {
        bool b = false;
        if (base >= 2)
            b |= in_range(ch, '0', '1');
        if (base >= 10)
            b |= in_range(ch, '0', '9');
        if (base >= 16) {
            b |= in_range(ch, 'a', 'f');
            b |= in_range(ch, 'A', 'F');
        }
        return b;
    }

    uint32_t char_value(char ch) {
        if (in_range(ch, '0', '9'))
            return ch - '0';
        if (in_range(ch, 'a', 'f'))
            return (ch - 'a') + 10;
        if (in_range(ch, 'A', 'F'))
            return (ch - 'A') + 10;
        return -1;
    }

    bool is_alpha( char ch ) {
        return in_range(ch, 'a', 'z') ||
               in_range(ch, 'A', 'Z') ||
               ch == '_';
    }

    struct pair_t {
        const char * str_;
        token_type_t type_;
    };

    pair_t specials[] = {

        "+",   tok_op_add,
        "-",   tok_op_sub,
        "*",   tok_op_mul,
        "/",   tok_op_div,
        "%",   tok_op_mod,
        "<<",  tok_op_shl,
        ">>",  tok_op_shl,

        "<",   tok_op_lt,
        "<=",  tok_op_ltequ,
        ">",   tok_op_gt,
        ">=",  tok_op_gtequ,
        "==",  tok_op_equ,
        "!=",  tok_op_nequ,

        ".",   tok_chr_dot,
        "(",   tok_chr_paren_l,
        ")",   tok_chr_paren_r,
        "=",   tok_chr_equal,
        ",",   tok_chr_comma,

        "and", tok_op_and,
        "or",  tok_op_or,

        nullptr, tok_empty
    };

    pair_t keywords[] = {

        "global",   tok_key_global,
        "local",    tok_key_local,

        "none",     tok_key_none,
        "bool",     tok_key_bool,
        "int",      tok_key_int,
        "float",    tok_key_float,
        "string",   tok_key_string,
        "func",     tok_key_func,

        "type",     tok_key_type,
        "function", tok_key_function,
        "end",      tok_key_end,

        "true",     tok_key_true,
        "false",    tok_key_false,

        "if",       tok_key_if,
        "else",     tok_key_else,
        "while",    tok_key_while,
        "for",      tok_key_for,
        "break",    tok_key_break,
        "continue", tok_key_continue,
        "return",   tok_key_return,
        "leave",    tok_key_leave,

        nullptr, tok_empty
    };
} // namespace {}

char lexer_t::next( ) {

    switch (*stream_) {
    case ('\0'):
        return *stream_;

    case ('\n') :
        tokens_.push(new_token(tok_eol));
        line_table_.push_back(stream_ + 1);
        line_++;
        column_ = 0;

    default:
        column_++;
        return *(stream_++);
    }
}

token_type_t lexer_t::identify_alpha( std::string & str ) {

    // lookup in keyword table
    for ( uint32_t i=0; keywords[i].str_; ++i )
        if ( str == keywords[i].str_ )
            return keywords[i].type_;

    // if unknown assume its an identifier
    return tok_identifier;
}

void lexer_t::skip_whitespace() {

    while (! at_eof()) {
        switch (peek()) {
        case ('\t'):
        case ('\r'):
        case (' ' ):
            next();
            break;
        default:
            return;
        }
    }
}

void lexer_t::eat_alpha( ) {

    assert (is_alpha(peek()));

    const char * start = stream_;

    while (!at_eof()) {

        char ch = peek();
        if (!(is_alpha(ch) | is_decimal(ch)))
            break;
        next();
    }
    const char * end = stream_;

    if ( start == end )
        throw exception_t("zero length alpha");

    std::string str( start, end );
    assert( str.size() > 0 );

    token_t token = new_token( identify_alpha( str ) );
    token.value_.string_ = str;
    tokens_.push( token );
}

void lexer_t::eat_number( ) {

    assert( is_number( peek(), 10 ));

    uint32_t num  = 0;
    uint32_t div  = 0;
    uint32_t base = 10;

    if (peek(0) == '0') {
        if (peek(1) == 'x') {
            base = 16;
            skip(2);
        } 
        else if (peek(1) == 'b') {
            base = 2;
            skip(2);
        }
    }

    while (! at_eof() ) {
        
        if (peek() == '.') {

            if (base != 10)
                throw exception_t("malaformed literal");
            if (div > 0)
                throw exception_t("malaformed literal");
            div = 1;
            next();
        }
        else {

            if (is_number(peek(), base)) {

                num *= base;
                num += char_value(next());
                div *= (div > 0) ? base : 0;
            }
            else
                break;
        }
    }

    if (div == 0) {

        token_t tok(tok_lit_integer);
        tok.value_.int_ = num;
        tokens_.push(tok);
    }
    else {

        token_t tok = new_token(tok_lit_float);
        tok.value_.float_ = float(double(num) / double(div));
        tokens_.push(tok);
    }
}

void lexer_t::eat_string( ) {

    assert(next() == '\"');

    const char * start = stream_;
    const char * end = stream_;

    while (!at_eof()) {
        char ch = next();
        if (ch == '\n')
            throw exception_t("string not terminated");
        if (ch == '\"')
            break;
        end = stream_;
    }

    if (start == end)
        throw exception_t("error parsing string literal");

    token_t tok = new_token(tok_lit_string);
    tok.value_.string_.assign(start, end);
    tokens_.push( tok );
}

bool lexer_t::found( const char * str ) {

    int32_t i=0;
    for (i=0; str[i]!='\0'; ++i)
        if (str[i] != peek(i))
            return false;

    stream_ += i;
    return true;
}

bool lexer_t::eat_special( ) {

    for (uint32_t i=0; specials[i].str_; ++i ) {

        const char * str = specials[i].str_;
        if ( found(str) ) {
            tokens_.push(new_token(specials[i].type_));
            return true;
        }
    }
    return false;
}

void lexer_t::eat_comment() {
    while (!at_eof())
        if (next() == '\n')
            break;
}

bool lexer_t::run( exception_t & error ) {

    line_table_.clear();
    line_table_.push_back(stream_);

    try {

        while (! at_eof()) {

            skip_whitespace();

            char ch = peek();

            if (ch == '\0')
                break;

            if (ch == '\n') {
                next();
                continue;
            }

            if (is_alpha(ch)) {
                eat_alpha();
                continue;
            }

            if (is_decimal(ch)) {
                eat_number();
                continue;
            }

            if (ch == '\"') {
                eat_string();
                continue;
            }

            if (ch == ';') {
                eat_comment();
                continue;
            }

            if (eat_special())
                continue;

            throw exception_t("unexpected charactor");
        }

        tokens_.push(new_token(tok_eol));
        tokens_.push(new_token(tok_eof));
    }
    catch ( exception_t thrown ) {
        error = thrown;
        return false;
    }
    return true;
}

std::string lexer_t::get_line(uint32_t line) {

    if (line_table_.size() < line)
        return std::string();
    return line_table_[line];
}
