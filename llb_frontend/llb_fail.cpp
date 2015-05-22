#include "llb_fail.h"
#include "llb_token.h"

llb_fail_t::llb_fail_t()
    : msg_()
    , location_()
{
}

llb_fail_t::llb_fail_t(const std::string & str, token_t & tok)
    : msg_(str)
    , location_(tok.pos_)
{
}

llb_fail_t::llb_fail_t(const std::string & str, location_t & pos)
    : msg_(str)
    , location_(pos)
{
}

llb_fail_t::llb_fail_t(const std::string & str,
    uint32_t line,
    uint32_t column)
    : msg_(str)
    , location_(line, column)
{
}
