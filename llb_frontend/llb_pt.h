#pragma once

#include <memory>

#include "llb_forward.h"
#include "llb_token.h"

#define HAS_RTTI( NAME )                                    \
    static const pt_node_t::type_t & get_type_static() {   \
        static pt_node_t::type_t type( NAME );             \
        return type;                                        \
    }                                                       \
    virtual const pt_node_t::type_t & get_type() const {   \
        return get_type_static();                           \
    }

#define VISITABLE()                                                 \
    virtual void accept( pt_node_visitor_t & visitor ) override {   \
        visitor.visit(*this);                                       \
    }

struct pt_type_t {

    enum {
        e_none,
        e_bool,
        e_int,
        e_float,
        e_string,
        e_type
    }
    type_;

    weak_pt_node_t decl_;
};

class pt_node_visitor_t {
public:
    virtual void visit( pt_t & pt ) = 0;
    virtual void visit( pt_module_t & n ) = 0;
    virtual void visit( pt_literal_t & n ) = 0;
    virtual void visit( pt_identifier_t & n ) = 0;
    virtual void visit( pt_decl_function_t & n ) = 0;
    virtual void visit( pt_decl_var_t & n ) = 0;
    virtual void visit( pt_op_bin_t & n ) = 0;
    virtual void visit( pt_op_ury_t & n ) = 0;
    virtual void visit( pt_if_t & n ) = 0;
    virtual void visit( pt_while_t & n ) = 0;
    virtual void visit( pt_return_t & n ) = 0;
    virtual void visit( pt_break_t & n ) = 0;
    virtual void visit( pt_continue_t & n ) = 0;
    virtual void visit( pt_call_t & n ) = 0;
    virtual void visit( pt_expr_t & n ) = 0;
    virtual void visit( pt_stmt_t & n ) = 0;
};

struct pt_node_t {

    struct type_t {
    protected:
        uint32_t id_;
        const char * name_;

        static uint32_t new_id( ) {
            static uint32_t id = 0;
            return id++;
        }

    public:
        type_t( const char * name )
            : id_( new_id() )
            , name_( name )
        {
        }

        const char * name( ) const {
            return name_;
        }

        bool operator == (const type_t & t) const {
            return id_ == t.id_;
        }
    };

    virtual const type_t & get_type() const = 0;
    virtual void accept( pt_node_visitor_t & visitor ) = 0;

    virtual ~pt_node_t() {}

    template <typename type_t>
    bool is_a() const {
        return get_type() == type_t::get_type_static();
    }

    template <typename type_t>
    type_t * upcast() {
        if (! is_a<type_t>() )
            return nullptr;
        return static_cast<type_t*>( this );
    }

    virtual location_t get_location() const = 0;
};

class pt_t {
public:
    std::vector<shared_pt_node_t> stack_;

    void push( shared_pt_node_t node_ptr ) {
        assert( node_ptr.get() );
        stack_.push_back( node_ptr );
    }

    template <typename type_t>
    void push( type_t * derived_node ) {
        assert( derived_node != nullptr );
        pt_node_t * node = static_cast<pt_node_t*>( derived_node );
        stack_.push_back( shared_pt_node_t( node ) );
    }

    shared_pt_node_t pop() {
        assert( stack_.size() > 0 );
        shared_pt_node_t node = top( );
        stack_.pop_back();
        return node;
    }

    shared_pt_node_t top() {
        assert( stack_.size() > 0 );
        return *stack_.rbegin();
    }

    uint32_t index() const {
        return uint32_t( stack_.size() );
    }

    void visit( pt_node_visitor_t & visitor ) {

        visitor.visit(*this);
    }
};

struct pt_module_t
    : public pt_node_t {
public:
    HAS_RTTI("pt_module_t");
    VISITABLE();

    pt_module_t( )
        : functions_()
        , globals_()
    {
    }

    void add_function( shared_pt_node_t n ) {
        functions_.push_back(n);
    }

    void add_global( shared_pt_node_t n ) {
        globals_.push_back(n);
    }

    std::vector<shared_pt_node_t> functions_;
    std::vector<shared_pt_node_t> globals_;

    virtual location_t get_location() const {
        return location_t();
    }
};

struct pt_op_bin_t
    : public pt_node_t {
public:
    HAS_RTTI( "pt_op_bin_t" );
    VISITABLE();

    token_t operator_;
    shared_pt_node_t lhs_, rhs_;

    pt_op_bin_t( token_t op,
                 shared_pt_node_t lhs,
                 shared_pt_node_t rhs )
        : operator_( op )
        , lhs_( lhs )
        , rhs_( rhs )
    {
    }

    virtual location_t get_location() const override {
        return operator_.pos_;
    }

    struct {
        pt_type_t type_;
    }
    ext_;
};

struct pt_op_ury_t
        : public pt_node_t {
public:
    HAS_RTTI( "pt_op_ury_t" );
    VISITABLE();

    token_t operator_;
    shared_pt_node_t child_;

    pt_op_ury_t( token_t op,
                 shared_pt_node_t child )
        : operator_( op )
        , child_( child )
    {
    }

    virtual location_t get_location() const override {
        return operator_.pos_;
    }

    struct {
        pt_type_t type_;
    }
    ext_;
};

struct pt_literal_t
        : public pt_node_t {
public:
    HAS_RTTI("pt_literal_t");
    VISITABLE();

    token_t value_;

    pt_literal_t( token_t value )
        : value_( value )
    {
    }

    virtual location_t get_location() const override {
        return value_.pos_;
    }

    struct {
        pt_type_t type_;
    }
    ext_;
};

