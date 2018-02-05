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

#ifndef BCL_H
#define BCL_H

typedef double bFlt;

/*
  Pair of integers
*/
struct bInt2
{
    int _0;
    int _1;
};
typedef struct bInt2 bInt2;

/*
  Triplet of integers
*/
struct bInt3
{
    int _0;
    int _1;
    int _2;
};
typedef struct bInt3 bInt3;

/*
  Pair of floating points
*/
struct bFlt2
{
    bFlt _0;
    bFlt _1;
};
typedef struct bFlt2 bFlt2;

/*
  Triplet of floating points
*/
struct bFlt3
{
    bFlt _0;
    bFlt _1;
    bFlt _2;
};
typedef struct bFlt3 bFlt3;

/*
  Quadruplet of floating points
*/
struct bFlt4
{
    bFlt _0;
    bFlt _1;
    bFlt _2;
    bFlt _3;
};
typedef struct bFlt4 bFlt4;

#define B_ELM(__C__,__I__) ((__C__)._##__I__)
#define B_ELM_SET(__C__,__I__,__V__) (B_ELM(__C__,__I__)=__V__)

extern bInt2 bInt2_make(int i0, int i1);
extern bInt3 bInt3_make(int i0, int i1, int i2);
extern bFlt2 bFlt2_make(bFlt c0, bFlt c1);
extern bFlt3 bFlt3_make(bFlt c0, bFlt c1, bFlt c2);
extern bFlt4 bFlt4_make(bFlt c0, bFlt c1, bFlt c2, bFlt c3);

extern bFlt2 bFlt2_set(bFlt2 self, int i, bFlt v);
extern bFlt3 bFlt3_set(bFlt3 self, int i, bFlt v);
extern bFlt4 bFlt4_set(bFlt4 self, int i, bFlt v);

extern bFlt bFlt2_get(bFlt2 self, int i);
extern bFlt bFlt3_get(bFlt3 self, int i);
extern bFlt bFlt4_get(bFlt4 self, int i);

extern bFlt2 bFlt2_by(bFlt2 x, bFlt a);
extern bFlt3 bFlt3_by(bFlt3 x, bFlt a);
extern bFlt4 bFlt4_by(bFlt4 x, bFlt a);

extern bFlt2 bFlt2_sum(bFlt2 x, bFlt2 y);
extern bFlt3 bFlt3_sum(bFlt3 x, bFlt3 y);
extern bFlt4 bFlt4_sum(bFlt4 x, bFlt4 y);
extern bFlt2 bFlt2_prod(bFlt2 x, bFlt2 y);
extern bFlt3 bFlt3_prod(bFlt3 x, bFlt3 y);
extern bFlt4 bFlt4_prod(bFlt4 x, bFlt4 y);

extern bFlt bFlt3_max(bFlt3 c);
extern bFlt bFlt3_maxd(bFlt3 c1, bFlt3 c2);
extern bFlt bFlt_max3(bFlt a, bFlt b, bFlt c);
extern bFlt bFlt_min3(bFlt a, bFlt b, bFlt c);

extern int bFlt_equal(bFlt x1, bFlt x2);
extern int bFlt_toi(bFlt x);

extern const double pi;

extern double b_degrees_to_radiants(double degrees);
extern int b_is_a_leap_year(int year);
extern char *base_name(char *path);
extern char *dir_name(char *path);

extern bFlt2 bFlt2_0;
extern bFlt2 bFlt2_1;
extern bFlt3 bFlt3_0;
extern bFlt3 bFlt3_1;
extern bFlt4 bFlt4_0;
extern bFlt4 bFlt4_1;

#include "exception.h"
#include "memory.h"
#include "message.h"
#include "str.h"
#include "pnm.h"

#endif  /* BCL_H */
