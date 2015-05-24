#pragma once

#include "llb_pt.h"

class pt_walker_t
    : public pt_node_visitor_t {
protected:
    std::vector<pt_node_t*> stack_;

    pt_module_t * get_module( ) {
        if ( stack_.size() <= 0 )
            return nullptr;
        pt_node_t * mod = stack_[0];
        assert( mod != nullptr );
        return mod->upcast<pt_module_t>();
    }

    pt_function_decl_t * get_function( ) {
        if ( stack_.size() <= 1 )
            return nullptr;
        pt_node_t * func = stack_[1];
        assert( func != nullptr );
        return func->upcast<pt_function_decl_t>();
    }

protected:
    void enter( pt_node_t & node ) {
        stack_.push_back(&node);
    }

    void leave( ) {
        stack_.pop_back();
    }

public:
    virtual void visit( pt_t & n ) {
        for (auto & i : n.stack_)
            i->accept(*this);
    }

    virtual void visit( pt_module_t & n ) {
        enter( n );
        for (auto & i : n.globals_)
            i->accept(*this);
        for (auto & i : n.functions_)
            i->accept(*this);
        leave();
    }

#if 1
    virtual void visit( pt_literal_t & n ) {
        enter( n );
        leave();
    }

    virtual void visit( pt_identifier_t & n ) {
        enter( n );
        leave();
    }
#endif

    virtual void visit( pt_function_decl_t & n ) {
        enter( n );
        for (auto & i : n.args_)
            i->accept(*this);
        if (n.body_)
            n.body_->accept(*this);
        leave();
    }

    virtual void visit(pt_function_body_t & n) {
        enter(n);
        for (auto & i : n.stmt_)
            i->accept(*this);
        leave();
    }

    virtual void visit( pt_decl_var_t & n ) {
        enter( n );
        if (n.expr_.get())
            n.expr_->accept(*this);
        leave();
    }

    virtual void visit( pt_op_bin_t & n ) {
        enter( n );
        if (n.lhs_)
            n.lhs_->accept(*this);
        if (n.rhs_)
            n.rhs_->accept(*this);
        leave();
    }

    virtual void visit( pt_op_ury_t & n ) {
        enter( n );
        if (n.child_)
            n.child_->accept(*this);
        leave();
    }

    virtual void visit( pt_if_t & n ) {
        enter( n );
        if (n.expr_)
            n.expr_->accept(*this);
        for (auto & i : n.stmt_1_)
            i->accept(*this);
        for (auto & i : n.stmt_0_)
            i->accept(*this);
        leave();
    }

    virtual void visit( pt_while_t & n ) {
        enter( n );
        if (n.expr_)
            n.expr_->accept(*this);
        for (auto & i : n.stmt_)
            i->accept(*this);
        leave();
    }

    virtual void visit( pt_return_t & n ) {
        enter( n );
        if (n.expr_)
            n.expr_->accept(*this);
        leave();
    }

#if 1
    virtual void visit( pt_break_t & n ) {
        enter( n );
        leave();
    }

    virtual void visit( pt_continue_t & n ) {
        enter( n );
        leave();
    }
#endif

#if 0
    virtual void visit( pt_assign_t & n ) {
        enter( n );
        if (n.expr_)
            n.expr_->accept(*this);
        leave();
    }
#endif

    virtual void visit( pt_call_t & n ) {
        enter( n );
        for (auto & i : n.arg_)
            i->accept(*this);
        leave();
    }

    virtual void visit( pt_expr_t & n ) {
        enter( n );
        if (n.expr_)
            n.expr_->accept(*this);
        leave();
    }

    virtual void visit(pt_stmt_t & n) {
        enter(n);
        if (n.child_)
            n.child_->accept(*this);
        leave();
    }
};
