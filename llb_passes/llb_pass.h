#pragma once

#include "llb_forward.h"
#include "llb_pass_types.h"

struct pt_pass_t {

    virtual void get_dependant_passes(pt_pass_manager_t & manager) {
    }

    virtual bool run( llb_context_t & modules,
                      llb_fail_t & fail) = 0;
};

struct pt_pass_manager_t {

    void schedule(llb_pass_type_t type);

    void run();

protected:
    std::vector<llb_pass_type_t> schedule_;
};
