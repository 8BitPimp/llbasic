#include "llb_pass_hoist_var.h"
#include "llb_context.h"

bool llb_pass_hoist_var_t::run(llb_context_t & cxt, llb_fail_t & fail) {

    try {
        cxt.pt_.visit(*this);
    }
    catch (llb_fail_t & thrown) {
        fail = thrown;
        return false;
    }
    return true;
}

void llb_pass_hoist_var_t::visit(pt_function_body_t & n) {

    for (shared_pt_node_t & node : n.stmt_) {

        if (node->is_a<pt_decl_var_t>()) {

            pt_decl_var_t * decl = node->upcast<pt_decl_var_t>();
            assert(decl != nullptr);

            // add decl to the functions locals
            n.add_local(node);

            // create node to encapsulate the assignment
            shared_pt_node_t ident(new pt_identifier_t(decl->name_));
            llb_token_t tok = llb_token_t(llb_token_type_t::tok_chr_assign);
            shared_pt_node_t op(new pt_op_bin_t(tok, ident, decl->expr_));
            shared_pt_node_t expr(new pt_expr_t(op));
            shared_pt_node_t stmt(new pt_stmt_t(expr));

            // swap decl for an assignment operation
            node = stmt;
        }
    }
}
