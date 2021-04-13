#include <string.h>
#include "../src/AST.hpp"

using namespace node_t;

#define child_node_print_run(X) if(X!=NULL){((node_t::one_t*)X)->print(deep+1);}

    base_expr_t::base_expr_t(node_type_t::node_type_t node_type):one_t(node_type){
        Is_expr=true;
    }

    void base_expr_t::print2(int deep){
        deep_print(deep,"var_type:");
        ((one_t*)val_type)->print(deep+1);
    }

    void base_expr_t::print_first(int deep){
        one_t::print_first(deep);
        
        if(val_type!=NULL){
            deep_print(deep,"val_type:%s\n",val_type->name);
        }else{
            deep_print(deep,"val_type:NULL\n");
        }
    };


    op_t::op_t(const char* op,one_t* left,one_t* right):base_expr_t(node_type_t::op_t){
        this->op=op;
        this->left=left;
        this->right=right;
    }

    void op_t::print2(int deep){
        deep_print(deep,"op:%s\n",op);

        deep_print(deep,"left:\n");
        child_node_print_run(left);

        deep_print(deep,"right:\n");
        child_node_print_run(right);
    }

    some_t::some_t():one_t(node_type_t::some_t){
        data=new one_t*[32];
        size=32;
        i_size=0;
    }

    void some_t::add(one_t* v){ 
        data[i_size]=v;
        i_size++;
        if(i_size>=size){
            size*=2;
            one_t** temp=new one_t*[size];
            for(int i=0;i<i_size;i++){
                temp[i]=data[i];
            }
            data=temp;
            delete data;
        }
    }    

    void some_t::print2(int deep){
        for(int i=0;i<i_size+1;i++){
            //deep_print(deep,"\n",op);
            child_node_print_run(data[i]);
        }
    }

    import_t::import_t(node_t::one_t* name):one_t(node_type_t::import_t){
        this->name=name;
    }

    void import_t::print2(int deep){
        deep_print(deep,"name:\n");

        child_node_print_run(name);
    }

    def_func_t::def_func_t(
        one_t* ret_type,
        one_t* name,
        one_t* args,
        one_t* body
    ):one_t(node_type_t::def_func_t){
        this->ret_type=ret_type;
        this->name=name;
        this->args=args;
        this->body=body;
    }

    void def_func_t::print2(int deep){
        deep_print(deep,"ret_type:\n");
        child_node_print_run(ret_type);
        deep_print(deep,"name:\n");
        child_node_print_run(name);
        deep_print(deep,"args:\n");
        child_node_print_run(args);
        deep_print(deep,"body:\n");
        child_node_print_run(body);
    }

block_t::block_t():some_t(){
        this->node_type=node_type_t::block_t;
    }

    args_t::args_t():some_t(){
        this->node_type=node_type_t::args_t;
    }

func_call_args_t::func_call_args_t():some_t(){
        this->node_type=node_type_t::func_call_args_t;
}


    def_var_t::def_var_t(
        one_t* type,
        one_t* name,
        one_t* init_expr,
        int pointer_n
    ):one_t(node_type_t::def_var_t){
        this->type=type;
        this->name=name;
        this->init_expr=init_expr;
        this->pointer_n=pointer_n;
    }


    void def_var_t::print2(int deep){
        deep_print(deep,"type:\n");
        child_node_print_run(type);
        deep_print(deep,"pointer_n:%d\n",pointer_n);
        deep_print(deep,"name:\n");
        child_node_print_run(name);
        deep_print(deep,"init_expr:\n");
        child_node_print_run(init_expr);
    }

    return_t::return_t(base_expr_t* v):one_t(node_type_t::return_t){
        this->ret_expr=v;
    }
    void return_t::print2(int deep){
        deep_print(deep,"type:\n");
        child_node_print_run(ret_expr);
    }


    func_call_t::func_call_t(one_t* name,func_call_args_t* args):base_expr_t(node_type_t::func_call_t){
        this->name=name;
        this->args=args;
    }

    void func_call_t::print2(int deep){
        deep_print(deep,"name:\n");
        child_node_print_run(name);
        deep_print(deep,"args:\n");
        child_node_print_run(args);
    }



template<class T>
    ident_var_t<T>::ident_var_t(symbol_t* type,T data):base_expr_t(node_type_t::ident_var_t){
        val_type=type;
        this->data=data;
    }

    bool symbol_t::operator ==(const symbol_t &v) const{
        if (strcmp(this->name, v.name) == 0) {
            return true;
        }
        else{
            return false;
        }
    }

    symbol_t::symbol_t(const char* name):one_t(node_type_t::symbol_t){
        this->name=name;
    }

    void symbol_t::print2(int deep){
        deep_print(deep,"name:%s\n",name);
    }

    int_var_t::int_var_t(int data):ident_var_t<int>(new symbol_t("int"),data)
    {
        this->node_type=node_type_t::int_var_t;
    }
    void int_var_t::print2(int deep){
        deep_print(deep,"data:%d\n",data);
    }


double_var_t::double_var_t(double data):ident_var_t<double>(new node_t::symbol_t("double"),data)
{
        this->node_type=node_type_t::double_var_t;
}
void double_var_t::print2(int deep){
    deep_print(deep,"data:%f\n",data);
}



    one_t::one_t(node_type_t::node_type_t node_type){
        this->node_type=node_type;
        Is_expr=false;
    }

    void  one_t::print_first(int deep){
        for(int i=0;i<deep;i++){
            printf(" ");
        }
        printf("node_type:%d\n",node_type);
    }

    void  one_t::print_end(int deep){
        //printf("\n");
    }

    void  one_t::deep_print(int deep,const char* fmt,...){
        for(int i=0;i<deep;i++){
            printf(" ");
        }
        va_list list;
        va_start(list, fmt);
        vprintf(fmt, list);
        va_end(list);

    }


void one_t::print(int deep){
        print_first(deep);
        one_t* A=this;

        //printf("TRUE\n");

        switch (A->node_type)
        {
            /*
        case node_type_t::one_t:
            ((one_t*)A)->print2(deep);
            break;
            */
        case node_type_t::op_t:
            ((op_t*)A)->print2(deep);
            break;
        case node_type_t::some_t:
            ((some_t*)A)->print2(deep);
            break;
        case node_type_t::import_t:
            ((import_t*)A)->print2(deep);
            break;
        case node_type_t::block_t:
            ((block_t*)A)->print2(deep);
            break;
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
            /*
        case node_type_t::ident_var_t:
            ((ident_var_t*)A)->print2(deep);
            break;
            */
        case node_type_t::symbol_t:
            ((symbol_t*)A)->print2(deep);
            break;
        case node_type_t::int_var_t:
            ((int_var_t*)A)->print2(deep);
            break;
        case node_type_t::double_var_t:
            ((double_var_t*)A)->print2(deep);
            break;
        case node_type_t::def_func_t:
            ((def_func_t*)A)->print2(deep);
            break;
        }

        print_end(deep);
}