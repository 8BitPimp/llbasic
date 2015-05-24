#include "llb_pass.h"

#include "llb_pass_printer.h"
#include "llb_pass_linker.h"
#include "llb_pass_hoist_var.h"
#include "llb_context.h"

void llb_pass_manager_t::schedule(llb_pass_type_t type) {

    // check we dont have dependancy cycle
    // raise existing dependancies earlier


    schedule_.push_back(type);
}

bool llb_pass_manager_t::run( llb_context_t & cxt ) {

    std::vector<llb_pass_type_t> retired;

    for (llb_pass_type_t type : schedule_) {

        for (llb_pass_type_t x : retired) {
            if (type == x)
                assert(!"pass dependancy loop");
        }

        retired.push_back(type);

        const llb_pass_creator_t * creator = find_creator(type);
        if (!creator) {
            assert(!"unknown pass");
        }

        std::unique_ptr<llb_pass_t> pass;
        creator->create(pass);
        if (!pass.get()) {
            assert(!"pass creation failed");
        }
        pass->run( cxt, llb_fail_t() );
    }

    return true;
}

const llb_pass_creator_t * llb_pass_manager_t::find_creator(llb_pass_type_t type) {

    for (const llb_pass_creator_t* creator : register_) {
        if (creator->get_type() == type)
            return creator;
    }
    return nullptr;
}

void llb_pass_manager_t::register_pass(const llb_pass_creator_t *reg) {
    register_.push_back(reg);
}

void llb_pass_manager_t::register_defaults() {

    register_pass(new llb_creator_pass_linker_t);
    register_pass(new llb_creator_pass_printer_t);
    register_pass(new llb_creator_pass_hoist_var_t);
}