struct pt_identifier_t
        : public pt_node_t {
public:
    HAS_RTTI("pt_identifier_t");
    VISITABLE();

    token_t name_;

    pt_identifier_t( token_t name )
        : name_( name )
    {
    }

    virtual location_t get_location() const override {
        return name_.pos_;
    }

    struct {

        weak_pt_node_t decl_;
        pt_type_t type_;
    } ext_;
};

struct pt_return_t
    : public pt_node_t {
public:
    HAS_RTTI("pt_return_t");
    VISITABLE();

    token_t token_;
    shared_pt_node_t expr_;

    pt_return_t( token_t tok,
                 shared_pt_node_t expr )
        : token_( tok )
        , expr_( expr )
    {
    }

    virtual location_t get_location() const override {
        return token_.pos_;
    }

    struct {

        weak_pt_node_t func_;
        pt_type_t type_;
        weak_pt_node_t assign_;
    }
    ext_;
};

struct pt_while_t
    : public pt_node_t {
public:
    HAS_RTTI("pt_while_t");
    VISITABLE();

    token_t token_;
    shared_pt_node_t expr_;
    std::vector<shared_pt_node_t> stmt_;

    pt_while_t( token_t tok,
                shared_pt_node_t expr )
        : token_( tok )
        , expr_( expr )
        , stmt_( )
    {
    }

    virtual location_t get_location() const override {
        return token_.pos_;
    }

    void add_stmt( shared_pt_node_t node ) {
        stmt_.push_back( node );
    }
};

struct pt_decl_function_t
    : public pt_node_t {
public:
    HAS_RTTI("pt_decl_function_t");
    VISITABLE();

    token_t name_;
    token_t ret_type_;
    std::vector<shared_pt_node_t> args_;
    std::vector<shared_pt_node_t> stmt_;

    pt_decl_function_t( token_t name,
                        token_t ret_type )
        : name_( name )
        , ret_type_( ret_type )
        , args_( )
        , stmt_( )
    {
    }

    virtual location_t get_location() const override {
        return name_.pos_;
    }

    void add_arg( shared_pt_node_t node ) {
        args_.push_back( node );
    }

    void add_stmt( shared_pt_node_t node ) {
        stmt_.push_back( node );
    }

    struct {

        std::vector<weak_pt_node_t> locals_;
        pt_type_t type_;
    }
    ext_;
};

struct pt_call_t
    : public pt_node_t {
public:
    HAS_RTTI("pt_call_t");
    VISITABLE();

    token_t name_;
    std::vector<shared_pt_node_t> arg_;

    pt_call_t( token_t name )
        : name_( name )
        , arg_()
    {
    };

    virtual location_t get_location() const override {
        return name_.pos_;
    }

    void add_arg( shared_pt_node_t node ) {
        arg_.push_back( node );
    }

    struct {

        weak_pt_node_t func_;
    }
    ext_;
};

struct pt_if_t
    : public pt_node_t {
public:
    HAS_RTTI("pt_if_t");
    VISITABLE();

    token_t token_;
    shared_pt_node_t expr_;
    std::vector<shared_pt_node_t> stmt_1_;
    std::vector<shared_pt_node_t> stmt_0_;

    pt_if_t( token_t tok, shared_pt_node_t expr )
        : token_( tok )
        , expr_( expr )
        , stmt_1_()
        , stmt_0_()
    {
    }

    virtual location_t get_location() const override {
        return token_.pos_;
    }

    void add_stmt( shared_pt_node_t node,
                   bool branch ) {
        (branch ? stmt_1_ : stmt_0_).push_back( node );
    }
};

struct pt_break_t
    : public pt_node_t {
public:
    HAS_RTTI("pt_break_t");
    VISITABLE();

    token_t token_;

    pt_break_t(token_t tok)
        : token_(tok)
    {
    }

    struct {

        weak_pt_node_t loop_;
    } ext_;
};

struct pt_continue_t
        : public pt_node_t {
public:
    HAS_RTTI("pt_continue_t");
    VISITABLE();

    struct {

        weak_pt_node_t loop_;
    } ext_;
};

struct pt_expr_t
    : public pt_node_t {
public:
    HAS_RTTI("pt_expr_t");
    VISITABLE();

    shared_pt_node_t expr_;

    pt_expr_t( shared_pt_node_t expr )
        : expr_( expr )
    {
    }

    virtual location_t get_location() const override {
        return expr_->get_location();
    }

    struct {
        pt_type_t type_;
    }
    ext_;
};

struct pt_decl_var_t
    : public pt_node_t {
public:
    HAS_RTTI("pt_decl_var_t");
    VISITABLE();

    enum scope_t {
        e_unknown,
        e_global,
        e_local,
        e_arg,
    };

    enum kind_t {
        e_primitive,
        e_vector,
    };

    scope_t scope_;
    token_t name_, type_;
    shared_pt_node_t expr_;

    pt_decl_var_t( scope_t scope,
                   token_t name,
                   token_t type,
                   shared_pt_node_t expr )
        : scope_( e_unknown )
        , name_( name )
        , type_( type )
        , expr_( expr )
    {
    }

    virtual location_t get_location() const override {
        return name_.pos_;
    }

    struct {
        pt_type_t type_;
    }
    ext_;
};

struct pt_stmt_t
    : public pt_node_t {
public:
    HAS_RTTI("pt_stmt_t");
    VISITABLE();

    pt_stmt_t(shared_pt_node_t node)
        : child_(node) 
    {
    }

    virtual location_t get_location() const override {
        return child_->get_location();
    }

    shared_pt_node_t child_;
};
