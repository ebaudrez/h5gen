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
#include <stdarg.h>
#include <stdio.h>

/* interface to lexer */
extern int yylineno;
extern char *yytext;

void
log_debug_int(const char *file, int line, const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    fprintf(stdout, "debug: ");
    vfprintf(stdout, format, ap);
    fprintf(stdout, " (%s:%d)\n", file, line);
    va_end(ap);
}

void
log_info_int(const char *file, int line, const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    fprintf(stdout, "info: ");
    vfprintf(stdout, format, ap);
    fprintf(stdout, " (%s:%d)\n", file, line);
    va_end(ap);
}

void
log_warn_int(const char *file, int line, const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    fprintf(stderr, "warning: ");
    vfprintf(stderr, format, ap);
    fprintf(stderr, " (%s:%d)\n", file, line);
    va_end(ap);
}

void
log_error_int(const char *file, int line, const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    fprintf(stderr, "error: ");
    vfprintf(stderr, format, ap);
    fprintf(stderr, " (%s:%d)\n", file, line);
    va_end(ap);
}

void
yyerror(char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    fprintf(stderr, "parse error at line %d near '%s': ", yylineno, yytext);
    vfprintf(stderr, format, ap);
    fputs("\n", stderr);
    va_end(ap);
}

