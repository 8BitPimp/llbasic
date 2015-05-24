#pragma once

#include <vector>
#include <memory>

#include "llb_forward.h"
#include "llb_pt.h"

struct llb_context_t {

    shared_llb_module_t new_module(
        const std::string & name,
        const char * source);

    shared_llb_module_t new_module(
        const std::string & name,
        std::shared_ptr<uint8_t> source);

    shared_llb_module_t find_module(const std::string & name);

    std::vector<shared_llb_module_t> list_;

    std::vector<std::shared_ptr<uint8_t>> sources_;

    pt_t pt_;
};
