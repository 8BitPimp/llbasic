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

    // module
    virtual void visit( pt_module_t & n ) = 0;

    // functions
    virtual void visit(pt_function_decl_t & n) = 0;
    virtual void visit(pt_function_body_t & n) = 0;

    // variable
    virtual void visit(pt_decl_var_t & n) = 0;

    // statment
    virtual void visit( pt_if_t & n ) = 0;
    virtual void visit( pt_while_t & n ) = 0;
    virtual void visit( pt_return_t & n ) = 0;
    virtual void visit( pt_break_t & n ) = 0;
    virtual void visit( pt_continue_t & n ) = 0;
    virtual void visit( pt_call_t & n ) = 0;
    virtual void visit( pt_expr_t & n ) = 0;
    virtual void visit( pt_stmt_t & n ) = 0;

    // expression
    virtual void visit(pt_op_bin_t & n) = 0;
    virtual void visit(pt_op_ury_t & n) = 0;
    virtual void visit(pt_literal_t & n) = 0;
    virtual void visit(pt_identifier_t & n) = 0;
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

    virtual llb_location_t get_location() const = 0;
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

    template <typename type_t>
    shared_pt_node_t pop_specific() {
        assert(stack_.size() > 0);
        shared_pt_node_t node = top();
        assert(node->is_a<type_t>());
        stack_.pop_back();
        return node;
    }

    shared_pt_node_t pop() {
        assert( stack_.size() > 0 );
        shared_pt_node_t node = top( );
        stack_.pop_back();
        return node;
    }

    template <typename type_t>
    shared_pt_node_t top_specific() {
        assert(stack_.size() > 0);
        shared_pt_node_t node = *stack_.rbegin();
        assert(node->is_a<type_t>());
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

    pt_module_t()
        : name_()
        , functions_()
        , globals_()
    {
    }

    void add_function( shared_pt_node_t n ) {
        functions_.push_back(n);
    }

    void add_global( shared_pt_node_t n ) {
        globals_.push_back(n);
    }

    std::string name_;
    std::vector<shared_pt_node_t> functions_;
    std::vector<shared_pt_node_t> globals_;

    struct {
        std::vector<shared_pt_node_t> init_;
    }
    ext_;

    virtual llb_location_t get_location() const {
        return llb_location_t();
    }
};

struct pt_function_decl_t
    : public pt_node_t {
public:
    HAS_RTTI("pt_function_decl_t");
    VISITABLE();

    llb_token_t name_;
    llb_token_t ret_type_;
    std::vector<shared_pt_node_t> args_;

    pt_function_decl_t(llb_token_t name, llb_token_t type)
        : name_(name)
        , ret_type_(type)
    {}

    void add_arg(shared_pt_node_t node) {
        args_.push_back(node);
    }

    virtual llb_location_t get_location() const override {
        return name_.pos_;
    }

    shared_pt_node_t body_;
};

struct pt_function_body_t
    : public pt_node_t {
public:
    HAS_RTTI("pt_function_body_t");
    VISITABLE();

    weak_pt_node_t proto_;
    std::vector<shared_pt_node_t> stmt_;

    pt_function_body_t(shared_pt_node_t proto)
        : proto_(proto)
        , stmt_()
    {
    }

    virtual llb_location_t get_location() const override {
        shared_pt_node_t proto = proto_.lock();
        if (proto.get())
            return proto->get_location();
        else
            return llb_location_t();
    }

    void add_stmt(shared_pt_node_t node) {
        stmt_.push_back(node);
    }

    void add_local(shared_pt_node_t local) {
        ext_.locals_.push_back(local);
    }

    struct {

        std::vector<shared_pt_node_t> locals_;
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

    scope_t scope_;
    llb_token_t name_, type_;
    shared_pt_node_t expr_;

    pt_decl_var_t(scope_t scope,
        llb_token_t name,
        llb_token_t type,
        shared_pt_node_t expr)
        : scope_(e_unknown)
        , name_(name)
        , type_(type)
        , expr_(expr)
    {
    }

    virtual llb_location_t get_location() const override {
        return name_.pos_;
    }

    struct {
        pt_type_t type_;
    }
    ext_;
};

struct pt_op_bin_t
    : public pt_node_t {
public:
    HAS_RTTI( "pt_op_bin_t" );
    VISITABLE();

    llb_token_t operator_;
    shared_pt_node_t lhs_, rhs_;

    pt_op_bin_t( llb_token_t op,
                 shared_pt_node_t lhs,
                 shared_pt_node_t rhs )
        : operator_( op )
        , lhs_( lhs )
        , rhs_( rhs )
    {
    }

    virtual llb_location_t get_location() const override {
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

    llb_token_t operator_;
    shared_pt_node_t child_;

    pt_op_ury_t( llb_token_t op,
                 shared_pt_node_t child )
        : operator_( op )
        , child_( child )
    {
    }

    virtual llb_location_t get_location() const override {
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

    llb_token_t value_;

    pt_literal_t( llb_token_t value )
        : value_( value )
    {
    }

    virtual llb_location_t get_location() const override {
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

    llb_token_t name_;

    pt_identifier_t( llb_token_t name )
        : name_( name )
    {
    }

    virtual llb_location_t get_location() const override {
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

    llb_token_t token_;
    shared_pt_node_t expr_;

    pt_return_t( llb_token_t tok,
                 shared_pt_node_t expr )
        : token_( tok )
        , expr_( expr )
    {
    }

    virtual llb_location_t get_location() const override {
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

    llb_token_t token_;
    shared_pt_node_t expr_;
    std::vector<shared_pt_node_t> stmt_;

    pt_while_t( llb_token_t tok,
                shared_pt_node_t expr )
        : token_( tok )
        , expr_( expr )
        , stmt_( )
    {
    }

    virtual llb_location_t get_location() const override {
        return token_.pos_;
    }

    void add_stmt( shared_pt_node_t node ) {
        stmt_.push_back( node );
    }
};

struct pt_call_t
    : public pt_node_t {
public:
    HAS_RTTI("pt_call_t");
    VISITABLE();

    llb_token_t name_;
    std::vector<shared_pt_node_t> arg_;

    pt_call_t( llb_token_t name )
        : name_( name )
        , arg_()
    {
    };

    virtual llb_location_t get_location() const override {
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

    llb_token_t token_;
    shared_pt_node_t expr_;
    std::vector<shared_pt_node_t> stmt_1_;
    std::vector<shared_pt_node_t> stmt_0_;

    pt_if_t( llb_token_t tok, shared_pt_node_t expr )
        : token_( tok )
        , expr_( expr )
        , stmt_1_()
        , stmt_0_()
    {
    }

    virtual llb_location_t get_location() const override {
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

    llb_token_t token_;

    pt_break_t(llb_token_t tok)
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

    virtual llb_location_t get_location() const override {
        return expr_->get_location();
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

    virtual llb_location_t get_location() const override {
        return child_->get_location();
    }

    shared_pt_node_t child_;
};
