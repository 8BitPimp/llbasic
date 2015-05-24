#pragma once

#include <stdio.h>

#include "llb_forward.h"
#include "llb_pt.h"
#include "llb_pass.h"
#include "llb_pt_walker.h"

class llb_pass_hoist_var_t
    : public llb_pass_t
    , public llb_pt_walker_t {
public:
    virtual bool run(llb_context_t & modules, llb_fail_t & fail);
    virtual void visit(pt_function_body_t & n);
};

struct llb_creator_pass_hoist_var_t
    : public llb_pass_creator_t {

    virtual llb_pass_type_t get_type() const override {
        return llb_pass_type_t::e_pass_hoist_var;
    }

    virtual void create(std::unique_ptr<llb_pass_t> & out) const {
        out.reset(new llb_pass_hoist_var_t());
    };
};
