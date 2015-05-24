#pragma once

#include <stdint.h>
#include <string>
#include <sstream>
#include <initializer_list>

class llb_var_arg_t {
public:

    llb_var_arg_t( const std::string & in )
        : type_(e_string)
        , string_(&in)
    {}

    llb_var_arg_t( int32_t in )
        : type_(e_int)
        , int_(in)
    {}

    llb_var_arg_t(uint32_t in)
        : type_(e_uint)
        , uint_(in)
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
        e_uint,
        e_float,
    }
    type_;

    union {
        const char        * cstring_;
        const std::string * string_;
        int32_t             int_;
        uint32_t            uint_;
        float               float_;
    };
};

class llb_stream_t {
public:
    void print( const std::string & fmt,
                const std::initializer_list<llb_var_arg_t> & args );

    void print( const std::string & str );

    void println( const std::string & fmt,
                  const std::initializer_list<llb_var_arg_t> & args);
    
    void put_char(const char ch);

    void new_line();

    void clear();

    std::string to_string() const;

    const char * to_cstring() const;

    void fill(const char ch, uint32_t count);

protected:
    std::stringstream stream_;
};

class llb_string_t {
public:

    static
    std::string format( const std::string & fmt, 
                        const std::initializer_list<llb_var_arg_t> & args );

};
