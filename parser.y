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
}

%token HDF5 GROUP
%token <string> IDENTIFIER
%type <node> file group
%type <list> member_list

%%

file        : HDF5 IDENTIFIER '{' group '}' { file = node_new_file($2, $4); }
            ;

group       : GROUP IDENTIFIER '{' member_list '}' { $$ = node_new_group($2, $4); }
            ;

member_list : /* empty */ { $$ = NULL; }
            | member_list group { $$ = nodelist_prepend($1, $2); }
            ;
