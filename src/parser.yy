

%code requires
{
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "../src/AST.hpp"
}

%code top
{
#include "../src/AST.hpp"
static void node_lineinfo(parser_state* p, node_t::one_t* node)
{
  if (!node) return;
  node->fname = p->fname;
  node->linen = p->lineno;
}

}
%union {
    node_one_t* nd;
    int int_n_t;
}


%type <nd> program  topstmts topstmt_list topstmt stmts stmt_list stmt
%type <nd> primary expr expr_stmt func_call_args func_call_arg match_list one_match_list
%type <nd> var_def 
%type <nd> f_args f_args1
%type <nd> identifier lit_number lit_string def_struct_stmts def_struct_stmt_list def_struct_stmt
%type <nd> init_array_elem one_init_array_elem
%type <int_n_t> var_def_pointer
                


%pure-parser
%parse-param {parser_state *p}
%lex-param {p}

%{
int yylex(YYSTYPE *lval, parser_state *p);
static void yyerror(parser_state *p, const char *s);
%}

%token
        keyword_stmt_return
        keyword_namespace
        keyword_import
        keyword_def_struct
        keyword_return
        keyword_def_var
        keyword_new
        keyword_for
        keyword_while
        keyword_if
        keyword_else
        keyword_match
        keyword_end
        stmt_nil
        stmt_true
        stmt_false
        op_if
        op_else
        op_rasgn
        op_pointer_access
        op_access
        op_plus
        op_minus
        op_mult
        op_div
        op_mod
        op_eq
        op_neq
        op_lt
        op_le
        op_gt
        op_ge
        op_l_and
        op_l_or
        op_or
        op_and
        op_colon2
        op_assignment
        op_assignment_plus
        op_assignment_minus
        op_assignment_mult
        op_assignment_div
        op_assignment_mod
        op_assignment_l_and
        op_assignment_l_or
        op_assignment_and
        op_assignment_or
        end_semicolon
  
%token
        lit_time
        lit_number
        lit_string
        identifier


%left op_pointer_access op_access
%right keyword_if
%right keyword_match
%left  op_or
%left  op_and
%left  op_l_or
%left  op_l_and
%nonassoc  op_eq op_neq
%left  op_lt op_le op_gt op_ge
%left  op_plus op_minus
%left  op_mult op_div op_mod
%right '!' '~'

%left op_assignment_plus
%left op_assignment_minus
%left op_assignment_mult
%left op_assignment_div
%left op_assignment_mod
%left op_assignment_l_and
%left op_assignment_l_or
%left op_assignment_and
%left op_assignment_or

%%
program         : topstmts
                    {
                      p->lval = $1;
                    }
                ;

topstmts        : topstmt_list
                    {
                      $$ = $1;
                    }
                ;

topstmt_list    : topstmt
                    {
                      $$ = new node_t::some_t();
                      node_lineinfo(p, $$);
                      if ($1) {
                        ((node_t::some_t*)$$)->add($1);
                        node_lineinfo(p, $1);
                      }
                    }
                | topstmt_list topstmt
                    {
                      $$ = $1;
                      if ($2) {
                        if ($1) {
                          ((node_t::some_t*)$$)->add($2);
                        }
                        else {
                          $1 = $2;
                        }
                        node_lineinfo(p, $2);
                      }
                    }
                ;

topstmt         :
                keyword_import identifier
                {
                    $$ = new node_t::import_t($2);
                }
                |identifier identifier '(' f_args ')'  '{' stmts '}'
                {
                    $$ = new node_t::def_func_t($1,(node_t::symbol_t*)$2,$4,$7);
                }                
                ;

def_struct_stmts : def_struct_stmt_list
                {
                  $$=$1;
                }
                ;

def_struct_stmt_list : def_struct_stmt
                    {
                      $$ = new node_t::some_t();
                      node_lineinfo(p, $$);
                      if ($1) {
                        ((node_t::some_t*)$$)->add($1);
                        node_lineinfo(p, $1);
                      }
                    }
                      | def_struct_stmt_list def_struct_stmt
                    {
                      $$ = $1;
                      if ($2!=NULL) {
                        if ($1!=NULL) {
                          ((node_t::some_t*)$$)->add($2);
                        }
                        else {
                          $1 = $2;
                        }
                        node_lineinfo(p, $2);
                      }
                    }
                      ;

def_struct_stmt : var_def ';'
                    {
                      $$=$1;
                    }
                  ;

stmts            :stmt_list
                {
                    $$=$1;
                }
                ;

stmt_list       : stmt
                    {
                      $$ = new node_t::some_t();
                      node_lineinfo(p, $$);
                      if ($1) {
                        ((node_t::some_t*)$$)->add($1);
                        node_lineinfo(p, $1);
                      }
                    }
                | stmt_list stmt
                    {
                      $$ = $1;
                      if ($2!=NULL) {
                        if ($1!=NULL) {
                          ((node_t::some_t*)$$)->add($2);
                        }
                        else {
                          $1 = $2;
                        }
                        node_lineinfo(p, $2);
                      }
                    }
                ;

stmt            :keyword_return expr ';'
                {
                  $$=new node_t::return_t((node_t::base_expr_t*)$2);
                }
                |var_def ';'{
                  $$=$1;
                }
                | expr_stmt
                {
                  $$=$1;
                }
                | '{' stmts '}'
                {
                  $$= ($2);
                }
                ;

var_def         :identifier var_def_pointer identifier op_assignment expr
                {
                    $$ = new node_t::def_var_t($1,$3,$5,$2);
                }
                |identifier var_def_pointer identifier
                {
                    $$ = new node_t::def_var_t($1,$3,NULL,$2);
                }
                ;

