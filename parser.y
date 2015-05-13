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
#include "log.h"
#include "node.h"
%}

%union {
    node_t *node;
    char   *string;
}

%token <string> IDENTIFIER
%token HDF5 GROUP
%type <node> file group

%%

file        : HDF5 IDENTIFIER '{' group '}' { file = node_new_file($2, $4); }
            ;

group       : GROUP IDENTIFIER '{' '}' { $$ = node_new_group($2); }
            ;
