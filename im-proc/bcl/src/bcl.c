/* Some basic functions for C programming
 * Copyright (C) 1999, 2000 Achille Braquelaire (achille@labri.fr)
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
#include <float.h>
#include <string.h>

#include "bcl.h"

const double pi = 3.14159265358979323846;

bFlt2 bFlt2_0 = { 0.0, 0.0 };
bFlt2 bFlt2_1 = { 1.0, 1.0 };
bFlt3 bFlt3_0 = { 0.0, 0.0, 0.0 };
bFlt3 bFlt3_1 = { 1.0, 1.0, 1.0 };
bFlt4 bFlt4_0 = { 0.0, 0.0, 0.0, 0.0 };
bFlt4 bFlt4_1 = { 1.0, 1.0, 1.0, 1.0 };

#define B_EPSILON (2*DBL_EPSILON)

static bFlt L_max(bFlt a, bFlt b)
{
    if (a > b)
	return a;
    return b;
}
static bFlt L_abs(bFlt a)
{
    if (a >= 0.0)
	return a;
    return -a;
}
bFlt 
bFlt_max3(bFlt a, bFlt b, bFlt c)
{
    if (a > b)
    {
	if (a > c)
	    return a;
	else
	    return c;
    }
    if (b > c)
	return b;
    else
	return c;
}

bFlt 
bFlt_min3(bFlt a, bFlt b, bFlt c)
{
    if (a < b)
    {
	if (a < c)
	    return a;
	else
	    return c;
    }
    if (b < c)
	return b;
    else
	return c;
}

int 
bFlt_toi(bFlt x)
{
    if (x < 0.0)
	return (int) (x - 0.4999999999);
    else
	return (int) (x + 0.4999999999);
}

int
bFlt_equal(bFlt x1, bFlt x2)
{
    bFlt delta = x1 - x2;

    if (delta < 0.0)
	delta = -delta;

    return delta <= B_EPSILON;
}

bInt2
bInt2_make(int c0, int c1)
{
    bInt2 pair;

    B_ELM(pair, 0) = c0;
    B_ELM(pair, 1) = c1;

    return pair;
}

bInt3
bInt3_make(int c0, int c1, int c2)
{
    bInt3 triplet;

    B_ELM(triplet, 0) = c0;
    B_ELM(triplet, 1) = c1;
    B_ELM(triplet, 2) = c2;

    return triplet;
}

bFlt2
bFlt2_make(bFlt c0, bFlt c1)
{
    bFlt2 pair;

    B_ELM(pair, 0) = c0;
    B_ELM(pair, 1) = c1;

    return pair;
}

bFlt3
bFlt3_make(bFlt c0, bFlt c1, bFlt c2)
{
    bFlt3 triplet;

    B_ELM(triplet, 0) = c0;
    B_ELM(triplet, 1) = c1;
    B_ELM(triplet, 2) = c2;

    return triplet;
}

bFlt4
bFlt4_make(bFlt c0, bFlt c1, bFlt c2, bFlt c3)
{
    bFlt4 quadruplet;

    B_ELM(quadruplet, 0) = c0;
    B_ELM(quadruplet, 1) = c1;
    B_ELM(quadruplet, 2) = c2;
    B_ELM(quadruplet, 3) = c3;

    return quadruplet;
}

bFlt2
bFlt2_set(bFlt2 self, int i, bFlt v)
{
    switch (i)
    {
      case 0:
	B_ELM_SET(self, 0, v);
	break;

      case 1:
	B_ELM_SET(self, 1, v);
	break;

      default:
	RAISE(error, "bFlt2_set: incorrect index");
    }
    return self;
}

bFlt3
bFlt3_set(bFlt3 self, int i, bFlt v)
{
    switch (i)
    {
      case 0:
	B_ELM_SET(self, 0, v);
	break;

      case 1:
	B_ELM_SET(self, 1, v);
	break;

      case 2:
	B_ELM_SET(self, 2, v);
	break;

      default:
	RAISE(error, "bFlt3_set: incorrect index");
    }
    return self;
}

bFlt4
bFlt4_set(bFlt4 self, int i, bFlt v)
{
    switch (i)
    {
      case 0:
	B_ELM_SET(self, 0, v);
	break;

      case 1:
	B_ELM_SET(self, 1, v);
	break;

      case 2:
	B_ELM_SET(self, 2, v);
	break;

      case 3:
	B_ELM_SET(self, 3, v);
	break;

      default:
	RAISE(error, "bFlt4_set: incorrect index");
    }
    return self;
}

bFlt
bFlt2_get(bFlt2 self, int i)
{
    bFlt result;

    switch (i)
    {
      case 0:
	result = B_ELM(self, 0);
	break;

      case 1:
	result = B_ELM(self, 1);
	break;

      default:
	result = 0;
	RAISE(error, "bFlt2_get: incorrect index");
    }
    return result;
}

bFlt
bFlt3_get(bFlt3 self, int i)
{
    bFlt result;

    switch (i)
    {
      case 0:
	result = B_ELM(self, 0);
	break;

      case 1:
	result = B_ELM(self, 1);
	break;

      case 2:
	result = B_ELM(self, 2);
	break;

      default:
	result = 0;
	RAISE(error, "bFlt3_get: incorrect index");
    }
    return result;
}

bFlt
bFlt4_get(bFlt4 self, int i)
{
    bFlt result;

    switch (i)
    {
      case 0:
	result = B_ELM(self, 0);
	break;

      case 1:
	result = B_ELM(self, 1);
	break;

      case 2:
	result = B_ELM(self, 2);
	break;

      case 3:
	result = B_ELM(self, 3);
	break;

      default:
	result = 0;
	RAISE(error, "bFlt4_get: incorrect index");
    }
    return result;
}



bFlt2
bFlt2_by(bFlt2 x, bFlt a)
{
    return bFlt2_make(B_ELM(x, 0)*a, B_ELM(x, 1)*a);
}

bFlt3 
bFlt3_by(bFlt3 x, bFlt a)
{
    return bFlt3_make(B_ELM(x, 0)*a, B_ELM(x, 1)*a, B_ELM(x, 2)*a);
}

bFlt4
bFlt4_by(bFlt4 x, bFlt a)
{
    return bFlt4_make(B_ELM(x, 0)*a, 
		      B_ELM(x, 1)*a, 
		      B_ELM(x, 2)*a, 
		      B_ELM(x, 3)*a);
}


bFlt2
bFlt2_sum(bFlt2 x, bFlt2 y)
{
    return bFlt2_make(B_ELM(x, 0) + B_ELM(y, 0), 
		      B_ELM(x, 1) + B_ELM(y, 1));
}

bFlt3
bFlt3_sum(bFlt3 x, bFlt3 y)
{
    return bFlt3_make(B_ELM(x, 0) + B_ELM(y, 0), 
		      B_ELM(x, 1) + B_ELM(y, 1), 
		      B_ELM(x, 2) + B_ELM(y, 2));
}

bFlt4
bFlt4_sum(bFlt4 x, bFlt4 y)
{
    return bFlt4_make(B_ELM(x, 0) + B_ELM(y, 0), 
		      B_ELM(x, 1) + B_ELM(y, 1), 
		      B_ELM(x, 2) + B_ELM(y, 2), 
		      B_ELM(x, 3) + B_ELM(y, 3));
}

bFlt2
bFlt2_prod(bFlt2 x, bFlt2 y)
{
    return bFlt2_make(B_ELM(x, 0)*B_ELM(y, 0), 
		      B_ELM(x, 1)*B_ELM(y, 1));
}

bFlt3
bFlt3_prod(bFlt3 x, bFlt3 y)
{
    return bFlt3_make(B_ELM(x, 0)*B_ELM(y, 0), 
		      B_ELM(x, 1)*B_ELM(y, 1), 
		      B_ELM(x, 2)*B_ELM(y, 2));
}

bFlt4
bFlt4_prod(bFlt4 x, bFlt4 y)
{
    return bFlt4_make(B_ELM(x, 0)*B_ELM(y, 0), 
		      B_ELM(x, 1)*B_ELM(y, 1), 
		      B_ELM(x, 2)*B_ELM(y, 2), 
		      B_ELM(x, 3)*B_ELM(y, 3));
}

bFlt
bFlt3_max(bFlt3 c)
{
    bFlt c0 = B_ELM(c, 0);
    bFlt c1 = B_ELM(c, 1);
    bFlt c2 = B_ELM(c, 2);

    bFlt m = L_max(c0, c1);

    return L_max(m, c2);
}

bFlt
bFlt3_maxd(bFlt3 c1, bFlt3 c2)
{
    bFlt d0 = L_abs(B_ELM(c1, 0) - B_ELM(c2, 0));
    bFlt d1 = L_abs(B_ELM(c1, 1) - B_ELM(c2, 1));
    bFlt d2 = L_abs(B_ELM(c1, 2) - B_ELM(c2, 2));

    bFlt m1 = L_max(d0, d1);

    return L_max(m1, d2);
}

double
b_degrees_to_radiants(double degrees)
{
    return degrees*pi/180.0;
}

int
b_is_a_leap_year(int year)
{
    if (year%4 != 0)
	return 0;

    if (year%100 != 0)
	return 1;

    return (year/100)%4 == 0;
}

char *base_name(char *path)
{
    int l = strlen(path);
    char *p = (char *) strrchr(path, '/');

    if (p == 0)
        return strcpy(memory_alloc(l+1), path);
    return strcpy(memory_alloc(l-(p-path)), p+1);
}

char *
dir_name(char *path)
{
    char *p = (char *) strrchr(path, '/');

    if (p == 0)
        return str_dup(".");
    else
    {
	int l = p-path;
	char *s = memory_alloc(l+1);

	strncpy(s, path, l);
	s[l] = '\0';
	return s;
    }
}

