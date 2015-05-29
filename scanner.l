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
"ATTRIBUTE"             { return ATTRIBUTE; }
"DATA"                  { BEGIN IN_DATA; return DATA; }
"DATASET"               { return DATASET; }
"DATASPACE"             { return DATASPACE; }
"DATATYPE"              { return DATATYPE; }
"GROUP"                 { return GROUP; }
"HDF5"                  { return HDF5; }
"SCALAR"                { return SCALAR; }
"SIMPLE"                { return SIMPLE; }

    /* predefined datatypes, from H5Tpublic.h */
"H5T_IEEE_F32BE"        { yylval.id = H5T_IEEE_F32BE; return PREDEF_DATATYPE; }
"H5T_IEEE_F32LE"        { yylval.id = H5T_IEEE_F32LE; return PREDEF_DATATYPE; }
"H5T_IEEE_F64BE"        { yylval.id = H5T_IEEE_F64BE; return PREDEF_DATATYPE; }
"H5T_IEEE_F64LE"        { yylval.id = H5T_IEEE_F64LE; return PREDEF_DATATYPE; }
"H5T_STD_I8BE"          { yylval.id = H5T_STD_I8BE; return PREDEF_DATATYPE; }
"H5T_STD_I8LE"          { yylval.id = H5T_STD_I8LE; return PREDEF_DATATYPE; }
"H5T_STD_I16BE"         { yylval.id = H5T_STD_I16BE; return PREDEF_DATATYPE; }
"H5T_STD_I16LE"         { yylval.id = H5T_STD_I16LE; return PREDEF_DATATYPE; }
"H5T_STD_I32BE"         { yylval.id = H5T_STD_I32BE; return PREDEF_DATATYPE; }
"H5T_STD_I32LE"         { yylval.id = H5T_STD_I32LE; return PREDEF_DATATYPE; }
"H5T_STD_I64BE"         { yylval.id = H5T_STD_I64BE; return PREDEF_DATATYPE; }
"H5T_STD_I64LE"         { yylval.id = H5T_STD_I64LE; return PREDEF_DATATYPE; }
"H5T_STD_U8BE"          { yylval.id = H5T_STD_U8BE; return PREDEF_DATATYPE; }
"H5T_STD_U8LE"          { yylval.id = H5T_STD_U8LE; return PREDEF_DATATYPE; }
"H5T_STD_U16BE"         { yylval.id = H5T_STD_U16BE; return PREDEF_DATATYPE; }
"H5T_STD_U16LE"         { yylval.id = H5T_STD_U16LE; return PREDEF_DATATYPE; }
"H5T_STD_U32BE"         { yylval.id = H5T_STD_U32BE; return PREDEF_DATATYPE; }
"H5T_STD_U32LE"         { yylval.id = H5T_STD_U32LE; return PREDEF_DATATYPE; }
"H5T_STD_U64BE"         { yylval.id = H5T_STD_U64BE; return PREDEF_DATATYPE; }
"H5T_STD_U64LE"         { yylval.id = H5T_STD_U64LE; return PREDEF_DATATYPE; }
"H5T_STD_B8BE"          { yylval.id = H5T_STD_B8BE; return PREDEF_DATATYPE; }
"H5T_STD_B8LE"          { yylval.id = H5T_STD_B8LE; return PREDEF_DATATYPE; }
"H5T_STD_B16BE"         { yylval.id = H5T_STD_B16BE; return PREDEF_DATATYPE; }
"H5T_STD_B16LE"         { yylval.id = H5T_STD_B16LE; return PREDEF_DATATYPE; }
"H5T_STD_B32BE"         { yylval.id = H5T_STD_B32BE; return PREDEF_DATATYPE; }
"H5T_STD_B32LE"         { yylval.id = H5T_STD_B32LE; return PREDEF_DATATYPE; }
"H5T_STD_B64BE"         { yylval.id = H5T_STD_B64BE; return PREDEF_DATATYPE; }
"H5T_STD_B64LE"         { yylval.id = H5T_STD_B64LE; return PREDEF_DATATYPE; }
"H5T_NATIVE_CHAR"       { yylval.id = H5T_NATIVE_CHAR; return PREDEF_DATATYPE; }
"H5T_NATIVE_SCHAR"      { yylval.id = H5T_NATIVE_SCHAR; return PREDEF_DATATYPE; }
"H5T_NATIVE_UCHAR"      { yylval.id = H5T_NATIVE_UCHAR; return PREDEF_DATATYPE; }
"H5T_NATIVE_SHORT"      { yylval.id = H5T_NATIVE_SHORT; return PREDEF_DATATYPE; }
"H5T_NATIVE_USHORT"     { yylval.id = H5T_NATIVE_USHORT; return PREDEF_DATATYPE; }
"H5T_NATIVE_INT"        { yylval.id = H5T_NATIVE_INT; return PREDEF_DATATYPE; }
"H5T_NATIVE_UINT"       { yylval.id = H5T_NATIVE_UINT; return PREDEF_DATATYPE; }
"H5T_NATIVE_LONG"       { yylval.id = H5T_NATIVE_LONG; return PREDEF_DATATYPE; }
"H5T_NATIVE_ULONG"      { yylval.id = H5T_NATIVE_ULONG; return PREDEF_DATATYPE; }
"H5T_NATIVE_LLONG"      { yylval.id = H5T_NATIVE_LLONG; return PREDEF_DATATYPE; }
"H5T_NATIVE_ULLONG"     { yylval.id = H5T_NATIVE_ULLONG; return PREDEF_DATATYPE; }
"H5T_NATIVE_FLOAT"      { yylval.id = H5T_NATIVE_FLOAT; return PREDEF_DATATYPE; }
"H5T_NATIVE_DOUBLE"     { yylval.id = H5T_NATIVE_DOUBLE; return PREDEF_DATATYPE; }
"H5T_NATIVE_LDOUBLE"    { yylval.id = H5T_NATIVE_LDOUBLE; return PREDEF_DATATYPE; }
"H5T_NATIVE_B8"         { yylval.id = H5T_NATIVE_B8; return PREDEF_DATATYPE; }
"H5T_NATIVE_B16"        { yylval.id = H5T_NATIVE_B16; return PREDEF_DATATYPE; }
"H5T_NATIVE_B32"        { yylval.id = H5T_NATIVE_B32; return PREDEF_DATATYPE; }
"H5T_NATIVE_B64"        { yylval.id = H5T_NATIVE_B64; return PREDEF_DATATYPE; }
"H5T_NATIVE_OPAQUE"     { yylval.id = H5T_NATIVE_OPAQUE; return PREDEF_DATATYPE; }
"H5T_NATIVE_HADDR"      { yylval.id = H5T_NATIVE_HADDR; return PREDEF_DATATYPE; }
"H5T_NATIVE_HSIZE"      { yylval.id = H5T_NATIVE_HSIZE; return PREDEF_DATATYPE; }
"H5T_NATIVE_HSSIZE"     { yylval.id = H5T_NATIVE_HSSIZE; return PREDEF_DATATYPE; }
"H5T_NATIVE_HERR"       { yylval.id = H5T_NATIVE_HERR; return PREDEF_DATATYPE; }
"H5T_NATIVE_HBOOL"      { yylval.id = H5T_NATIVE_HBOOL; return PREDEF_DATATYPE; }

    /* numbers */
-?[0-9]+                { yylval.integer = atoi(yytext); return INTEGER; }
-?([0-9]+\.[0-9]*|\.[0-9]+){EXP}? |
-?[0-9]+{EXP}           { yylval.realnum = atof(yytext); return REALNUM; }

    /* strings and identifiers */
\"([^"\\]|\\['"])*\"    { yylval.string = strdup(yytext + 1);
                          yylval.string[yyleng - 2] = '\0';
                          return QUOTED_STRING; }
[_a-zA-Z][_a-zA-Z0-9]*  { yyerror("unrecognized identifier %s", yytext); }

    /* other */
\n                      /* ignore newline */
[ \t]                   /* ignore whitespace */
.                       { yyerror("unrecognized character %c", *yytext); }
