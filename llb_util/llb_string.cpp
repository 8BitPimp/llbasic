#include <sstream>
#include <assert.h>

#include "llb_string.h"

std::string llb_string_t::format(
    const std::string & fmt,
    const std::initializer_list<llb_var_arg_t> & args) {

    std::stringstream stream;

    // itterate over the strings
    for (uint32_t i = 0; fmt[i]; ++i ) {

        // check for format specifier
        if ( fmt[i] == '%') {
            // argument index
            assert(fmt[i + 1] != '\0');
            int32_t index = fmt[i + 1] - '0';
            assert(index >= 0 && index < int32_t( args.size() ));
            // access argument
            const llb_var_arg_t & v = args.begin()[index];
            switch (v.type_) {
            case (llb_var_arg_t::e_int) :
                stream << v.int_;
                break;
            case (llb_var_arg_t::e_float) :
                stream << v.float_;
                break;
            case (llb_var_arg_t::e_string) :
                assert(v.string_);
                stream << *v.string_;
                break;
            case (llb_var_arg_t::e_cstring) :
                assert(v.cstring_);
                stream << v.cstring_;
                break;
            default:
                assert(! "invalid var type");
            }
            // skip argument index
            ++i;
        }
        else {
            // append charactor
            stream << fmt[i];
        }
    }

    return stream.str();
}
