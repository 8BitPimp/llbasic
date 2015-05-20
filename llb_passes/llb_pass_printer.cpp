#include "llb_string.h"
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

    std::string msg;

    switch (n.value_.type_) {
    case (tok_lit_float):
        msg = llb_string_t::format(
        "%0: %1", {
            n.get_type().name(),
            n.value_.get_float() });
        break;

    case (tok_lit_integer):
        msg = llb_string_t::format(
        "%0: %1", {
            n.get_type().name(),
            n.value_.get_int() });
        break;

    case (tok_lit_string):
        msg = llb_string_t::format(
        "%0: %1", {
            n.get_type().name(),
            n.value_.get_string() });
        break;
    }

    printf("%s\n", msg.c_str());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_identifier_t & n) {
    indent();

    std::string msg = llb_string_t::format(
        "%0: %1", {
            n.get_type().name(),
            n.name_.get_string() });

    printf("%s\n", msg.c_str());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_decl_function_t & n) {
    indent();

    std::string msg = llb_string_t::format(
        "%0: %1 %2", {
            n.get_type().name(),
            n.ret_type_.get_string(),
            n.name_.get_string() });

    printf("%s\n", msg.c_str());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_decl_var_t & n) {
    indent();

    std::string msg = llb_string_t::format(
        "%0: %1 %2", {
            n.get_type().name(),
            n.type_.get_string(),
            n.name_.get_string() });

    printf("%s\n", msg.c_str());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_op_bin_t & n) {
    indent();
    std::string msg = llb_string_t::format(
        "%0: %1", {
            n.get_type().name(),
            token_t::get_type_symbol(n.operator_.type_) });

    printf("%s\n", msg.c_str());
    pt_walker_t::visit(n);
}

void pt_pass_printer_t::visit(pt_op_ury_t & n) {
    indent();

    std::string msg = llb_string_t::format(
        "%0: %1", {
        n.get_type().name(),
        token_t::get_type_symbol( n.operator_.type_ ) } );

    printf("%s\n", msg.c_str());
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

void pt_pass_printer_t::visit(pt_call_t & n) {
    indent();

    std::string msg = llb_string_t::format(
        "%0: %1", {
            n.get_type().name(),
            n.name_.get_string() });

    printf("%s\n", msg.c_str());
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