var_def_pointer : 
                {
                  $$=0;
                }       
                |op_mult{
                  $$=1;
                }
                |var_def_pointer op_mult{
                  $$=$1+1;
                }
                ;

f_args           :  /* no args */
                    {
                      $$ = NULL;
                    }
                | f_args1
                ;

f_args1          :var_def
                    {
                        $$=new node_t::args_t();
                        ((node_t::args_t*)$$)->add( $1);
                    }
                | f_args1 ',' var_def
                    {
                      $$ = $1;
                      ((node_t::args_t*)$$)->add($3);
                    }
                ;

expr_stmt       :';'
                {
                  $$=NULL;
                }
                |expr ';'
                {
                  $$=$1;
                }
                ;

expr            :expr op_colon2 expr
                  {
                      $$ = new node_t::op_t("::", $1, $3);
                  }
                |expr op_access expr
                { 
                  $$=new node_t::op_t(".",$1,$3);
                }
                |expr op_pointer_access expr
                { 
                  $$=new node_t::op_t("->",$1,$3);
                }
                |expr '[' expr ']'
                { 
                  $$=new node_t::op_t("[]",$1,$3);
                }
                |expr '(' func_call_args ')'
                { 
                  $$=new node_t::func_call_t($1,(node_t::func_call_args_t*)$3);
                }
                | expr op_plus expr
                    {
                      $$ = new node_t::op_t("+", $1, $3);
                    }
                | expr op_minus expr
                    {
                      $$ = new node_t::op_t("-", $1, $3);
                    }
                | expr op_mult expr
                    {
                      $$ = new node_t::op_t("*", $1, $3);
                    }
                | expr op_div expr
                    {
                      $$ = new node_t::op_t("/", $1, $3);
                    }
                | expr op_mod expr
                    {
                      $$ = new node_t::op_t("%", $1, $3);
                    }
                |expr op_gt expr
                    {
                      $$ = new node_t::op_t(">", $1, $3);
                    }
                | expr op_ge expr
                    {
                      $$ = new node_t::op_t(">=", $1, $3);
                    }
                | expr op_lt expr
                    {
                      $$ = new node_t::op_t("<", $1, $3);
                    }
                | expr op_le expr
                    {
                      $$ = new node_t::op_t("<=", $1, $3);
                    }
                | expr op_eq expr
                    {
                      $$ = new node_t::op_t("==", $1, $3);
                    }
                | expr op_neq expr
                    {
                      $$ = new node_t::op_t("!=", $1, $3);
                    }
                | op_plus expr                 %prec '!'
                    {
                      $$ = $2;
                    }
                | op_minus expr                %prec '!'
                    {
                      $$ = new node_t::op_t("-", NULL, $2);
                    }
                | '!' expr
                    {
                      $$ = new node_t::op_t("!", NULL, $2);
                    }
                | '~' expr
                    {
                      $$ = new node_t::op_t("~", NULL, $2);
                    }
                | expr op_and expr
                    {
                      $$ = new node_t::op_t("&", $1, $3);
                    }
                | expr op_or expr
                    {
                      $$ = new node_t::op_t("|", $1, $3);
                    }
                | expr op_l_and expr
                    {
                      $$ = new node_t::op_t("&&", $1, $3);
                    }
                | expr op_l_or expr
                    {
                      $$ = new node_t::op_t("||", $1, $3);
                    }
                |expr op_assignment expr
                    {
                      $$ = new node_t::op_t("=", $1, $3);
                    }
                |expr op_assignment_plus expr
                    {
                      $$ = new node_t::op_t("+=", $1, $3);
                    }
                | expr op_assignment_minus expr
                    {
                      $$ = new node_t::op_t("-=", $1, $3);
                    }
                | expr op_assignment_mult expr
                    {
                      $$ = new node_t::op_t("*=", $1, $3);
                    }
                | expr op_assignment_div expr
                    {
                      $$ = new node_t::op_t("/=", $1, $3);
                    }
                | expr op_assignment_mod expr
                    {
                      $$ = new node_t::op_t("%=", $1, $3);
                    }
                | expr op_assignment_l_and expr
                    {
                      $$ = new node_t::op_t("&&=", $1, $3);
                    }
                | expr op_assignment_l_or expr
                    {
                      $$ = new node_t::op_t("||=", $1, $3);
                    }
                | expr op_assignment_and expr
                    {
                      $$ = new node_t::op_t("&=", $1, $3);
                    }
                | expr op_assignment_or expr
                    {
                      $$ = new node_t::op_t("|=", $1, $3);
                    }
                | primary
                ;


func_call_args  :
                {
                  $$=NULL;
                }
                |func_call_arg
                    {
                      $$ = new node_t::func_call_args_t();
                      node_lineinfo(p, $$);
                      if ($1) {
                        ((node_t::func_call_args_t*)$$)->add( $1);
                        node_lineinfo(p, $1);
                      }
                    }
                |func_call_args ',' func_call_arg
                    {
                      $$ = $1;
                      if ($3) {
                        if ($1) {
                          ((node_t::func_call_args_t*)$$)->add($3);
                        }
                        else {
                          $1 = $3;
                        }
                        node_lineinfo(p, $3);
                      }
                    }
                ;

func_call_arg       :expr
                      {
                        $$=$1;
                      }     
                      ;           

primary         : lit_number
                | identifier
                | '(' expr ')'
                    {
                       $$ = $2;
                    }
                ;
%%

//#include "lex.tab.cpp"

//#include "lex.tab.hpp"

static void
yyerror(parser_state *p, const char *s)
{
  p->nerr++;
  if (p->fname) {
    fprintf(stderr, "%s:%d:%s\n", p->fname, p->lineno, s);
  }
  else {
    fprintf(stderr, "%d:%s\n", p->lineno, s);
  }
}
