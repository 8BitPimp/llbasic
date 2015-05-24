#pragma once

#include <stdio.h>

#include "llb_string.h"
#include "llb_pt.h"
#include "llb_pass.h"
#include "llb_pt_walker.h"

class pt_pass_printer_t
        : public pt_pass_t
        , public pt_walker_t {
protected:
    void indent();
    std::string path_;
    llb_stream_t stream_;

public:
    pt_pass_printer_t(std::string path);

    virtual bool run(llb_context_t & modules, llb_fail_t & fail);

    virtual void visit(pt_module_t & n);

    virtual void visit(pt_function_decl_t & n);
    virtual void visit(pt_function_body_t & n);

    virtual void visit(pt_decl_var_t & n);

    virtual void visit(pt_expr_t & n);
    virtual void visit(pt_op_bin_t & n);
    virtual void visit(pt_op_ury_t & n);
    virtual void visit(pt_literal_t & n);
    virtual void visit(pt_identifier_t & n);

    virtual void visit(pt_if_t & n);
    virtual void visit(pt_while_t & n);
    virtual void visit(pt_return_t & n);
    virtual void visit(pt_break_t & n);
    virtual void visit(pt_continue_t & n);
    virtual void visit(pt_call_t & n);
    virtual void visit(pt_stmt_t & n);
};
