#include "../src/mean_AST.hpp"

mean_AST_t::mean_AST_t(){
    
}

void mean_AST_t::check_ast_op(node_t::base_expr_t* left,node_t::base_expr_t* right){
    if(*(left->val_type)==*(right->val_type)){
    }else{
        //TODO 暗黙のキャスト
        fprintf(stderr,"Type Error\n");
    }
}


void mean_AST_t::check_ast(node_t::one_t* ast){
    {
        node_t::one_t* A=ast;
        switch (A->node_type)
        {
        case node_t::node_type_t::op_t:
            {
                auto temp=(node_t::op_t*)A;
                check_ast(temp->right);
                check_ast(temp->left);
                check_ast_op((node_t::base_expr_t*)temp->right,(node_t::base_expr_t*)temp->left);
                temp->val_type=((node_t::base_expr_t*)temp->right)->val_type;
                break;
            }
        case node_t::node_type_t::some_t:
            {
                auto temp=(node_t::some_t*)A;
                for(int i=0;i<temp->i_size;i++){
                    check_ast(temp->data[i]);
                }
                break;
            }
        case node_t::node_type_t::import_t:
            {
                auto temp=(node_t::import_t*)A;
                //check import
                break;
            }
        case node_t::node_type_t::block_t:
            {
                //スコープ
                auto temp=(node_t::block_t*)A;
                for(int i=0;i<temp->i_size;i++){
                    check_ast(temp->data[i]);
                }
                break;
            }
        case node_t::node_type_t::args_t:
            {
                //スコープ
                auto temp=(node_t::block_t*)A;
                check_ast(temp);
                break;
            }

        /*
        case node_type_t::args_t:
            ((args_t*)A)->print2(deep);
            break;
        case node_type_t::func_call_args_t:
            ((func_call_args_t*)A)->print2(deep);
            break;
        case node_type_t::def_var_t:
            ((def_var_t*)A)->print2(deep);
            break;
        case node_type_t::return_t:
            ((return_t*)A)->print2(deep);
            break;
        case node_type_t::func_call_t:
            ((func_call_t*)A)->print2(deep);
            break;
        */
            /*
        case node_type_t::ident_var_t:
            ((ident_var_t*)A)->print2(deep);
            break;
            */
        case node_t::node_type_t::symbol_t:
            {
                //symbol check
                break;
            }
        case node_t::node_type_t::int_var_t:
            {
                //スコープ
                auto temp=(node_t::block_t*)A;
                break;
            }
        /*
        case node_type_t::int_var_t:
            ((int_var_t*)A)->print2(deep);
            break;
        case node_type_t::double_var_t:
            ((double_var_t*)A)->print2(deep);
            break;
        case node_type_t::def_func_t:
            ((def_func_t*)A)->print2(deep);
            break;
        */
        case node_t::node_type_t::def_func_t:
        {
            //関数定義処理
            auto temp=(node_t::def_func_t*)A;
            check_ast(temp->body);
            break;
        }
        }
    }
};