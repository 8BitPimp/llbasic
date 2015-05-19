#include <vector>

#include "llb_parser.h"
#include "llb_token.h"
#include "llb_pt.h"

struct token_stack_t {

    void push( token_t & tok ) {
        stack_.push_back( tok );
    }

    token_t top( ) {
        assert( stack_.size() > 0 );
        return *stack_.rbegin();
    }

    token_t pop( ) {
        assert( stack_.size() > 0 );
        token_t tok = *stack_.rbegin();
        stack_.pop_back();
        return tok;
    }

    void clear() {
        stack_.clear();
    }

    bool empty( ) const {
        return stack_.size() == 0;
    }

    uint32_t get_mark( ) const {
        return uint32_t(stack_.size());
    }

    std::vector<token_t> stack_;
};

struct expr_info_t {

    void clear( ) {
        ops_.clear();
    }

    token_stack_t ops_;
};

namespace {

    int32_t precedence(token_type_t type) {

        switch (type) {

            case (tok_chr_dot) :
                return 0;

            case (tok_chr_assign) :
                return 1;

            case (tok_op_add):
            case (tok_op_sub):
                return 2;

            case (tok_op_mul):
            case (tok_op_div):
            case (tok_op_mod):
                return 3;

            case (tok_op_shl):
            case (tok_op_shr):
                return 4;

            case (tok_op_lt):
            case (tok_op_gt):
            case (tok_op_equ):
            case (tok_op_nequ):
            case (tok_op_ltequ):
            case (tok_op_gtequ):
                return 5;

            case (tok_op_and):
            case (tok_op_or):
                return 6;

            default:
                return -1;
        }
    }

    bool is_operator(token_type_t type) {

        return precedence(type) > -1;
    }

} // namespace {}

void parser_t::parse_expr_lhs( expr_info_t & expr ) {

    bool unary_op = false;
    token_t unary = list_.peek(0);

    switch (list_.peek(0).type_) {
    case ( tok_ury_neg ):
    case ( tok_ury_not ):
#if 0
    case ( tok_key_new ):
#endif
        unary_op = true;
        list_.pop();
    default:
        ; // not a unary operator
    }

    switch (list_.peek(0).type_) {
    case ( tok_chr_paren_l ): {
        list_.pop(tok_chr_paren_l);
        parse_expr();
        shared_pt_node_t expr = pt_.pop();
        assert(expr->is_a<pt_expr_t>());
        pt_.push(expr->upcast<pt_expr_t>()->expr_);
        list_.pop(tok_chr_paren_r);
        }
        break;
#if 0
    case ( tok_chr_brace_l ):
        list_.pop( tok_chr_brace_l );
        parse_expr( );
        pt_.push( new pt_index_t( list_.pop() ));
        list_.pop( tok_chr_brace_r );
#endif
    case ( tok_lit_float ):
    case ( tok_lit_integer ):
    case ( tok_lit_string ):
        pt_.push( new pt_literal_t( list_.pop() ));
        break;

    case ( tok_identifier ):
        if ( list_.peek(1).type_ == tok_chr_paren_l ) {
            parse_stmt_call();
        }
        else {
            pt_.push( new pt_identifier_t( list_.pop() ));
        }
        break;

    default:
        fail("expecting expression", list_.peek(0));
    }

    if ( unary_op ) {
        pt_.push( new pt_op_ury_t( unary, pt_.pop() ) );
    }
}

void parser_t::parse_expr_reduce( expr_info_t & expr, int32_t prec ) {

    while (! expr.ops_.empty() ) {

        if (precedence(expr.ops_.top().type_) < prec)
            break;

        token_t op = expr.ops_.pop();

        shared_pt_node_t rhs = pt_.pop();
        shared_pt_node_t lhs = pt_.pop();

        pt_.push( new pt_op_bin_t( op, lhs, rhs ) );
    }
}

void parser_t::parse_expr_inner( expr_info_t & expr ) {

    parse_expr_lhs( expr );

    if (! is_operator( list_.peek(0).type_ ))
        return;
    token_t & op = list_.pop();

    enum action_t {
        e_shift,
        e_reduce,
    }
    action = e_shift;

    // precedence is lower then stack top? must reduce
    if (! expr.ops_.empty() )
        if (precedence(op.type_) < precedence(expr.ops_.top().type_))
            action = e_reduce;

    // optional reduce
    if ( action == e_reduce )
        parse_expr_reduce( expr, precedence(op.type_) );

    // always shift
    expr.ops_.push(op);

    // parse expression RHS
    parse_expr_inner( expr );
}

void parser_t::parse_expr() {

    expr_info_t expr;
    parse_expr_inner( expr );

    // emit whatever is left over
    parse_expr_reduce( expr, 0 );

    pt_.push( new pt_expr_t( pt_.pop() ) );
}
