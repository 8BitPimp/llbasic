#pragma once

#include <vector>
#include <memory>

#include "llb_forward.h"

struct llb_module_t {

    llb_module_t( const std::string & name, const char * src );

    std::string get_line(uint32_t line);

    std::string name_;
    const char * source_;
    std::vector<const char *> line_begin_;
    std::unique_ptr<llb_token_list_t> tokens_;
};

typedef std::shared_ptr<llb_module_t> shared_llb_module_t;
