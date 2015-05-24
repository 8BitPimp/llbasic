#pragma once

#include <stdio.h>

#include "llb_string.h"
#include "llb_pt.h"
#include "llb_pass.h"
#include "llb_pt_walker.h"

class llb_pass_printer_t
        : public llb_pass_t
        , public llb_pt_walker_t {
protected:
    void indent();
    std::string path_;
    llb_stream_t stream_;

public:
    llb_pass_printer_t();

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

struct llb_creator_pass_printer_t
    : public llb_pass_creator_t {

    virtual llb_pass_type_t get_type() const override {
        return llb_pass_type_t::e_pass_printer;
    }

    virtual void create(std::unique_ptr<llb_pass_t> & out) const {
        out.reset(new llb_pass_printer_t());
    };
};
