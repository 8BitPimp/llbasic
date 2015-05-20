#include "llb_backend_cpp.h"

void emit_whitespace() {
    printf(" ");
}

void emit_token_string( const token_t & node ) {

    if (node.type_ == token_type_t::tok_identifier) {
        if (node.get_string().empty())
            throw "empty identifier";
        printf("%s", node.get_string().c_str());
    }
    emit_whitespace();
}

void emit_datatype( const token_t & type ) {
    
    if (type.type_ != tok_identifier)
        throw "invalid type";

    printf("%s", type.get_string().c_str());

    emit_whitespace();
}

void emit_new_line() {
    printf("\n");
}

void emit_function_proto( pt_decl_function_t & n ) {

    emit_datatype(n.ret_type_);
    emit_token_string(n.name_);
    printf("( ");

    bool first = true;

    for (auto & i : n.args_) {

        if (!first)
            printf(", ");
        first = false;

        pt_decl_var_t * arg = i->upcast<pt_decl_var_t>();
        if (arg == nullptr)
            assert(!"internal error");
        emit_datatype(arg->type_);
        emit_token_string(arg->name_);
    }

    printf(")");
}

void llb_backend_cpp_t::visit(pt_module_t & n) {
    pt_walker_t::visit(n);
}

void llb_backend_cpp_t::visit(pt_literal_t & n) {
    pt_walker_t::visit(n);
}

void llb_backend_cpp_t::visit(pt_identifier_t & n) {
    pt_walker_t::visit(n);
}

void llb_backend_cpp_t::visit(pt_decl_function_t & n) {

    emit_function_proto(n);
    printf(";");
    emit_new_line();
    pt_walker_t::visit(n);
}

void llb_backend_cpp_t::visit(pt_decl_var_t & n) {
    pt_walker_t::visit(n);
}

void llb_backend_cpp_t::visit(pt_op_bin_t & n) {
    pt_walker_t::visit(n);
}

void llb_backend_cpp_t::visit(pt_op_ury_t & n) {
    pt_walker_t::visit(n);
}

void llb_backend_cpp_t::visit(pt_if_t & n) {
    pt_walker_t::visit(n);
}

void llb_backend_cpp_t::visit(pt_while_t & n) {
    pt_walker_t::visit(n);
}

void llb_backend_cpp_t::visit(pt_return_t & n) {
    pt_walker_t::visit(n);
}

void llb_backend_cpp_t::visit(pt_break_t & n) {
    pt_walker_t::visit(n);
}

void llb_backend_cpp_t::visit(pt_continue_t & n) {
    pt_walker_t::visit(n);
}

#if 0
void llb_backend_cpp_t::visit(pt_assign_t & n) {
    pt_walker_t::visit(n);
}
#endif

void llb_backend_cpp_t::visit(pt_call_t & n) {
    pt_walker_t::visit(n);
}

void llb_backend_cpp_t::visit(pt_expr_t & n) {
    pt_walker_t::visit(n);
}
