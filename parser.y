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
%token DATA
%token DATASET
%token DATASPACE
%token DATATYPE
%token GROUP
%token HDF5
%token <integer> INTEGER
%token <id> PREDEF_DATATYPE
%token <string> QUOTED_STRING
%token <realnum> REALNUM
%token SCALAR
%token SIMPLE

/* nonterminals */
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

member_list : /* empty */         { $$ = NULL; }
            | member_list member  { $$ = nodelist_append($1, $2); }
            ;

member      : group
            | datatype
            | dataspace
            | dataset
            | data
            ;

value_list  : value                { $$ = nodelist_append(NULL, $1); }
            | value_list ',' value { $$ = nodelist_append($1, $3); }
            ;

par_value_list : '(' value_list ')' { $$ = $2; }

value       : INTEGER { $$ = node_new_integer($1); }
            | REALNUM { $$ = node_new_realnum($1); }
            ;

file        : HDF5 QUOTED_STRING '{' group '}' { file = node_new_file($2, $4); }
            ;

group       : GROUP QUOTED_STRING '{' member_list '}' { $$ = node_new_group($2, $4); }
            ;

datatype    : DATATYPE PREDEF_DATATYPE { $$ = node_new_datatype($2); }
            ;

dataspace   : DATASPACE SCALAR                                           { $$ = node_new_dataspace_scalar(); }
            | DATASPACE SIMPLE '{' par_value_list '/' par_value_list '}' { $$ = node_new_dataspace_simple($4, $6); }
            ;

data        : DATA '{' value_list '}' { $$ = node_new_data($3); }
            ;

dataset     : DATASET QUOTED_STRING '{' member_list '}' { $$ = node_new_dataset($2, $4); }
            ;
