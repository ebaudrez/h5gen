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
#include "opt.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "log.h"
#include <assert.h>

static void
print_usage(void)
{
    puts("h5gen - generate HDF5 files from DDL\n"
         "Usage\n"
         "    h5gen [ -o output.h5 ] [ input.ddl ]\n"
         "\n"
         "Options\n"
         "    -o output.h5: output HDF5 file to output.h5 (default: use name in DDL)\n"
         "\n"
         "Input\n"
         "    input.ddl: file containing DDL (default: read from stdin)\n");
}

opt_t *
opt_new(int argc, char **argv)
{
    int opt;
    opt_t *options;

    options = malloc(sizeof *options);
    if (!options) return NULL;
    options->input = NULL;
    options->output = NULL;
    opterr = 0; /* prevent getopt() from printing error messages */
    while ((opt = getopt(argc, argv, ":ho:")) != -1) {
        switch (opt) {
            case 'o':
                options->output = strdup(optarg);
                break;

            case 'h':
                print_usage();
                exit(EXIT_SUCCESS);
                break;

            case '?':
                log_error("invalid option '%c'", optopt);
                print_usage();
                exit(EXIT_FAILURE);

            case ':':
                log_error("missing argument for option '%c'", optopt);
                print_usage();
                exit(EXIT_FAILURE);

            default:
                assert(0);
        }
    }
    while (optind < argc) {
        if (options->input) {
            log_error("only one input file is allowed");
            print_usage();
            exit(EXIT_FAILURE);
        }
        options->input = strdup(argv[optind++]);
    }
    return options;
}

void
opt_free(opt_t *options)
{
    if (!options) return;
    free(options->input);
    free(options->output);
    free(options);
}

