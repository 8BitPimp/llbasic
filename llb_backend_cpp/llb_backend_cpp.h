#pragma once

#include <stdio.h>

#include "llb_pt_walker.h"
#include "llb_pt.h"
#include "llb_string.h"

class llb_backend_cpp_t
    : public pt_walker_t {
public:
    llb_backend_cpp_t(const std::string & path);

    virtual void visit(pt_module_t & n);
    virtual void visit(pt_literal_t & n);
    virtual void visit(pt_identifier_t & n);
    virtual void visit(pt_function_decl_t & n);
    virtual void visit(pt_function_body_t & n);
    virtual void visit(pt_decl_var_t & n);
    virtual void visit(pt_op_bin_t & n);
    virtual void visit(pt_op_ury_t & n);
    virtual void visit(pt_if_t & n);
    virtual void visit(pt_while_t & n);
    virtual void visit(pt_return_t & n);
    virtual void visit(pt_break_t & n);
    virtual void visit(pt_continue_t & n);
    virtual void visit(pt_call_t & n);
    virtual void visit(pt_expr_t & n);

protected:
    std::string path_;
    llb_stream_t out_;
};
