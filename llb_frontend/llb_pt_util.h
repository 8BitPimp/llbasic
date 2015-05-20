#pragma once

#include "llb_pt.h"

class pt_util_t {
protected:
    pt_t & pt_;

public:

    pt_util_t(pt_t & pt)
        : pt_(pt)
    {
    }

    pt_module_t * get_module() {
        pt_module_t * mod = pt_.top()->upcast<pt_module_t>();
    }

    pt_decl_function_t * get_function(const std::string & name) {
        pt_module_t * mod = get_module();
        if (!mod)
            return false;
        for (auto & i : mod->functions_) {

            pt_decl_function_t * func = i->upcast<pt_decl_function_t>();
            if (!func)
                continue;

            if (func->name_.type_ != tok_identifier)
                continue;

            if (name == func->name_.get_string())
                return func;
        }
        return nullptr;
    }

    pt_decl_var_t * get_global(const std::string & name) {
        return nullptr;
    }

};
