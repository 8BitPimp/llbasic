#pragma once

#include <vector>

#include "llb_forward.h"
#include "llb_pass_types.h"

struct llb_pass_t {

    virtual void get_dependant_passes(llb_pass_manager_t & manager) {
    }

    virtual bool run( llb_context_t & modules,
                      llb_fail_t & fail) = 0;
};

struct llb_pass_creator_t {

    virtual llb_pass_type_t get_type() const = 0;
    virtual void create(std::unique_ptr<llb_pass_t> & out) const = 0;
};

struct llb_pass_manager_t {

    void schedule(llb_pass_type_t type);

    void register_defaults();

    bool run( llb_context_t & cxt );

    void register_pass(const llb_pass_creator_t *);

protected:
    const llb_pass_creator_t * find_creator(llb_pass_type_t);

    std::vector<const llb_pass_creator_t*> register_;
    std::vector<llb_pass_type_t> schedule_;
};
