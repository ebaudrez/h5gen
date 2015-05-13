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

#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

/* header file for abstracting the logging framework */

#define log_debug(...) log_debug_int(__FILE__, __LINE__, __VA_ARGS__)
#define log_info(...) log_info_int(__FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...) log_warn_int(__FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_error_int(__FILE__, __LINE__, __VA_ARGS__)
extern void log_debug_int(const char *file, int line, const char *format, ...);
extern void log_info_int(const char *file, int line, const char *format, ...);
extern void log_warn_int(const char *file, int line, const char *format, ...);
extern void log_error_int(const char *file, int line, const char *format, ...);

/* interface to lexer/parser */
extern void yyerror(char *format, ...);

#endif /* LOG_H_INCLUDED */
