/*
 * Copyright (C) 2015 Edward Baudrez <edward.baudrez@gmail.com>
 * This file is part of h5gen.
 *
 * h5gen is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * h5gen is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with h5gen; if not, see <http://www.gnu.org/licenses/>.
 */

%option noyywrap nodefault
%option yylineno

%{
#include "log.h"
#include "node.h"
#include "parser.h"
#include <string.h>
#include <assert.h>
%}

%s IN_DATA

/* real number exponent */
EXP     ([eE][-+]?[0-9]+)

%%

    /* comments */
^[ \t]*#.*\n            /* ignore shell-style comments */

    /* ignore the index lists that h5dump helpfully inserts
     * before each row of data */
<IN_DATA>"}"            { BEGIN INITIAL; return yytext[0]; }
<IN_DATA>^[ \t]*\([ \t]*[0-9]+[ \t]*(,[ \t]*[0-9]+[ \t]*)*\)[ \t]*:

    /* one-character tokens */
[{}(),/]                { return yytext[0]; }

    /* keywords */
"DATA"                  { BEGIN IN_DATA; return DATA; }
"DATASET"               { return DATASET; }
"DATASPACE"             { return DATASPACE; }
"DATATYPE"              { return DATATYPE; }
"GROUP"                 { return GROUP; }
"HDF5"                  { return HDF5; }
"SCALAR"                { return SCALAR; }
"SIMPLE"                { return SIMPLE; }

    /* numbers */
-?[0-9]+                { yylval.integer = atoi(yytext); return INTEGER; }
-?([0-9]+\.[0-9]*|\.[0-9]+){EXP}? |
-?[0-9]+{EXP}           { yylval.realnum = atof(yytext); return REALNUM; }

    /* strings and identifiers */
\"([^"\\]|\\['"])*\"    { yylval.string = strdup(yytext + 1);
                          yylval.string[yyleng - 2] = '\0';
                          return QUOTED_STRING; }
[_a-zA-Z][_a-zA-Z0-9]*  { yylval.string = strdup(yytext); return IDENTIFIER; }

    /* other */
\n                      /* ignore newline */
[ \t]                   /* ignore whitespace */
.                       { yyerror("Mystery character %c", *yytext); }
