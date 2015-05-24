#include "llb_backend_cpp.h"

llb_backend_cpp_t::llb_backend_cpp_t(const std::string & path)
    : path_(path)
    , out_()
{
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

void llb_backend_cpp_t::visit(pt_function_decl_t & n) {

}

void llb_backend_cpp_t::visit(pt_function_body_t & n) {

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

void llb_backend_cpp_t::visit(pt_call_t & n) {
    pt_walker_t::visit(n);
}

void llb_backend_cpp_t::visit(pt_expr_t & n) {
    pt_walker_t::visit(n);
}
