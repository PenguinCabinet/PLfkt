#ifndef AST_HPP
#define AST_HPP
#include <stdio.h>
#include <stdarg.h>


namespace node_t{

namespace node_type_t{
    enum node_type_t{
        one_t,
        op_t,
        some_t,
        import_t,
        block_t,
        args_t,
        func_call_args_t,
        def_var_t,
        return_t,
        func_call_t,
        ident_var_t,//10
        symbol_t,
        int_var_t,
        double_var_t,
        def_func_t
    };
    
};



class one_t{
  public:
    char* fname;
    int linen;
    node_type_t::node_type_t node_type;

    bool Is_expr;

    one_t(node_type_t::node_type_t node_type);

    virtual void print_first(int deep);

    virtual void print_end(int deep);

    void deep_print(int deep,const char* fmt,...);

    void print(int deep);
};

class symbol_t;

class base_expr_t:public one_t{
   public:
    symbol_t* val_type;
    base_expr_t(node_type_t::node_type_t node_type);

    void print_first(int deep)override;
    /*
    void print_first(int deep)override {
        for(int i=0;i<deep;i++){
            printf(" ");
        }
        printf("var_type:");
        val_type->print(deep+1);
    }*/
    void print2(int deep);
};

class op_t:public base_expr_t{
  public:
    const char* op;
    one_t* left;
    one_t* right;

    op_t(const char* op,one_t* left,one_t* right);
    void print2(int deep);
};

class some_t:public one_t{
  public:
    one_t** data;
    int size;
    int i_size;

    some_t();

    void add(one_t* v);

    void print2(int deep);
};

class import_t:public one_t{
  public:
  node_t::one_t* name;

    import_t(node_t::one_t* name);

    void print2(int deep);
};

class def_func_t:public one_t{
  public:
  one_t* ret_type;
  symbol_t* name;
  one_t* args;
  one_t* body;

    def_func_t(
        one_t* ret_type,
        symbol_t* name,
        one_t* args,
        one_t* body
    );
    void print2(int deep);
};

class block_t:public some_t{
    public:
    block_t();
};

class args_t:public some_t{
    public:
    args_t();
};

class func_call_args_t:public some_t{
    public:
    func_call_args_t();
};

/*
class block_t:public one_t{
  public:
  one_t* body;

    block_t(
        one_t* body
    ){
        this->body=body;
    }
};
*/

class def_var_t:public one_t{
  public:
  one_t* type;
  one_t* name;
  one_t* init_expr;
  int pointer_n;

    def_var_t(
        one_t* type,
        one_t* name,
        one_t* init_expr,
        int pointer_n
    );

    void print2(int deep);
};

class return_t:public one_t{
    public:
    base_expr_t* ret_expr;

    return_t(base_expr_t* v);

    void print2(int deep);
};

class func_call_t:public base_expr_t{
    public:
    one_t* name;
    func_call_args_t* args;

    func_call_t(one_t* name,func_call_args_t* args);

    void print2(int deep);
};

template<class T>
class ident_var_t:public base_expr_t{
    public:
    T data;

    ident_var_t(symbol_t* type,T data);
};



class symbol_t:public one_t{
    public:
    const char* name;

    symbol_t(const char* name);

    void print2(int deep);

    bool operator ==(const symbol_t &v) const;
};


class int_var_t:public ident_var_t<int>{
    public:

    int_var_t(int data);
    void print2(int deep);
};

class double_var_t:public ident_var_t<double>{
    public:

    double_var_t(double data);
    void print2(int deep);
};

};


class parser_state{
  public:
    int lineno;
    int tline;
    char* fname;
    int nerr;
    
    node_t::one_t* lval;
};

typedef node_t::one_t node_one_t;
#endif
