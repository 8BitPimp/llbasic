#include "llb_string.h"
#include "llb_pass_printer.h"
#include "llb_module.h"
#include "llb_context.h"
#include "llb_file.h"

llb_pass_printer_t::llb_pass_printer_t()
    : path_("ast.txt")
    , stream_()
{
}

void llb_pass_printer_t::indent() {
    
    for (uint32_t i = 0; i<stack_.size(); i++)
        stream_.print(". ");
}

bool llb_pass_printer_t::run(llb_context_t & modules, llb_fail_t & fail) {

    try {
        modules.pt_.visit(*this);
    }
    catch (llb_fail_t & thrown) {
        fail = thrown;
        return false;
    }

    file_writer_t out;
    if (out.open(path_.c_str())) {
        out.write(stream_.to_string());
    }

    return true;
}

void llb_pass_printer_t::visit(pt_module_t & n) {
    indent();
    stream_.println("%0: %1", { n.get_type().name(), n.name_ } );
    llb_pt_walker_t::visit(n);
}

void llb_pass_printer_t::visit(pt_literal_t & n) {
    indent();

    std::string msg;

    switch (n.value_.type_) {
    case (tok_lit_float):
        stream_.println( "%0: %1", { n.get_type().name(), n.value_.get_float() });
        break;

    case (tok_lit_integer):
        stream_.println( "%0: %1", { n.get_type().name(), n.value_.get_int() });
        break;

    case (tok_lit_string):
        stream_.println( "%0: %1", { n.get_type().name(), n.value_.get_string() });
        break;
    }

    llb_pt_walker_t::visit(n);
}

void llb_pass_printer_t::visit(pt_identifier_t & n) {
    indent();
    stream_.println( "%0: %1", { n.get_type().name(), n.name_.get_string() });
    llb_pt_walker_t::visit(n);
}

void llb_pass_printer_t::visit(pt_function_decl_t & n) {
    indent();
    stream_.println( "%0: %1 %2", { n.get_type().name(), n.ret_type_.get_string(), n.name_.get_string() });
    llb_pt_walker_t::visit(n);
}

void llb_pass_printer_t::visit(pt_function_body_t & n) {
    indent();
    stream_.println("%0", { n.get_type().name() });
    llb_pt_walker_t::visit(n);
}

void llb_pass_printer_t::visit(pt_decl_var_t & n) {
    indent();
    stream_.println( "%0: %1 %2", { n.get_type().name(), n.type_.get_string(), n.name_.get_string() });
    llb_pt_walker_t::visit(n);
}

void llb_pass_printer_t::visit(pt_op_bin_t & n) {
    indent();
    stream_.println( "%0: %1", { n.get_type().name(), llb_token_t::get_type_symbol(n.operator_.type_) });
    llb_pt_walker_t::visit(n);
}

void llb_pass_printer_t::visit(pt_op_ury_t & n) {
    indent();
    stream_.println( "%0: %1", { n.get_type().name(), llb_token_t::get_type_symbol( n.operator_.type_ ) } );
    llb_pt_walker_t::visit(n);
}

void llb_pass_printer_t::visit(pt_if_t & n) {
    indent();
    stream_.println("%0", { n.get_type().name() } );
    llb_pt_walker_t::visit(n);
}

void llb_pass_printer_t::visit(pt_while_t & n) {
    indent();
    stream_.println("%0", { n.get_type().name() });
    llb_pt_walker_t::visit(n);
}

void llb_pass_printer_t::visit(pt_return_t & n) {
    indent();
    stream_.println("%0", { n.get_type().name() });
    llb_pt_walker_t::visit(n);
}

void llb_pass_printer_t::visit(pt_break_t & n) {
    indent();
    stream_.println("%0", { n.get_type().name() });
    llb_pt_walker_t::visit(n);
}

void llb_pass_printer_t::visit(pt_continue_t & n) {
    indent();
    stream_.println("%0", { n.get_type().name() });
    llb_pt_walker_t::visit(n);
}

void llb_pass_printer_t::visit(pt_call_t & n) {
    indent();
    stream_.println( "%0: %1", { n.get_type().name(), n.name_.get_string() });
    llb_pt_walker_t::visit(n);
}

void llb_pass_printer_t::visit(pt_expr_t & n) {
    indent();
    stream_.println("%0", { n.get_type().name() });
    llb_pt_walker_t::visit(n);
}

void llb_pass_printer_t::visit(pt_stmt_t & n) {
    indent();
    stream_.println("%0", { n.get_type().name() });
    llb_pt_walker_t::visit(n);
}
