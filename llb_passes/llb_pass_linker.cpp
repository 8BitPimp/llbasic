#include <stdio.h>

#include "llb_pt.h"
#include "llb_pass.h"
#include "llb_pt_walker.h"
#include "llb_module.h"
#include "llb_context.h"

#include "llb_pass_linker.h"

void llb_pass_linker_t::add_type(shared_pt_node_t type) {
    // not ready yet ...
}

void llb_pass_linker_t::add_function(shared_pt_node_t func) {

    if (!func->is_a<pt_function_decl_t>())
        assert(!"expecting function");
    // name of new function
    llb_token_t iname = func->upcast<pt_function_decl_t>()->name_;
    // check for name collision
    for (auto &i : functions_) {
        llb_token_t fname = i->upcast<pt_function_decl_t>()->name_;
        if (iname.get_string() == fname.get_string())
            throw llb_fail_t("function redefined", iname);
    }
    // push back new function
    functions_.push_back(func);
}

void llb_pass_linker_t::add_global(shared_pt_node_t global) {

    if (!global->is_a<pt_decl_var_t>())
        assert(!"expecting function");
    // name of new function
    llb_token_t iname = global->upcast<pt_decl_var_t>()->name_;
    // check for name collision
    for (auto &i : globals_) {
        llb_token_t fname = i->upcast<pt_decl_var_t>()->name_;
        if (iname.get_string() == fname.get_string())
            throw llb_fail_t("global redefined", iname);
    }
    // push back new global
    globals_.push_back(global);
}

void llb_pass_linker_t::add_local(shared_pt_node_t local) {
}

bool llb_pass_linker_t::run(llb_context_t & modules, llb_fail_t & fail) {

    try {
        modules.pt_.visit(*this);
    }
    catch (llb_fail_t & thrown) {
        fail = thrown;
        return false;
    }
    return true;
}

void llb_pass_linker_t::visit(pt_t & n) {

    // collect all functions, globals and types
    for (auto & i : n.stack_) {
        pt_module_t * module = i->upcast<pt_module_t>();
        if (!module)
            assert(!"expecting module");
        for (auto &i : module->functions_)
            add_function(i);
        for (auto &i : module->globals_)
            add_global(i);
    }

    // traverse all modules
    for (auto & i : n.stack_) {
        i->accept(*this);
    }
}

void llb_pass_linker_t::visit(pt_module_t & n) {
    // traverse all functions
    for (auto &i : n.functions_)
        i->accept(*this);
}

void llb_pass_linker_t::visit(pt_function_decl_t & n) {
    // check args dont collide with globals
    for (auto &i : n.args_) {
    }
    // walk statments
    // ...
}

void llb_pass_linker_t::visit(pt_identifier_t & n) {

    // lookup
}

void llb_pass_linker_t::visit(pt_decl_var_t & n) {

    // add to scope
}

void llb_pass_linker_t::visit(pt_call_t & n) {

    // check name in function list
}
