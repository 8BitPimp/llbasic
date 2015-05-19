#include "llb_pass_printer.h"

void pt_pass_printer_t::indent() {
    for (uint32_t i = 0; i<stack_.size(); i++)
        printf(". ");
}

void pt_pass_printer_t::visit(pt_module_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_literal_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_identifier_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_decl_function_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_decl_var_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_op_bin_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_op_ury_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_if_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_while_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_return_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_break_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_continue_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}

#if 0
void pt_pass_printer_t::visit(pt_assign_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}
#endif

void pt_pass_printer_t::visit(pt_call_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_expr_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_stmt_t & n) {
    indent();
    printf("%s\n", n.get_type().name());
    pt_walker_t::visit(n);
}
