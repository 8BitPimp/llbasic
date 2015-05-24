#pragma once

#include <cassert>

#include "llb_arg_parser.h"

bool arg_parser_t::is_empty() const {

    return files_.empty() && options_.empty() && switches_.empty();
}

bool arg_parser_t::parse(uint32_t argc, const char ** args) {

    if (!args)
        return false;

    files_.clear();
    options_.clear();

    pair_t pair = { nullptr, nullptr };

    for (uint32_t i = 0; i < argc; i++) {

        const char * arg = args[i];
        assert(arg != nullptr);
        
        if (arg[0] == '-') {
            arg += 1;

            if (arg[0] == '-') {
                arg += 1;

                switches_.push_back(arg);
            }
            else {

                pair.opt_ = arg;
            }
        }
        else {

            pair.arg_ = arg;
            if (pair.opt_ == nullptr)
                files_.push_back(arg);
            else
                options_.push_back(pair);

            pair.arg_ = nullptr;
            pair.opt_ = nullptr;
        }
    }

    return true;
}

bool arg_parser_t::find(const std::string & key, std::string & out) {

    for (pair_t & pair : options_) {
        if (pair.opt_ != key)
            continue;
        out = pair.arg_;
        return true;
    }
    return false;
}

bool arg_parser_t::find(const std::string & key) {

    for (const char * sw : switches_) {
        if (key == sw)
            return true;
    }
    return false;
}
