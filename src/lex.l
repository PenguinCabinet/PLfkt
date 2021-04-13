%{
#include <string.h>
#include <ctype.h>
#include "../src/AST.hpp"
#include "parser.tab.hpp"

#define YY_DECL    int yylex(YYSTYPE *lval, parser_state *p)

static int lex_return(parser_state *p, int c)
{
  p->lineno = p->tline;
  p->tline = yylineno;
  return c;
}



#define LEX_RETURN(v) return lex_return(p, v)
%}

%option noyywrap
%option yylineno
%option yy_scan_string


space       [ \n\t]*
char [a-zA-Z_]
charnum ({char}|[0-9])
word ({char}{charnum}*)

%%

"+="{space}  LEX_RETURN(op_assignment_plus);
"-="{space}  LEX_RETURN(op_assignment_minus);
"*="{space}  LEX_RETURN(op_assignment_mult);
"/="{space}  LEX_RETURN(op_assignment_div);
"%="{space}  LEX_RETURN(op_assignment_mod);
"&&="{space} LEX_RETURN(op_assignment_l_and);
"||="{space} LEX_RETURN(op_assignment_l_or);
"&="{space}  LEX_RETURN(op_assignment_and);
"|="{space}  LEX_RETURN(op_assignment_or);

"+"{space}  LEX_RETURN(op_plus);
"-"{space}  LEX_RETURN(op_minus);
"*"{space}  LEX_RETURN(op_mult);
"/"{space}  LEX_RETURN(op_div);
"%"{space}  LEX_RETURN(op_mod);
"=="{space} LEX_RETURN(op_eq);
"!="{space} LEX_RETURN(op_neq);
"<"{space}  LEX_RETURN(op_lt);
"<="{space} LEX_RETURN(op_le);
">"{space}  LEX_RETURN(op_gt);
">="{space} LEX_RETURN(op_ge);
"&&"{space} LEX_RETURN(op_l_and);
"||"{space} LEX_RETURN(op_l_or);
"&"{space}  LEX_RETURN(op_and);
"|"{space}  LEX_RETURN(op_or);
"."{space} LEX_RETURN(op_access);
"->"{space} LEX_RETURN(op_pointer_access);
"="{space}  LEX_RETURN(op_assignment);
"::"{space} LEX_RETURN(op_colon2);
"["{space} return('[');
"]"{space} return(']');
":"{space} return(':');

","{space} return(',');

"struct" LEX_RETURN(keyword_def_struct);

"return" LEX_RETURN(keyword_return);

"for" LEX_RETURN(keyword_for);

"while" LEX_RETURN(keyword_while);

"if" LEX_RETURN(keyword_if);

"else" LEX_RETURN(keyword_else);

"match" LEX_RETURN(keyword_match);

"end" LEX_RETURN(keyword_end);


(([1-9][0-9]*)|0) {
  lval->nd = new node_t::int_var_t(atol(yytext));
  LEX_RETURN(lit_number);
};

(([1-9][0-9]*)|0)(\.[0-9][0-9]*)? {
  double temp;
  sscanf(yytext, "%lf", &temp);
  lval->nd = new node_t::double_var_t(temp);
  LEX_RETURN(lit_number);
};

0x[0-9a-fA-F]+ {
  unsigned long temp;
  sscanf(yytext+2, "%lx", &temp);
  lval->nd = new node_t::int_var_t(temp);
  LEX_RETURN(lit_number);
};

0o[0-7]+ {
  unsigned long temp;
  sscanf(yytext+2, "%lo", &temp);
  lval->nd = new node_t::int_var_t(temp);
  LEX_RETURN(lit_number);
};

{word} {
  char* result=new char[yyleng+1];
  strncpy(result,yytext,yyleng);
  result[yyleng]='\n';
  lval->nd = new node_t::symbol_t(result);
  LEX_RETURN(identifier);
};

\"([^\\\"]|\\.)*\": {
  //lval->nd = node_string_new(yytext+1, yyleng-2);
  lval->nd=NULL;
  LEX_RETURN(lit_string);
};

"("  {return('(');};
")"  {return(')');};

"{"  {return('{');};
"}"  {return('}');};

";" {return(';');};

[ \n\t] ;

. {
    char c = yytext[0];

    fprintf(stderr, "%s:%d:lexical error", p->fname, yylineno);
    if ((c & 0x80) || !isprint(c)) {
      fprintf(stderr, "('\\%03o').\n", c);
    }
    else if (c == '\\') {
      fprintf(stderr, "('\\\\').\n");
    }
    else {
      fprintf(stderr, "('%c').\n", c);
    }
    exit(1); 
};

%%


