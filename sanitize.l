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

/*
 * This is a very simple lexer that is intended to reformat the input DDL in
 * the format that h5dump seems to output, in order to prevent spurious
 * differences that would throw off the tests.
 *
 * Among other things, this lexer
 *  - removes shell-style comments
 *  - removes indentation
 *  - reformats floating-point numbers to prevent spurious differences between
 *    "7." and "7", or "2.990" and "2.99"
 */

%option noyywrap

/* real number exponent */
EXP     ([eE][-+]?[0-9]+)

%%

    /* remove shell-style comments (only at the beginning of the line) */
^[ \t]*#.*\n

    /* remove indentation */
^[ \t]+

    /* intercept strings to avoid unwanted recognition of numbers */
\"([^"\\]|\\['"])*\"    { printf("%s", yytext); }

    /* intercept floating-point numbers, and re-output them in the format "%g",
     * as h5tools_str_sprint() seems to do */
-?([0-9]+\.[0-9]*|\.[0-9]+){EXP}? |
-?[0-9]+{EXP}           { printf("%g", atof(yytext)); }

%%

int
main(void)
{
    yylex();
    return 0;
}
