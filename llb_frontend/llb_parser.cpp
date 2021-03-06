#include "llb_fail.h"
#include "llb_parser.h"
#include "llb_token.h"
#include "llb_pt.h"
#include "llb_module.h"

namespace {

    const llb_token_type_t type_prefix = tok_chr_colon;
}

void llb_parser_t::parse_stmt_return() {

    llb_token_t tok = list_.pop(tok_key_return);
    shared_pt_node_t node;

    if (! list_.found( tok_eol ) ) {
        parse_expr();
        node = pt_.pop();

        list_.pop( tok_eol );
    }
    pt_.push( new pt_return_t( tok, node ) );
}

void llb_parser_t::parse_stmt_continue() {
    list_.pop(tok_key_continue);
}

void llb_parser_t::parse_stmt_break() {

    list_.pop(tok_key_break);
}

void llb_parser_t::parse_stmt_while() {

    llb_token_t tok = list_.pop(tok_key_while);
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

void llb_parser_t::parse_stmt_for() {

    list_.pop(tok_key_for);
    list_.pop(tok_chr_paren_l);
}

void llb_parser_t::parse_stmt_if() {

    llb_token_t tok = list_.pop(tok_key_if);
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

void llb_parser_t::parse_stmt_call( ) {

    llb_token_t name = list_.pop(tok_identifier);
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

void llb_parser_t::parse_stmt() {

    switch (list_.peek(0).type_) {
    case (tok_identifier):
        parse_expr();
        pt_.push( new pt_stmt_t( pt_.pop() ) );
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

void llb_parser_t::parse_function_decl() {

    list_.pop(tok_key_function);
    llb_token_t name = list_.pop(tok_identifier);
    list_.pop(type_prefix);
    llb_token_t type = list_.pop(tok_identifier);
    list_.pop(tok_chr_paren_l);

    std::unique_ptr<pt_function_decl_t> ptr_func(new pt_function_decl_t(name, type));
    assert(ptr_func.get());

    if (!list_.found(tok_chr_paren_r)) {

        do {
            llb_token_t name = list_.pop(tok_identifier);
            list_.pop(type_prefix);
            llb_token_t type = list_.pop();
            pt_.push(new pt_decl_var_t(pt_decl_var_t::e_arg, name, type, shared_pt_node_t()));
            ptr_func->add_arg(pt_.pop());
        } while (list_.found(tok_chr_comma));

        list_.pop(tok_chr_paren_r);
    }
    list_.pop(tok_eol);

    pt_.push(ptr_func.release());
}

void llb_parser_t::parse_function() {

    parse_function_decl();
    shared_pt_node_t decl = pt_.top_specific<pt_function_decl_t>();

    std::unique_ptr<pt_function_body_t> ptr_func(new pt_function_body_t(decl));
    assert(ptr_func.get());

    uint32_t floor = pt_.index();
    while (! list_.found(tok_key_end)) {

        if (list_.found(tok_eol))
            continue;

        parse_stmt();
        assert(pt_.index() > floor); // we must have something
        while (pt_.index() > floor)
            ptr_func->add_stmt(pt_.pop());
    }
    list_.pop(tok_eol );

    decl->upcast<pt_function_decl_t>()->body_ = shared_pt_node_t(ptr_func.release());
}

void llb_parser_t::parse_var_decl() {

    pt_decl_var_t::scope_t scope = pt_decl_var_t::e_unknown;
    if (list_.found(tok_key_local))
        scope = pt_decl_var_t::e_local;
    else if (list_.found(tok_key_global))
        scope = pt_decl_var_t::e_global;

    do {
        llb_token_t name = list_.pop( tok_identifier );
        list_.pop(type_prefix);
        llb_token_t type = list_.pop( tok_identifier );
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

void llb_parser_t::parse_type() {


}

void llb_parser_t::parse_field() {
}

void llb_parser_t::parse_external() {

    list_.pop(tok_key_external);
    parse_function_decl();
}

void llb_parser_t::parse_module() {

    std::unique_ptr<pt_module_t> module( new pt_module_t );
    assert( module.get() );

    module->name_ = module_->name_;

    uint32_t ix = pt_.index();

    while (!list_.at_eof()) {

        llb_token_t &tok = list_.peek(0);
        switch (tok.type_) {
            case (tok_key_global): {
                uint32_t tide = pt_.index();
                parse_var_decl();
                while (pt_.index() > tide)
                    module->add_global(pt_.pop_specific<pt_decl_var_t>());
                }
                break;

            case (tok_key_external):
                parse_external();
                module->add_function(pt_.pop_specific<pt_function_decl_t>());
                break;

            case (tok_key_type):
                parse_type();
                break;

            case (tok_key_function) :
                parse_function();
                module->add_function(pt_.pop_specific<pt_function_decl_t>());
                break;

            case (tok_eol):
                list_.pop(tok_eol);
                break;

            default:
                fail("unexpected token", tok);
        }
    }

    assert(pt_.index() == ix);
    pt_.push( module.release() );
}

void llb_parser_t::fail(std::string str, const pt_node_t & node) {

    llb_location_t pos = node.get_location();
    throw llb_fail_t(str, pos.line_, pos.column_);
}

void llb_parser_t::fail(std::string str, const llb_token_t & tok) {

    throw llb_fail_t(str, llb_token_t(tok));
}

bool llb_parser_t::run(llb_fail_t & error) {

    try {
        parse_module();
    }
    catch (llb_fail_t thrown) {

        error = thrown;
        return false;
    }

    return true;
}

llb_parser_t::llb_parser_t(shared_llb_module_t module, pt_t & ast)
    : module_(module)
    , list_(*module->tokens_.get())
    , pt_(ast)
{
}
