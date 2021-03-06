#include "llb_fail.h"
#include "llb_lexer.h"
#include "llb_token.h"

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
        llb_token_type_t type_;
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
        ":",   tok_chr_colon,
        "(",   tok_chr_paren_l,
        ")",   tok_chr_paren_r,
        "=",   tok_chr_assign,
        ",",   tok_chr_comma,

        "and", tok_op_and,
        "or",  tok_op_or,

        nullptr, tok_empty
    };

    pair_t keywords[] = {

        "global",   tok_key_global,
        "local",    tok_key_local,

        "type",     tok_key_type,
        "function", tok_key_function,
        "external", tok_key_external,
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
        "new",      tok_key_new,

        nullptr, tok_empty
    };
} // namespace {}

char llb_lexer_t::next( ) {

    switch (*stream_) {
    case ('\0'):
        return *stream_;

    case ('\n') :
        push_token(new_token(tok_eol));
        if ( module_ )
            module_->line_begin_.push_back(stream_ + 1);
        line_++;
        column_ = 0;

    default:
        return *(stream_++);
    }
}

llb_token_type_t llb_lexer_t::identify_alpha( std::string & str ) {

    // lookup in keyword table
    for ( uint32_t i=0; keywords[i].str_; ++i )
        if ( str == keywords[i].str_ )
            return keywords[i].type_;

    // if unknown assume its an identifier
    return tok_identifier;
}

void llb_lexer_t::skip_whitespace() {

    while (! at_eof()) {
        switch (peek()) {
        case ('\t'):
        case ('\r'):
        case (' ') :
            column_++;
            next();
            break;
        default:
            return;
        }
    }
}

void llb_lexer_t::eat_alpha( ) {

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
        fail("zero length identifier");

    std::string str( start, end );
    assert( str.size() > 0 );

    llb_token_t token = new_token( identify_alpha( str ) );
    switch (token.type_) {
    case ( tok_lit_string ):
    case (tok_identifier) :
        token.set_string(str);
    }
    push_token(token);
}

void llb_lexer_t::eat_number( ) {

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

            if ((base != 10) || (div > 0))
                fail("malformed literal");
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

        llb_token_t tok = new_token(tok_lit_integer);
        tok.set_int( num );
        push_token(tok);
    }
    else {

        llb_token_t tok = new_token(tok_lit_float);
        tok.set_float( float(double(num) / double(div)) );
        push_token(tok);
    }
}

void llb_lexer_t::eat_string( ) {

    assert(next() == '\"');

    const char * start = stream_;
    const char * end = stream_;

    while (!at_eof()) {
        char ch = next();
        if (ch == '\n')
            fail("string not terminated");
        if (ch == '\"')
            break;
        end = stream_;
    }

    if (start == end)
        fail("error parsing string literal");

    llb_token_t tok = new_token(tok_lit_string);
    tok.set_string( std::string(start, end) );
    push_token(tok);
}

bool llb_lexer_t::found( const char * str ) {

    int32_t i=0;
    for (i=0; str[i]!='\0'; ++i)
        if (str[i] != peek(i))
            return false;

    stream_ += i;
    return true;
}

bool llb_lexer_t::eat_special( ) {

    for (uint32_t i=0; specials[i].str_; ++i ) {

        const char * str = specials[i].str_;
        if ( found(str) ) {
            push_token(new_token(specials[i].type_));
            return true;
        }
    }
    return false;
}

void llb_lexer_t::eat_comment() {
    while (!at_eof())
        if (next() == '\n')
            break;
}

bool llb_lexer_t::run(llb_fail_t & error) {

    if (module_) {
        module_->line_begin_.clear();
        module_->line_begin_.push_back(stream_);
    }

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

            fail("unexpected charactor");
        }

        push_token(new_token(tok_eol));
        push_token(new_token(tok_eof));
    }
    catch (llb_fail_t thrown) {
        error = thrown;
        return false;
    }
    return true;
}

llb_token_t llb_lexer_t::new_token(llb_token_type_t type) {

    llb_token_t tok(type);
    tok.pos_.set(line_, column_, module_);

    if (module_) {

        auto & line_tab = module_->line_begin_;
        assert(!line_tab.empty());
        const char * start = *line_tab.rbegin();
        assert(stream_ >= start);
        column_ = uint32_t(stream_ - start);
    }

    return tok;
};

llb_lexer_t::llb_lexer_t(shared_llb_module_t module)
    : module_(module)
    , line_(0)
    , column_(0)
    , stream_( nullptr )
{
    assert(module.get());
    stream_ = module->source_;
}

void llb_lexer_t::push_token(const llb_token_t & token) {

    assert(module_);
    module_->tokens_->push(token);
}
