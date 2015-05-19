#include "llb_error.h"
#include "llb_parser.h"
#include "llb_token.h"
#include "llb_pt.h"

void parser_t::parse_stmt_return() {

    token_t tok = list_.pop(tok_key_return);
    shared_pt_node_t node;

    if (! list_.found( tok_eol ) ) {
        parse_expr();
        node = pt_.pop();

        list_.pop( tok_eol );
    }
    pt_.push( new pt_return_t( tok, node ) );
}

void parser_t::parse_stmt_continue() {
    list_.pop(tok_key_continue);
}

void parser_t::parse_stmt_break() {

    list_.pop(tok_key_break);
}

void parser_t::parse_stmt_while() {

    token_t tok = list_.pop(tok_key_while);
    list_.pop(tok_chr_paren_l);
    parse_expr();
    list_.pop(tok_chr_paren_r);

    std::unique_ptr<pt_while_t> ptr_while( new pt_while_t( tok, pt_.pop() ) );
    assert( ptr_while.get() );

    while (! list_.found(tok_key_end)) {

        parse_stmt();
        ptr_while->add_stmt( pt_.pop() );
    }

    pt_.push( ptr_while.release() );
}

void parser_t::parse_stmt_for() {

    list_.pop(tok_key_for);
    list_.pop(tok_chr_paren_l);
}

void parser_t::parse_stmt_if() {

    token_t tok = list_.pop(tok_key_if);
    parse_expr();

    std::unique_ptr<pt_if_t> stmt_if( new pt_if_t( tok, pt_.pop() ) );

    bool in_true_branch = true;
    while (! list_.found(tok_key_end)) {

        if (list_.found(tok_key_else)) {
            if (! in_true_branch )
                fail("unexpected else", list_.previous());
            in_true_branch = false;
        }

        parse_stmt();
        stmt_if->add_stmt( pt_.pop(), in_true_branch );
    }
}

void parser_t::parse_stmt_call( ) {

    token_t name = list_.pop(tok_identifier);
    list_.pop( tok_chr_paren_l );

    std::unique_ptr<pt_call_t> call( new pt_call_t( name ) );
    assert( call.get() );

    if (! list_.found( tok_chr_paren_r )) {

        do {
            parse_expr();
            call->add_arg( pt_.pop() );
        }
        while ( list_.found( tok_chr_comma ));

        list_.pop( tok_chr_paren_r );
    }

    pt_.push( call.release() );
}

#if 0
void parser_t::parse_stmt_assign() {

    token_t name = list_.pop( tok_identifier );
    list_.pop( tok_chr_equal );
    parse_expr();
    list_.pop( tok_eol );

    shared_pt_node_t expr = pt_.pop( );
    pt_.push( new pt_assign_t( name, expr ));
}
#endif

void parser_t::parse_stmt() {

    switch (list_.peek(0).type_) {
    case (tok_identifier):

#if 0
        switch (list_.peek(1).type_) {
        case (tok_chr_equal):
            parse_stmt_assign();
            break;

        case (tok_chr_paren_l):
            parse_stmt_call();
            break;

        default:
            throw exception_t("unexpected token type");
    }
#else
        parse_expr();
        pt_.push( new pt_stmt_t( pt_.pop() ) );
#endif
        break;

    case (tok_key_local):
        parse_var_decl();
        break;

    case (tok_key_if):
        parse_stmt_if();
        break;

    case (tok_key_while):
        parse_stmt_while();
        break;

    case (tok_key_break):
        parse_stmt_break();
        break;

    case (tok_key_continue):
        parse_stmt_continue();
        break;

    case (tok_key_return):
        parse_stmt_return();
        break;

    case (tok_key_for):
        parse_stmt_for();
        break;

    default:
        fail("unexpected token type", list_.peek(0));
    }
}

void parser_t::parse_function() {

    list_.pop( tok_key_function );
    token_t name = list_.pop( tok_identifier );
    list_.pop(tok_chr_dot);
    token_t type = list_.pop(tok_identifier);
    list_.pop( tok_chr_paren_l );

    std::unique_ptr<pt_decl_function_t> ptr_func( new pt_decl_function_t( name, type ) );
    assert( ptr_func.get() );

    if (! list_.found( tok_chr_paren_r )) {

        do {
            token_t name = list_.pop( tok_identifier );
            list_.pop( tok_chr_dot );
            token_t type = list_.pop();
            pt_.push( new pt_decl_var_t( pt_decl_var_t::e_arg, name, type, shared_pt_node_t() ) );
            ptr_func->add_arg( pt_.pop() );
        }
        while ( list_.found( tok_chr_comma ));

        list_.pop(tok_chr_paren_r);
    }
    list_.pop( tok_eol );

    while (! list_.found(tok_key_end)) {

        if (list_.found(tok_eol))
            continue;

        uint32_t index = pt_.index();
        parse_stmt();
        assert(pt_.index() > index); // we have something
        while (pt_.index() > index)
            ptr_func->add_stmt(pt_.pop());
    }
    list_.pop(tok_eol );

    pt_.push(ptr_func.release());
}

void parser_t::parse_var_decl() {

    pt_decl_var_t::scope_t scope = pt_decl_var_t::e_unknown;
    if (list_.found(tok_key_local))
        scope = pt_decl_var_t::e_local;
    else if (list_.found(tok_key_global))
        scope = pt_decl_var_t::e_global;

    do {
        token_t name = list_.pop( tok_identifier );
        list_.pop(tok_chr_dot);
        token_t type = list_.pop( tok_identifier );
        shared_pt_node_t expr;

        if (list_.found(tok_chr_assign)) {

            parse_expr();
            expr = pt_.pop();
        }

        pt_.push(new pt_decl_var_t(scope, name, type, expr));
    }
    while (list_.found(tok_chr_comma));

    list_.pop(tok_eol);
}

void parser_t::parse_module() {

    std::unique_ptr<pt_module_t> module( new pt_module_t );
    assert( module.get() );

    while (!list_.at_eof()) {

        token_t &tok = list_.peek(0);
        switch (tok.type_) {
            case (tok_key_global): {
                uint32_t tide = pt_.index();
                parse_var_decl();
                while (pt_.index() > tide)
                    module->add_global(pt_.pop());
                }
                break;

            case (tok_key_function):
                parse_function();
                module->add_function(pt_.pop());
                break;

            case (tok_eol):
                list_.pop(tok_eol);
                break;

            default:
                fail("unexpected token", tok);
        }
    }

    assert(pt_.index() == 0);
    pt_.push( module.release() );
}

void parser_t::fail(std::string str, const pt_node_t & node) {

    source_pos_t pos = node.get_source_pos();
    throw llb_fail_t(str, pos.line_, pos.column_);
}

void parser_t::fail(std::string str, const token_t & tok) {

    throw llb_fail_t(str, tok.line_, tok.column_);
}

bool parser_t::run(llb_fail_t & error) {

    try {
        parse_module();
    }
    catch (llb_fail_t thrown) {

        error = thrown;
        return false;
    }

    return true;
}
