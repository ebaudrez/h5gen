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

#include <config.h>
#include "log.h"
#include "node.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "opt.h"

/* interface to lexer */
extern FILE *yyin;

int
main(int argc, char **argv)
{
    opt_t *options;

    options = opt_new(argc, argv);
    if (options->input) {
        if (!(yyin = fopen(options->input, "r"))) {
            int n = errno;
            log_error("cannot open input file '%s': %s\n", options->input, strerror(n));
            goto end;
        }
    }
    if (yyparse() != 0) {
        log_error("could not parse DDL");
        goto end;
    }
    if (node_create(file, NULL, options) != 0) {
        log_error("error creating HDF5 file");
    }
    node_free(file);

end:
    if (options->input) {
        fclose(yyin);
    }
    opt_free(options);
    return EXIT_SUCCESS;
}
