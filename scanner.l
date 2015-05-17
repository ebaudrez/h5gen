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

extern char *unquote(const char *);
%}

%%

"{" |
"}"                     { return yytext[0]; }
"HDF5"                  { return HDF5; }
"GROUP"                 { return GROUP; }
"DATATYPE"              { return DATATYPE; }
"DATASPACE"             { return DATASPACE; }
"DATASET"               { return DATASET; }
\"([^"\\]|\\['"])*\"    { yylval.string = unquote(yytext); return QUOTED_STRING; }
[_a-zA-Z][_a-zA-Z0-9]*  { yylval.string = strdup(yytext); return IDENTIFIER; }
\n                      /* ignore newline */
[ \t]                   /* ignore whitespace */
.                       { yyerror("Mystery character %c\n", *yytext); }

%%

char *
unquote(const char *s)
{
    char *res, *p;
    assert(s && *s++ == '"');
    res = strdup(s);
    assert(p = strrchr(res, '"'));
    *p = '\0';
    return res;
}
