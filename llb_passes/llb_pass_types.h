#pragma once

enum llb_pass_type_t {

    e_pass_printer,

    // hoist variable declarations out of statments and into
    // function and module definitions
    e_pass_hoist_var,

    // symbolicaly link multiple modules together
    e_pass_symbolic_link,
};
