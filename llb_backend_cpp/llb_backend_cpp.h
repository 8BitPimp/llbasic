#pragma once

#include <stdio.h>
#include <functional>

#include "llb_forward.h"
#include "llb_pass.h"
#include "llb_string.h"
#include "llb_pt_walker.h"

class llb_backend_cpp_t
    : public llb_pass_t
    , public llb_pt_walker_t {
protected:
    std::string path_;
    llb_stream_t out_;
    llb_context_t * cxt_;
    uint32_t indent_;

    void emit_header();

    void emit_decl (pt_function_decl_t & func);
    void emit_decls(pt_module_t & module);

    void emit_global (pt_decl_var_t & global);
    void emit_globals(pt_module_t & module);

    void emit_locals(pt_function_body_t & body);

    void emit_body(pt_function_body_t & body);
    void emit_impls(pt_function_decl_t & func);
    void emit_impls(pt_module_t & module);

    void for_each_module(std::function<void(pt_module_t &)> func);

    void visit(pt_op_bin_t & n);
    void visit(pt_identifier_t & n);
    void visit(pt_literal_t & n);

    void visit(pt_stmt_t & stmt);
    void visit(pt_return_t & n);

public:
    llb_backend_cpp_t();

    virtual void get_dependant_passes(llb_pass_manager_t & manager);
    virtual bool run(llb_context_t & modules, llb_fail_t & fail);
};

struct llb_creator_backend_cpp_t
    : public llb_pass_creator_t {

    virtual llb_pass_type_t get_type() const override {
        return llb_pass_type_t::e_pass_backend_cpp;
    }

    virtual void create(std::unique_ptr<llb_pass_t> & out) const override {
        out.reset(new llb_backend_cpp_t());
    }
};

