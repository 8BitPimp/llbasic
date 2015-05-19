#pragma once

#include <stdint.h>
#include <string>
#include <initializer_list>

class llb_var_arg_t {
public:

    llb_var_arg_t( std::string & in )
        : type_(e_string)
        , string_(&in)
    {}

    llb_var_arg_t( int32_t in )
        : type_(e_int)
        , int_(in)
    {}

    llb_var_arg_t(float in)
        : type_(e_float)
        , float_(in)
    {}

    llb_var_arg_t(const char cstr[])
        : type_(e_cstring)
        , cstring_(cstr)
    {}

    enum {
        e_cstring,
        e_string,
        e_int,
        e_float,
    }
    type_;

    union {
        const char  * cstring_;
        std::string * string_;
        int32_t       int_;
        float         float_;
    };
};

class llb_stream_t {
public:
    void print( const std::string & fmt,
                const std::initializer_list<llb_var_arg_t> & args );

    std::string to_string() const;

protected:


};

class llb_string_t {
public:

    static
    std::string format( const std::string & fmt, 
                        const std::initializer_list<llb_var_arg_t> & args );

};
