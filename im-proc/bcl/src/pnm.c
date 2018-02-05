/* PNM: loading and saving pbm/pgm/ppm files
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
#include <string.h>
#include <ctype.h>

#include "bcl.h"

#define COMMENT "Creator: BCL library"

#define OUTPUT_MAXVAL 255U
/* #define INTERNAL_MAXVAL 65535U */
#define INTERNAL_MAXVAL 255U

#define OUTPUT_QUANTIZATION_COEFF ((double)OUTPUT_MAXVAL/(double)INTERNAL_MAXVAL)

static char *L_magic[] = { "P1", "P2", "P3", "P4", "P5", "P6" };

unsigned short pnm_maxval = INTERNAL_MAXVAL;

DEFINE_LOCAL_EXCEPTION(get_int);

struct pnm
{
    pnmType original_type;
    int width;
    int height;
    unsigned short *image;
};

static int
L_is_pnm_whitespace(int c)
{
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}
  
static FILE *
L_r_open(char *path)  
{
    FILE *input;

    if (path == NULL)
	input = stdin;
    else
    {
	input = fopen(path, "r");
	if (input == NULL)
	{
	    static char message[256] = "Cannot read ";

	    strncat(message, path, 200);
	    RAISE(error, message);
	}
    }
    return input;    
}

static FILE *
L_w_open(char *path)  
{
    FILE *output;

    if (path == NULL)
	output = stdout;
    else
    {
	output = fopen(path, "w");
	if (output == NULL)
	{
	    static char message[256] = "Cannot write ";

	    strncat(message, path, 200);
	    RAISE(error, message);
	}
    }
    return output;    
}

static pnmType
L_get_magic(FILE *input)
{
    char magic[3];
    pnmType type;
    
    fread(magic, sizeof(char), 3, input);

    if (strncmp(magic, "P1", 2) == 0)
	type =  PnmAsciiPbm;
    else if (strncmp(magic, "P2", 2) == 0)
	type =  PnmAsciiPgm;
    else if (strncmp(magic, "P3", 2) == 0)
	type =  PnmAsciiPpm;
    else if (strncmp(magic, "P4", 2) == 0)
	type =  PnmRawPbm;
    else if (strncmp(magic, "P5", 2) == 0)
	type =  PnmRawPgm;
    else if (strncmp(magic, "P6", 2) == 0)
	type =  PnmRawPpm;
    else{
      type = PnmRawPpm;
      RAISE(error, "Not a pnm file");
    }
    return type;
}

static int
L_getc(FILE *input)
{
    int c = fgetc(input);

    if (c == '#')
    {
	while (c != '\n')
	    c = fgetc(input);
	c = L_getc(input);
    }
    return c;
}

static int
L_get_ascii_int(FILE *input)
{
    int c;
    int v;
    for (;;)
    {
	c = L_getc(input);

	if (! L_is_pnm_whitespace(c))
	    break;
    }
    if (!isdigit(c))
	RAISE(get_int, "integer missing");

    v = c - '0';
    
    for (;;)
    {
	int c = fgetc(input);
	
	if (c == EOF || L_is_pnm_whitespace(c))
	    break;
	if (isdigit(c))
	    v = 10*v + c - '0';
	else
	    RAISE(get_int, "incorrect integer terminator");
    }
    return v;
}
	
static int
L_get_binary_char(FILE *input)
{
    unsigned char v;

    if (fread(&v, sizeof v, 1, input) != 1)
	RAISE(get_int, NULL);
    return v;
}

static void
L_load_image(pnm self, 
	     FILE *input, 
	     int (*get_int)(FILE *),
	     int maxval, 
	     int dimension, 
	     int invert)
{
    int n = self->width * self->height;
    unsigned short *p = self->image;

    switch (dimension)
    {
      case 1:
	break;

      case 3:
	n *= 3;
	break;

      default:
	RAISE(error, 
	      str_static_storage("L_load_image: incorrect dimension (must be 1 or 3"));
	break;
    }

    while (n > 0)
    {
	int v = get_int(input);
	unsigned short iv;

	if (invert) 
	    v = maxval-v;
	iv = (v*INTERNAL_MAXVAL)/maxval;

	if (dimension == 1)
	{
	    *p++ = iv;
	    *p++ = iv;
	    *p++ = iv;
	}
	else
	    *p++ = iv;

	n--;
    }
}

