#pragma once

struct exception_t {

    const char * message_;

    exception_t()
        : message_( nullptr )
    {
    }

    exception_t( const char * msg )
        : message_( msg )
    {
    }
};