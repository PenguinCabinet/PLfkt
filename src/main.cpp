#include "parser.tab.hpp"
#include "lex.tab.hpp"
#include "../src/AST.hpp"
#include "../src/mean_AST.hpp"

int main(){
    const char* X="\
    int f(){\
        2*1+1;\
    }\
    ";
    parser_state* v=new parser_state();
    yy_scan_string(X);
    yyparse(v);
    //printf("TRUE2:%p",v->lval);

    v->lval->print(0);

    mean_AST_t* mean_AST=new mean_AST_t();
    mean_AST->check_ast(v->lval);

    v->lval->print(0);
    

    /*
    ASSERT_EQ(yyparse(v),0);
    node_debug_print((node_t*)(v->lval),0);
    compiler::compiler_class* compiler_data=new compiler::compiler_class();
    compiler_data->Run((node_t*)(v->lval));
    free(v);
    delete compiler_data;
    */
}