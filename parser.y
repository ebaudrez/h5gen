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

%{
#include <stdlib.h>
#include "log.h"
#include "node.h"
%}

%union {
    char       *string;
    node_t     *node;
    nodelist_t *list;
    int         integer;
    double      realnum;
    hid_t       id;
}

/* tokens */
%token TOK_ATTRIBUTE
%token TOK_DATA
%token TOK_DATASET
%token TOK_DATASPACE
%token TOK_DATATYPE
%token <id> TOK_FLOAT_TYPE
%token TOK_GROUP
%token TOK_HDF5
%token <integer> TOK_INTEGER
%token <id> TOK_INTEGER_TYPE
%token <string> TOK_STRING
%token <realnum> TOK_REALNUM
%token TOK_SCALAR
%token TOK_SIMPLE

/* nonterminals */
%type <node> attribute
%type <node> data
%type <node> dataset
%type <node> dataspace
%type <node> datatype
%type <node> file
%type <node> group
%type <node> member
%type <list> member_list
%type <list> par_value_list
%type <node> value
%type <list> value_list

%start file

%%

attribute   : TOK_ATTRIBUTE TOK_STRING '{' member_list '}' { $$ = node_new_attribute($2, $4); }
            ;

data        : TOK_DATA '{' value_list '}' { $$ = node_new_data($3); }
            ;

dataset     : TOK_DATASET TOK_STRING '{' member_list '}' { $$ = node_new_dataset($2, $4); }
            ;

dataspace   : TOK_DATASPACE TOK_SCALAR                                           { $$ = node_new_dataspace_scalar(); }
            | TOK_DATASPACE TOK_SIMPLE '{' par_value_list '/' par_value_list '}' { $$ = node_new_dataspace_simple($4, $6); }
            ;

datatype    : TOK_DATATYPE TOK_FLOAT_TYPE   { $$ = node_new_datatype_float($2); }
            | TOK_DATATYPE TOK_INTEGER_TYPE { $$ = node_new_datatype_integer($2); }
            ;

file        : TOK_HDF5 TOK_STRING '{' group '}' { file = node_new_file($2, $4); }
            ;

group       : TOK_GROUP TOK_STRING '{' member_list '}' { $$ = node_new_group($2, $4); }
            ;

member      : attribute
            | data
            | dataset
            | dataspace
            | datatype
            | group
            ;

member_list : /* empty */        { $$ = NULL; }
            | member_list member { $$ = nodelist_append($1, $2); }
            ;

par_value_list : '(' value_list ')' { $$ = $2; }

value       : TOK_INTEGER { $$ = node_new_integer($1); }
            | TOK_REALNUM { $$ = node_new_realnum($1); }
            ;

value_list  : value                { $$ = nodelist_append(NULL, $1); }
            | value_list ',' value { $$ = nodelist_append($1, $3); }
            ;
