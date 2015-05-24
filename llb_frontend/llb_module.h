#pragma once

#include <vector>
#include <memory>

#include "llb_forward.h"
#include "llb_pt.h"

struct module_t {

    module_t( const std::string & name, const char * src );

    std::string get_line(uint32_t line);

    std::string name_;
    const char * source_;
    std::vector<const char *> line_begin_;
    std::unique_ptr<token_list_t> tokens_;
};

typedef std::shared_ptr<module_t> shared_module_t;

struct llb_context_t {

    shared_module_t new_module(
        const std::string & name,
        const char * source);

    shared_module_t new_module(
        const std::string & name,
        std::shared_ptr<uint8_t> source);

    shared_module_t find_module(const std::string & name);

    std::vector<shared_module_t> list_;

    std::vector<std::shared_ptr<uint8_t>> sources_;

    pt_t pt_;
};