static void
L_load_binary_pbm_image(pnm self, FILE *input)
{
    unsigned short *p = self->image;
    /* self->width is supposed to be > 0 */
    unsigned int bytes_per_line = ((self->width-1)/8)+1; 
    char *input_line = memory_alloc(bytes_per_line);
    int n;
    for (n = self->height; n > 0; n--)
    {
	unsigned int i;
	int pixels_to_read = self->width;

	if (fread(input_line, sizeof(char), bytes_per_line, input) != 
	    bytes_per_line)
	{
	    memory_free(input_line);
	    RAISE(get_int, NULL);
	}

	for (i = 0; i < bytes_per_line; i++)	    
	{
	    int v = input_line[i];
	    unsigned char mask = 0x80;

	    while (n > 0)
	    {
		unsigned short w =  (v & mask) ? 0 : INTERNAL_MAXVAL;

		*p++ = w;
		*p++ = w;
		*p++ = w;

		mask >>= 1;
		
		pixels_to_read--;
		if (pixels_to_read == 0)
		    break;

		if (mask == 0)
		    break;
	    }
	    if (pixels_to_read == 0)
		break;
	}
    }
    memory_free(input_line);
}


static void
L_load_ascii_pbm(pnm self, FILE *input)
{
    L_load_image(self, input, L_get_ascii_int, 1, 1, 1);
}

static void
L_load_ascii_pgm(pnm self, int maxval, FILE * input)
{
    L_load_image(self, input, L_get_ascii_int, maxval, 1, 0);
}

static void
L_load_ascii_ppm(pnm self, int maxval, FILE *input)
{
    L_load_image(self, input, L_get_ascii_int, maxval, 3, 0);
}

static void
L_load_raw_pbm(pnm self, FILE *input)
{
    L_load_binary_pbm_image(self, input);
}

static void
L_load_raw_pgm(pnm self, int maxval, FILE *input)
{
    L_load_image(self, input, L_get_binary_char, maxval, 1, 0);
}

static void
L_load_raw_ppm(pnm self, int maxval, FILE *input)
{
    L_load_image(self, input, L_get_binary_char, maxval, 3, 0);
}

static pnm
L_init(int width, int height, pnmType type)
{
    pnm self = memory_alloc(sizeof(struct pnm));

    self->width = width;
    self->height = height;
    self->original_type = type;
    self->image = memory_calloc(3*self->width*self->height*sizeof(unsigned short));

    return self;
}
   
static pnm
L_load(FILE *input)  
{
    pnmType type = L_get_magic(input);
    int width = L_get_ascii_int(input);
    int height = L_get_ascii_int(input);
    pnm self = L_init(width, height, type);

    switch (type)
    {
      case PnmAsciiPbm:
	L_load_ascii_pbm(self, input);
	break;

      case PnmRawPbm:
	L_load_raw_pbm(self, input);
	break;

      default:
      {
	  int maxval = L_get_ascii_int(input);

	  switch (type)
	  {
	    case PnmAsciiPgm:
	      L_load_ascii_pgm(self, maxval, input);
	      break;
	      
	    case PnmAsciiPpm:
	      L_load_ascii_ppm(self, maxval, input);
	      break;
	      
	    case PnmRawPgm:
	      L_load_raw_pgm(self, maxval, input);
	      break;
	      
	    case PnmRawPpm:
	      L_load_raw_ppm(self, maxval, input);
	      break;

	    default:
	      RAISE(error, "Unknown pnm type");
	      break;
	  }
      }
    }
    return self;
}

static void
L_save_common_header(pnm self, pnmType type, FILE *output)
{
    fprintf(output, "%s\n", L_magic[type]);
    fprintf(output, "# %s\n", COMMENT);
    fprintf(output, "%d %d\n", self->width, self->height);
}

static void
L_save_maxval(pnm self, FILE *output)
{
  (void) self;
  fprintf(output, "%d\n", OUTPUT_MAXVAL);
}

static void
L_save_ppm_raw(pnm self, FILE *output)
{
    int n = self->width*self->height;
    unsigned short *p = self->image;

    while (n > 0)
    {
	unsigned char pixel[3];

	pixel[0] = (*p++)*OUTPUT_QUANTIZATION_COEFF;
	pixel[1] = (*p++)*OUTPUT_QUANTIZATION_COEFF;
	pixel[2] = (*p++)*OUTPUT_QUANTIZATION_COEFF;

	if (fwrite(pixel, sizeof(char), 3, output) != 3)
	    RAISE(error, "Output error");
	n--;
    }
}

static void
L_save(pnm self, pnmType type, FILE *output)
{
    L_save_common_header(self, type, output);

    switch (type)
    {
      default:
	RAISE(error, "Unknown pnmType");
	break;
	
      case PnmAscii:
      case PnmRaw:
	RAISE(error, "Pnm automatic save not implemented");
	break;

      case PnmAsciiPbm:
	RAISE(error, "\"PnmAsciiPbm\" save not implemented");
	break;

      case PnmRawPbm:
	RAISE(error, "\"PnmRawPbm\" save not implemented");
	break;

      case PnmAsciiPgm:
	RAISE(error, "\"PnmAsciiPgm\" save not implemented");
	break;

      case PnmAsciiPpm:
	RAISE(error, "\"PnmAsciiPpm\" save not implemented");
	break;

      case PnmRawPgm:
	RAISE(error, "\"PnmRawPgm\" save not implemented");
	break;

      case PnmRawPpm:
	L_save_maxval(self, output);
	L_save_ppm_raw(self, output);
	break;
    }
}

    
    
