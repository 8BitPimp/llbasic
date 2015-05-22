#pragma once

#include <memory>

struct token_t;
class token_list_t;

struct pt_pass_t;
struct pt_pass_manager_t;

struct location_t;
struct llb_fail_t;

struct module_t;
typedef std::shared_ptr<module_t> shared_module_t;
typedef std::weak_ptr<module_t> weak_module_t;

struct module_list_t;

struct pt_node_t;
typedef std::shared_ptr<pt_node_t> shared_pt_node_t;
typedef std::weak_ptr<pt_node_t> weak_pt_node_t;

class  pt_t;
struct pt_module_t;
struct pt_op_bin_t;
struct pt_op_ury_t;
struct pt_literal_t;
struct pt_identifier_t;
struct pt_expr_t;
struct pt_stmt_t;
struct pt_decl_var_t;
struct pt_decl_function_t;
struct pt_stmt_t;
struct pt_return_t;
struct pt_call_t;
struct pt_if_t;
struct pt_while_t;
struct pt_for_t;
struct pt_break_t;
struct pt_continue_t;
