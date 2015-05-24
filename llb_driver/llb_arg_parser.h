#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>

class arg_parser_t {
public:

    struct pair_t {
        
        const char * opt_;
        const char * arg_;
    };

    //
    bool is_empty() const;

    // parse arguments
    bool parse(uint32_t argc, const char ** args);
    
    // find option
    bool find(const std::string & key, std::string & out);

    // find switch
    bool find(const std::string & key);

    // free arguments
    std::vector<const char *> files_;

    // options
    std::vector<pair_t> options_;

    // switches
    std::vector<const char *> switches_;
};
