#pragma once

enum class llb_pass_type_t {

    // 
    e_pass_backend_cpp,
    
    // 
    e_pass_printer,

    // hoist type info
    e_pass_hoist_types,

    // hoist variable declarations out of statments and into
    // function and module definitions
    e_pass_hoist_var,

    // symbolicaly link multiple modules together
    e_pass_linker,
};
