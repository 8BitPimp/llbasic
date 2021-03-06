#include <assert.h>

#include "llb_module.h"
#include "llb_context.h"
#include "llb_token.h"

llb_module_t::llb_module_t(
    const std::string & name,
    const char * src)
    : name_(name)
    , source_(src)
{
    tokens_.reset(new llb_token_list_t);
    assert(tokens_ != nullptr);
}

shared_llb_module_t 
llb_context_t::new_module(
    const std::string & name,
    const char * source) {

    shared_llb_module_t module(new llb_module_t(name, source));
    assert(module.get());
    list_.push_back(module);
    return module;
}

shared_llb_module_t 
llb_context_t::new_module(
    const std::string & name,
    std::shared_ptr<uint8_t> source) {

    sources_.push_back(source);
    return new_module(name, (const char *)source.get());
}

shared_llb_module_t 
llb_context_t::find_module(
    const std::string & name) {

    for (auto & x : list_)
        if (x->name_ == name)
            return x;
    return shared_llb_module_t();
}

std::string 
llb_module_t::get_line(uint32_t line) {
    
    assert(line < line_begin_.size());

    const char * start = line_begin_[line];
    const char * end = start;

    while (true) {
        if (end[0] == '\0') break;
        if (end[0] == '\n') break;
        end++;
    }

    if (start == end)
        return std::string();

    return std::string(start, end);
}