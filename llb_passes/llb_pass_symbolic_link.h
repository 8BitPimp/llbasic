#pragma once

#include <stdio.h>

#include "llb_pt.h"
#include "llb_pass.h"
#include "llb_pt_walker.h"

class pt_pass_symbolic_link_t
        : public pt_pass_t
        , public pt_walker_t {
protected:
    std::vector<shared_pt_node_t> types_;
    std::vector<shared_pt_node_t> functions_;
    std::vector<shared_pt_node_t> globals_;
    std::vector<shared_pt_node_t> locals_;

    void add_type(shared_pt_node_t type) {
        // not ready yet ...
    }

    void add_function(shared_pt_node_t func) {

        if (!func->is_a<pt_function_decl_t>())
            assert(!"expecting function");
        // name of new function
        token_t iname = func->upcast<pt_function_decl_t>()->name_;
        // check for name collision
        for (auto &i : functions_) {
            token_t fname = i->upcast<pt_function_decl_t>()->name_;            
            if (iname.get_string() == fname.get_string())
                throw llb_fail_t("function redefined", iname);
        }
        // push back new function
        functions_.push_back(func);
    }

    void add_global(shared_pt_node_t global) {

        if (!global->is_a<pt_decl_var_t>())
            assert(!"expecting function");
        // name of new function
        token_t iname = global->upcast<pt_decl_var_t>()->name_;
        // check for name collision
        for (auto &i : globals_) {
            token_t fname = i->upcast<pt_decl_var_t>()->name_;
            if (iname.get_string() == fname.get_string())
                throw llb_fail_t("global redefined", iname);
        }
        // push back new global
        globals_.push_back(global);
    }

    void add_local(shared_pt_node_t local) {
    }

public:
    virtual bool run(llb_context_t & modules, llb_fail_t & fail) {

        try {
            modules.pt_.visit(*this);
        }
        catch (llb_fail_t & thrown) {
            fail = thrown;
            return false;
        }
        return true;
    }

    virtual void visit(pt_t & n) {

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
    virtual void visit(pt_module_t & n) {
        // traverse all functions
        for (auto &i : n.functions_)
            i->accept(*this);
    }

    virtual void visit(pt_function_decl_t & n) {
        // check args dont collide with globals
        for (auto &i : n.args_) {
        }
        // walk statments
        // ...
    }

    virtual void visit(pt_identifier_t & n) {

        // lookup
    }
    virtual void visit(pt_decl_var_t & n) {
    
        // add to scope
    }
    virtual void visit(pt_call_t & n) {
    
        // check name in function list
    }
};
