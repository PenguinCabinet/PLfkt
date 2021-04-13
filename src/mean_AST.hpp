#include "../src/AST.hpp"

class mean_AST_t{
    public:
    mean_AST_t();
    void check_ast(node_t::one_t* ast);
    void check_ast_op(node_t::base_expr_t* left,node_t::base_expr_t* right);
};
