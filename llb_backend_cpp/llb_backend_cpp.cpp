#include "llb_backend_cpp.h"

void emit_whitespace() {
    printf(" ");
}

void emit_token_string( token_t & node ) {

    if (node.type_ == token_type_t::tok_identifier) {
        if (node.value_.string_.empty())
            throw "empty identifier";
        printf("%s", node.value_.string_.c_str());
    }
    emit_whitespace();
}

void emit_datatype( token_type_t type ) {
    switch (type) {
    case(tok_key_none) :
        printf("void");
        break;
    case (tok_key_int):
        printf("int");
        break;
    case (tok_key_float):
        printf("int");
        break;
    case (tok_key_string):
        printf("string");
        break;
    default:
        throw "unknown data type";
    }
    emit_whitespace();
}

void emit_new_line() {
    printf("\n");
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

    emit_datatype(n.ret_type_.type_);
    emit_token_string(n.name_);
    printf("(");

    for (auto & i : n.args_) {

        
    }

    printf(") {");
    emit_new_line();
    printf("}");
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

void llb_backend_cpp_t::visit(pt_assign_t & n) {
    pt_walker_t::visit(n);
}

void llb_backend_cpp_t::visit(pt_call_t & n) {
    pt_walker_t::visit(n);
}

void llb_backend_cpp_t::visit(pt_expr_t & n) {
    pt_walker_t::visit(n);
}
