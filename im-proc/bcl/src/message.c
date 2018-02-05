/* Functions for message printing
 * Copyright (C) 1999, 2000, 2001 Achille Braquelaire (achille@labri.fr)
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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "str.h"
#include "message.h"

enum messageMode { Message, Warning, Fatal };

static char *header = NULL;

static void 
message_write(int mode, char *format, va_list parametres)
{
    if (header != NULL)
	fprintf(stderr, "%s: ", header);
    switch (mode)
    {
      case Warning:
	fprintf(stderr, "Warning: ");
	break;

      case Fatal:
	fprintf(stderr, "Error: ");
	break;
    }
    vfprintf(stderr, format, parametres);
}


void 
message(char *format, ...)
{
    va_list parametres;

    va_start(parametres, format);
    message_write(Message, format, parametres);
    va_end(parametres);
}

void 
message_warning(char *format, ...)
{
    va_list parametres;

    va_start(parametres, format);
    message_write(Warning, format, parametres);
    va_end(parametres);
}

void
message_fatal(char *format, ...)
{
    va_list parametres;

    va_start(parametres, format);
    message_write(Fatal, format, parametres);
    va_end(parametres);

    exit(EXIT_FAILURE);
}

void
message_usage(char *format, ...)
{
    va_list parametres;
    
    fprintf(stderr, "Usage: ");
    if (header != NULL)
	fprintf(stderr, "%s ", header);

    va_start(parametres, format);
    vfprintf(stderr, format, parametres);
    fprintf(stderr, "\n");
    va_end(parametres);

    exit(EXIT_FAILURE);
}


void
message_set_header(char *e)
{
    if (header != NULL)
	free(header);
    header = str_dup(e);
}

