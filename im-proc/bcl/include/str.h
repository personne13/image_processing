/* Some utility functions for C strings
 * Copyright (C) 1996, 99, 2003 Achille Braquelaire (achille@labri.fr)
 * 
 * This file is part of BCL (Basic Library for C programs)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation (Inc., 59
 * Temple Place - Suite 330, Boston, MA 02111-1307, USA); either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef STR_H
#define STR_H

typedef enum
{
    Char, Int, Double, String, Eop
} strFormatSpecification;


extern char *str_dup(char *string);
extern char *str_dup_free(char *string);
extern char *str_cat(char *first_string, char *second_string);
extern char *str_catn(char *first_string, ...);

extern char *str_prefix(char *s, char c);
extern char *str_suffix(char *s, char c);

extern char *str_static_storage(char *s);
extern char *str_formated_static_storage(strFormatSpecification start, ...);

#endif  /* STR_H */
