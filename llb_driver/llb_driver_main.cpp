// llb_util
#include "llb_string.h"
#include "llb_file.h"

// llb_frontend
#include "llb_fail.h"
#include "llb_module.h"
#include "llb_token.h"
#include "llb_lexer.h"
#include "llb_pt.h"
#include "llb_parser.h"
#include "llb_backend_cpp.h"
#include "llb_context.h"

// llb_passes
#include "llb_pass_linker.h"
#include "llb_pass_printer.h"

// argument parser
#include "llb_arg_parser.h"

struct llb_driver_cxt_t {

    llb_driver_cxt_t()
        : args_()
        , status_(e_ok)
        , message_()
        , modules_()
    {
    }

    arg_parser_t args_;

    enum {
        e_ok,
        e_failed,
    }
    status_;

    std::vector<std::string> message_;

    llb_context_t modules_;
};

void on_fail( const llb_fail_t & fail ) {

    shared_llb_module_t module = fail.location_.module_.lock();

    std::string module_name;
    if (module)
        module_name = module->name_;

    std::string msg = llb_string_t::format(
        "error:\n . %0\n . %3 %1,%2\n", {
        fail.msg_.c_str(),
        fail.location_.line_,
        fail.location_.column_,
        module_name });
    printf("%s", msg.c_str());

    if (module) {

        std::string line = fail.location_.get_line();
        if (!line.empty()) {
            printf(" . \"%s\"\n", line.c_str());

            for (uint32_t i = 0; i <= fail.location_.column_+3; ++i)
                putchar(' ');
            putchar('^');
            printf("\n");
        }
    }

    getchar();
    exit(0);
}

bool
load_modules( llb_driver_cxt_t & cxt ) {

    for (const char * path : cxt.args_.files_) {

        file_buffer_t buffer;
        if (!buffer.load(path)) {
            cxt.message_.push_back(
                llb_string_t::format("unable to load source from '%0'", { path }));
            return false;
        }

        shared_llb_module_t module = cxt.modules_.new_module(std::string(path), buffer.data_);
        if (!module) {
            cxt.message_.push_back(
                llb_string_t::format("unable to create new module from '%0'", { path }));
            return false;
        }
    }

    return true;
}

void print_messages( llb_driver_cxt_t & cxt ) {

    for (auto & x : cxt.message_) {
        puts(x.c_str());
    }
}

void print_usage() {
    const char usage[] =
R"(
    low level basic (llbasic):

    llbasic [switches] [options] source1.llb source2.llb ...
    switches:
        --help                  print this help screen
        --version               print the llbasic executable version

    options:
        -run-pass  [pass_name]  run a specific pass over the ast
        -emit-cpp  [path]       compile and emit cpp source
        -emit-ast  [path]       compile and emit human readable ast
        -emit-llvm [path]       compile and emit llvm bitcode
)";
    puts(usage);
}

void print_version() {
    puts("low level basic v0.01");
    puts("revision: ...");
    puts("compiled: " __DATE__ );
#if defined( _MSC_VER )
    puts("compiler: visual studio");
#endif
}

int main( const int argc, const char ** args  ) {

    llb_driver_cxt_t llb_cxt;
    assert(argc >= 1);
    if (!llb_cxt.args_.parse(argc-1, args+1)) {
        // unable to parse arguments
        print_usage();
        return -1;
    }

    if (llb_cxt.args_.find("help") || llb_cxt.args_.is_empty()) {
        print_usage();
        return 0;
    }

    if (! load_modules(llb_cxt) ) {
        // module loading failed
        return -1;
    }
    
    llb_fail_t fail;

    // loop over all modules
    for (auto & module : llb_cxt.modules_.list_) {

        // perform lexical analysis
        llb_lexer_t lexer(module);
        if (!lexer.run(fail)) {
            on_fail(fail);
            return -1;
        }

        // construct a parse tree
        llb_parser_t parser(module, llb_cxt.modules_.pt_);
        if (!parser.run(fail)) {
            on_fail(fail);
            return -2;
        }
    }

    llb_pass_manager_t manager;
    manager.register_defaults();
    manager.register_pass(new llb_creator_backend_cpp_t);

    manager.schedule(llb_pass_type_t::e_pass_hoist_var);
    manager.schedule(llb_pass_type_t::e_pass_linker);

    // emit the ast
    std::string path;
    if (llb_cxt.args_.find("emit-ast", path)) {
        // run the ast printer pass
        manager.schedule(llb_pass_type_t::e_pass_printer);
    }
    
    // translate to cpp code
    if (llb_cxt.args_.find("emit-cpp", path)) {
        // run the c++ codegen backend
        manager.schedule(llb_pass_type_t::e_pass_backend_cpp);
    }

    // run all of the passes
    if (!manager.run( llb_cxt.modules_ )) {
    }

    print_messages(llb_cxt);
    return 0;
}
