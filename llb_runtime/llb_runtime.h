#pragma once

#define EXPORT __declspec( dllexport )
#define LLBCALL __cdecl

#include <string>
#include <memory>

struct llb_string_t {
    std::string string_;
};

typedef std::shared_ptr<llb_string_t> llb_rt_string_t;

extern "C" {

    EXPORT void LLBCALL print(llb_rt_string_t arg);

};
