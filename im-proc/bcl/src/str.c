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

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "memory.h"
#include "message.h"
#include "str.h"

#define STATIC_STORAGE_SIZE 4096

static char L_static_storage[STATIC_STORAGE_SIZE];
static int L_static_storage_size = STATIC_STORAGE_SIZE - 1;

char *
str_dup_free(char *string)
{
    char *result = str_dup(string);

    memory_free(string);
    return result;
}

char *
str_dup(char *string)
{
    char *new_string;
    if (string == NULL)
	return NULL;

    HANDLE(memory_default,
	   new_string = memory_alloc(strlen(string)+1));
    if (EXCEPTION_RAISED(memory_default))
	RAISE(memory_default, "str_dup");

    strcpy(new_string, string);
    return new_string;
}

char *
str_cat(char *first_string, char *second_string)
{
    char *result;

    HANDLE(memory_default, 
	   result = memory_alloc(strlen(first_string) 
				 + strlen(second_string) + 1));
    if (EXCEPTION_RAISED(memory_default))
	RAISE(memory_default, "str_cat");

    strcpy(result, first_string);
    strcat(result, second_string);
    return result;
}

char *
str_catn(char *first_string, ...)
{

    size_t total_length = strlen(first_string);
    va_list string_list;
    char *result;
    char *current_pos;

    va_start(string_list, first_string);
    while (true)
    {
	char *p = va_arg(string_list, char *);

	if (p == NULL)
	    break;
	total_length += strlen(p);
    }
    va_end(string_list);

    HANDLE(memory_default, result = memory_alloc(total_length + 1));
    if (EXCEPTION_RAISED(memory_default))
	RAISE(memory_default, "str_catn");
    strcpy(result, first_string);

    current_pos = result + strlen(first_string);

    va_start(string_list, first_string);
    for (;;)
    {
	char *p = va_arg(string_list, char *);

	if (p == NULL)
	    break;

	while (*p != '\0')
	    *current_pos++ = *p++;
    }
    va_end(string_list);
    *current_pos = '\0';

    return result;
}

char *
str_prefix(char *s, char c)
{
    char *pc = strchr(s, c);
    int string_length = 0;
    char *result = NULL;

    if (pc == NULL)
	return str_dup(s);
   
    result = memory_alloc(string_length + 1);
    string_length = pc - s;
    strncpy(result, s, string_length);
    result[string_length] = '\0';

    return result;
}

char *
str_suffix(char *s, char c)
{
    char *pc = strchr(s, c);

    if (pc == NULL)
	return str_dup("");
    else
	return str_dup(pc + 1);
}


static int
L_remaining_size(int string_length)
{
    return L_static_storage_size - string_length;
}

static int
L_check_size(int string_length)
{
    if (L_remaining_size(string_length) < 0)
    {
	static char buffer[256];

	sprintf(buffer, 
		"string static storage: not enough space "
		"(%d required, %d available)\n", 
		string_length, L_static_storage_size);
	message_warning(buffer);

	return 0;
    }
    else
	return 1;
}

char *
str_static_storage(char *s)
{
    int n = strlen(s);

    L_check_size(n);
    strncpy(L_static_storage, s, L_static_storage_size);

    return L_static_storage;
}



char *
str_formated_static_storage(strFormatSpecification start, ...)
{
    va_list parameters;
    strFormatSpecification specification;
    int current_size = 0;
    int position = 0;

    va_start(parameters, start);
    specification = start;

    for (;;)
    {
	char buffer[32];
	int n;
	char *p;

        switch (specification)
        {
          case Char:
	  {
	      /* char c = va_arg(parameters, char); */
	      char c = va_arg(parameters, int);
	      
	      n = sprintf(buffer, "%c", c);
	      p = buffer;
	      break;
	  }

          case Int:
	  {
	      int i = va_arg(parameters, int);
	      
	      n = sprintf(buffer, "%d", i);
	      p = buffer;
	      break;
	  }

          case Double:
	  {
	      double x = va_arg(parameters, double);
	     
	      n = sprintf(buffer, "%g", x);
	      p = buffer;
	      break;
	  }

          case String:
	  {
	      char *s = va_arg(parameters, char *);
	      
	      n = strlen(s);
	      p = s;
	      break;
	  }

	  case Eop :
	    break;
        }

	if (specification == Eop)
	    break;

	if(!L_check_size(current_size + n))
	{
	    strncpy(L_static_storage+position, 
		    p,
		    L_remaining_size(current_size));
	    break;
	}

	strcpy(L_static_storage+position, p);
	position += n;
	current_size += n;
	specification = va_arg(parameters, strFormatSpecification);
    }

    return L_static_storage;
}
