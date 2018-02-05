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

#ifndef PNM_H
#define PNM_H

#define PNM_OFFSET(WIDTH,LINE,COLUMN) (3*((LINE)*(WIDTH) + (COLUMN)))

typedef enum
{ 
    PnmRed = 0,
    PnmGreen = 1,
    PnmBlue = 2
} pnmChannel;

typedef enum
{
    PnmAsciiPbm = 0,
    PnmAsciiPgm = 1,
    PnmAsciiPpm = 2,
    PnmRawPbm = 3,
    PnmRawPgm = 4,
    PnmRawPpm = 5,
    PnmAscii = 6, 
    PnmRaw = 7
} pnmType;

typedef struct pnm *pnm;

extern pnm pnm_new(int width, int height, pnmType type); /**/
extern void pnm_free(pnm self); /**/

extern pnm pnm_init(pnm self);
extern pnm pnm_dup(pnm self);

extern pnm pnm_load(char *path); /**/
extern void pnm_save(pnm self, pnmType type, char *path);/**/


extern pnmType pnm_get_type(pnm self);/**/
extern int pnm_get_width(pnm self); /**/
extern int pnm_get_height(pnm self);/**/

extern unsigned char *pnm_make_uchar_rgb_image(pnm self, char *buffer);
extern void pnm_set_uchar_rgb_image(pnm self, unsigned char *buffer);

extern unsigned short *pnm_get_channel(pnm self, unsigned short *buffer, pnmChannel channel);/**/
extern void pnm_set_channel(pnm self, unsigned short *buffer, pnmChannel channel);
/**/
extern unsigned short *pnm_get_image(pnm self);/**/
extern int pnm_offset(pnm self, int line, int column);/**/

extern unsigned short pnm_get_component(pnm self, int i, int j, pnmChannel channel);/**/
extern void pnm_set_component(pnm self, int i, int j, pnmChannel channel, unsigned short v);/**/

extern unsigned short pnm_maxval; /**/

#endif  /* PNM_H */
