#pragma once

#include <stdio.h>

#include "llb_forward.h"
#include "llb_pt.h"
#include "llb_pass.h"
#include "llb_pt_walker.h"

class llb_pass_linker_t
        : public llb_pass_t
        , public llb_pt_walker_t {
protected:
    std::vector<shared_pt_node_t> types_;
    std::vector<shared_pt_node_t> functions_;
    std::vector<shared_pt_node_t> globals_;
    std::vector<shared_pt_node_t> locals_;

    void add_type(shared_pt_node_t type);
    void add_function(shared_pt_node_t func);
    void add_global(shared_pt_node_t global);
    void add_local(shared_pt_node_t local);

public:
    virtual bool run(llb_context_t & modules, llb_fail_t & fail);
    virtual void visit(pt_t & n);
    virtual void visit(pt_module_t & n);
    virtual void visit(pt_function_decl_t & n);
    virtual void visit(pt_identifier_t & n);
    virtual void visit(pt_decl_var_t & n);
    virtual void visit(pt_call_t & n);
};

struct llb_creator_pass_linker_t
    : public llb_pass_creator_t {

    virtual llb_pass_type_t get_type() const override {
        return llb_pass_type_t::e_pass_linker;
    }

    virtual void create(std::unique_ptr<llb_pass_t> & out) const {
        out.reset(new llb_pass_linker_t());
    };
};