pnm
pnm_load(char *path)
{
    FILE *input = L_r_open(path);
    pnm self = NULL;

    HANDLE(any, self = L_load(input));
    fclose(input);

    if (EXCEPTION_RAISED(any))
    {
	if (self != NULL)
	    memory_free(self);

	if (EXCEPTION_RAISED(get_int))
	    RAISE(error, "Truncated pnm file");

	if (EXCEPTION_RAISED(any))
	    RAISE_AGAIN();
    }
    return self;
}

pnm
pnm_init(pnm self)
{
    return L_init(self->width, self->height, self->original_type);
}

pnm
pnm_dup(pnm self)
{
    pnm result = pnm_init(self);

    memcpy(result->image, 
	   self->image, 
	   3*self->width*self->height*sizeof(unsigned short));

    return result;
}

pnm
pnm_new(int width, int height, pnmType type)
{
    return L_init(width, height, type);
}

void
pnm_free(pnm self)
{
    memory_free(self->image);
    memory_free(self);
}

void 
pnm_save(pnm self, pnmType type, char *path)
{
    FILE *output = L_w_open(path);

    HANDLE(error, L_save(self, type, output));
    fclose(output);
    if (EXCEPTION_RAISED(any))
 	RAISE_AGAIN();
}


unsigned short *
pnm_get_image(pnm self)
{
    return self->image;
}

unsigned char *
pnm_make_uchar_rgb_image(pnm self, char *buffer)
{
    int nb_components = 3*self->width*self->height;
    unsigned char *new_image = (unsigned char*) buffer;

    if (new_image == NULL)
	new_image = memory_alloc(nb_components);
    {
	int n = nb_components;
	unsigned short *p1 = self->image;
	unsigned char *p2 = new_image;

	/* Cannot use memcpy here (short* -> char*) */
	while (n > 0)
	{
	    *p2++ = *p1++;
	    n--;
	}
    }
    return new_image;
}

void
pnm_set_uchar_rgb_image(pnm self, unsigned char *buffer)
{
    int nb_components = 3*self->width*self->height;
    unsigned char *p1 = buffer;
    unsigned short *p2 = self->image;

    /* Cannot use memcpy here (char* -> short*) */
    while (nb_components > 0)
    {
	*p2++ = *p1++;
	nb_components--;
    }
}

unsigned short *
pnm_get_channel(pnm self, unsigned short *buffer, pnmChannel channel)
{
    int nb_components = self->width*self->height;
    unsigned short *new_image = buffer;

    if (new_image == NULL)
	new_image = memory_alloc(nb_components*sizeof(unsigned short));

    {
	int n = nb_components;
	unsigned short *p1 = self->image + channel;
	unsigned short *p2 = new_image;

	while (n > 0)
	{
	    *p2 = *p1;
	    p1 +=3;
	    p2++;
	    n--;
	}
    }
    return new_image;
}

void
pnm_set_channel(pnm self, unsigned short *buffer, pnmChannel channel)
{
    int nb_components = self->width*self->height;
    int n = nb_components;
    unsigned short *p1 = buffer;
    unsigned short *p2 = self->image + channel;

    while (n > 0)
    {
	*p2 = *p1;
	p1 ++;
	p2 += 3;
	n--;
    }
}

int
pnm_offset(pnm self, int line, int column)
{
    int w = self->width;
    int h = self->height;

    if (line < 0)
	RAISE(error, "pnm_offset: line parameter < 0");
    if (line >= h)
	RAISE(error, "pnm_offset: line parameter > image height");

    if (column < 0)
	RAISE(error, "pnm_offset: column parameter < 0");
    if (column >= w)
	RAISE(error, "pnm_offset: column parameter > image width");

    return PNM_OFFSET(w, line, column);
}

int
pnm_get_width(pnm self)
{
    return self->width;
}

int
pnm_get_height(pnm self)
{
    return self->height;
}

pnmType
pnm_get_type(pnm self)
{
    return self->original_type;
}

unsigned short
pnm_get_component(pnm self, int line, int column, pnmChannel channel)
{
    return *(self->image + pnm_offset(self, line, column) + channel);
}

void
pnm_set_component(pnm self, int line, int column, pnmChannel channel, unsigned short v)
{
    *(self->image + pnm_offset(self, line, column) + channel) = v;
}
