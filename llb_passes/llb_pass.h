#pragma once

class  pt_t;
class  lexer_t;
struct llb_fail_t;

struct pt_pass_t {

    virtual bool run( pt_t & pt,
                      lexer_t & lexer, 
                      llb_fail_t & fail) = 0;
};
