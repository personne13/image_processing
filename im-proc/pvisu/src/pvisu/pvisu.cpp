/* -*- c-basic-offset: 3 -*-
 *
 * Copyright (c), GREYC.
 * All rights reserved
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of the GREYC, nor the name of its
 *     contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 *
 *
 * For more information, refer to:
 * https://clouard.users.greyc.fr/Pandore
 */

/**
 * @file pvisu.cpp
 *
 * Contains the main, builds the images and runs the application
 */

#include <QApplication>
#include <QErrorMessage>
#include <math.h>
#include <iostream>
#include <QDir>
#include <QStringList>
#include <QStringListIterator>


//#define DEBUG

#ifdef DEBUG
#include <time.h>
#endif

bool DRAWINGMODE;

#include "frame.h"
#include "imagemodel.h"
#include "textmodel.h"
#include "imagevisu.h"

char size[128];
char etc[128];

char *progname;

/**
 * Returns the name of the given colorspace.
 * @param c	The colorspace.
 * @return	the name of the colorspace.
 */
const char *ColorName( PColorSpace c ) {
   switch (c) {
   case RGB: return "RGB";
   case XYZ: return "XYZ";
   case LUV: return "LUV";
   case LAB: return "LAB";
   case HSL: return "HSL";
   case AST: return "AST";
   case I1I2I3: return "I1I2I3";
   case LCH: return "LCH";
   case WRY: return "WRY";
   case RNGNBN: return "RNGNBN";
   case YCBCR: return "YCBCR";
   case YCH1CH2: return "YCH1CH2";
   case YIQ: return "YIQ";
   case YUV: return "YUV";       
   default: return "unknown...";
   }
}

/**
 * Draws a line in imd, from coordinate pti to ptf with color color.
 */
void DrawLine( SourceImage *imd, Point3d pti, Point3d ptf, Float color ) {
   Long xmin, ymin, zmin;
   Long xmax, ymax, zmax;
   Long i, j, k;
   Long d;
   float penteyx, pentezx;
   float penteyz;
  
   if (pti.x == ptf.x) {
      penteyx = pentezx = MAXFLOAT;
      xmin = xmax = ptf.x;
   } else {
      d = ptf.x - pti.x;
      penteyx = (float)(ptf.y - pti.y) / d;
      pentezx = (float)(ptf.z - pti.z) / d;
      if (pti.x < ptf.x) {
	 xmin = pti.x;
	 xmax = ptf.x;
      } else {
	 xmin = ptf.x;
	 xmax = pti.x;
      }
   }
   if (pti.y == ptf.y) {
      penteyx = penteyz = 0.0;
      ymin = ymax = ptf.x;
   } else {
      if (pti.y < ptf.y) {
	 ymin = pti.y;
	 ymax = ptf.y;
      } else {
	 ymin = ptf.y;
	 ymax = pti.y; }
   }
   if (pti.z == ptf.z) {
      penteyz = MAXFLOAT;
      pentezx = 0.0;
      zmin = zmax = ptf.z;
   } else {
      penteyz = (float)(ptf.y - pti.y)/(ptf.z - pti.z);
      if (pti.z < ptf.z) {
	 zmin = pti.z;
	 zmax = ptf.z;
      } else {
	 zmin = ptf.z;
	 zmax = pti.z;
      }
   }
  
   if ((pentezx < -1) || (pentezx > 1)) {
      if ((penteyx < -1) || (penteyx > 1)) {
	 if (((pentezx < -1) && (penteyx < pentezx))
	     || ((pentezx > 1) && (penteyx > pentezx))) {
	    for (j = ymin; j <= ymax; j++) {
	       d = j - ptf.y;
	       k = (int)(d / penteyx + ptf.x);
	       i = (int)(d / penteyz + ptf.z);
	       (*imd)(0, i, j, k) = color;
	    }
	 } else {
	    for (i = zmin; i <= zmax; i++) {
	       d = i - ptf.z;
	       j = (int)(d * penteyz + ptf.y);
	       k = (int)(d / pentezx + ptf.x);
	       (*imd)(0, i, j, k) = color;
	    }
	 }
      } else {
	 for (k = xmin; k <= xmax; k++) {
	    d = k - ptf.x;
	    j = (int)(d * penteyx + ptf.y);
	    i = (int)(d * pentezx + ptf.z);
	    (*imd)(0, i, j, k) = color;
	 }
      }
   } else {
      if ((penteyx < -1) || (penteyx > 1)) {
	 for (j = ymin; j <= ymax; j++) {
	    d = j - ptf.y;
	    k = (int)(d / penteyx + ptf.x);
	    i = (int)(d / penteyz + ptf.z);
	    (*imd)(0, i, j, k) = color;
	 }
      } else {
	 for (k = xmin; k <= xmax; k++) {
	    d = k - ptf.x;
	    j = (int)(d * penteyx + ptf.y);
	    i = (int)(d * pentezx + ptf.z);
	    (*imd)(0, i, j, k) = color;
	 }
      }
   }
}


/**
 * @brief Visualization without input file.
 *
 * Creates the source image with <tt>ims</tt> data
 * and displays it thanks to the given graphical application <tt>app</tt>.
 * In case of pdraw program, creates the output
 * image <tt>imd</tt> with user's drawing.
 * @return	SUCCESS or FAILURE
 */
Model *Visu( char *filename ) {
   SourceImage *sourceImage = new SourceImage(1, 1, 256, 256);
   sourceImage->setType("");
   sourceImage->setContent("");
   sourceImage->setSize("");
   sourceImage->setPixel("");
   sourceImage->setMinval(0);
   sourceImage->setMaxval(MAXUINT1);
   sourceImage->setTypeval(SourceImage::tchar);
   sourceImage->setColored(false);
  
   Float *d = sourceImage->Vector(0);
   for ( ; d < sourceImage->Vector(0) + sourceImage->VectorSize(); ) {
      *(d++) = 0;
   }
   return new ImageModel(sourceImage, filename);
}

/**
 * Creates the source image with <tt>ims</tt> data
 * and displays it thanks to the given graphical application <tt>app</tt>.
 * In case of dessin program, creates the output
 * image <tt>imd</tt> with user's drawing.
 *
 * @param ims the input image
 * @param drawImage the output image (for dessin only)
 * @return SUCCESS or FAILURE
 */
Model *Visu( char *filename, Img2duc &ims, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(1, 1, ims.Height(), ims.Width());
   sourceImage->setType("Img2duc");
   sourceImage->setContent("Image, Gray level, 2D");
   snprintf(size, sizeof(size), "%ld rows x %ld cols", (long)ims.Height(), (long)ims.Width());
   sourceImage->setSize(size);
   sourceImage->setPixel(" integer 8-bits");
   sourceImage->setTypeval(SourceImage::tchar);
   sourceImage->setColored(false);
   
   Float *d = sourceImage->Vector(0);
   Uchar *s = ims.Vector();
   const Uchar *end = s + ims.VectorSize();
   Uchar min = *s;
   Uchar max = *s;
   for ( ;s < end; ) {
      if (*s > max) {
	 max = *s;
      } else if (*s < min) {
	 min = *s;
      }
      *(d++) = (Float)*(s++);
   }
   sourceImage->setMinval(min);
   sourceImage->setMaxval(max);

   ims.Delete();
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Img3duc &ims, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(1, ims.Depth(), ims.Height(), ims.Width());
   sourceImage->setType("Img3duc");
   sourceImage->setContent("Image, Gray level, 3D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows x %ld planes", (long)ims.Width(), (long)ims.Height(), (long)ims.Depth());
   sourceImage->setSize(size);
   sourceImage->setPixel("integer 8-bits");
   sourceImage->setTypeval(SourceImage::tchar);
   sourceImage->setColored(false);
   
   Float *d = sourceImage->Vector(0);
   Uchar *s = ims.Vector();
   const Uchar *end = s + ims.VectorSize();
   Uchar min = *s;
   Uchar max = *s;

   for ( ;s < end; ) {
      if (*s > max) {
	 max = *s;
      } else if (*s < min) {
	 min = *s;
      }
      *(d++) = (Float)*(s++);
   }
   sourceImage->setMinval(min);
   sourceImage->setMaxval(max);

   ims.Delete();
   ImageModel *visual;
   
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
       visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}


Model *Visu( char *filename, Imc2duc &ims, Imx3duc *drawImage ) {
#ifdef DEBUG
   clock_t deb, fin;
   deb = clock();
#endif
   SourceImage *sourceImage = new SourceImage(3, 1, ims.Height(), ims.Width());
   sourceImage->setType("Imc2duc");
   sourceImage->setContent("Image, Color level, 2D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows", (long)ims.Width(), (long)ims.Height());
   sourceImage->setSize(size);
   sourceImage->setPixel("integer 8-bits");
   snprintf(etc, sizeof(etc), "Color space</th><td>%s", ColorName(ims.ColorSpace()));
   etc[sizeof(etc) - 1] = 0;
   sourceImage->setEtc(etc);
   sourceImage->setTypeval(SourceImage::tchar);
   sourceImage->setColored(true);

   Uchar min = MAXUCHAR;
   Uchar max = 0;

   Uchar *sx = ims.Vector();
   Float *dx = sourceImage->Vector();
   const Uchar *end = sx + (3 * ims.VectorSize());
   for ( ;sx < end; ) {
      if (*sx > max) {
	 max = *sx;
      } else if (*sx < min) {
	 min = *sx;
      }
      *(dx++) = (Float)*(sx++);
   }
 
   sourceImage->setMinval(min);
   sourceImage->setMaxval(max);

#ifdef DEBUG
   fin = clock();
   printf("Visu........ : %.2fs\n", (float)(fin - deb) / CLOCKS_PER_SEC);
#endif
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   ims.Delete();
   return visual;
}

Model *Visu( char *filename, Img2dsl &ims, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(1, 1, ims.Height(), ims.Width());
   sourceImage->setType("Img2dsl");
   sourceImage->setContent("Image, Gray level 2D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows", (long)ims.Width(), (long)ims.Height());
   sourceImage->setSize(size);
   sourceImage->setPixel("integer 32-bits");
   sourceImage->setTypeval(SourceImage::tlong);
   sourceImage->setColored(false);
 
   Float *d = sourceImage->Vector(0);
   Long *s = ims.Vector();
   Float max = *s;
   Float min = *s;
   const Long *end = s + ims.VectorSize();
   for ( ;s < end; s++, d++ ) {
      if ((*(d) = (Float)*(s)) > max ) {
	 max = *d;
      } else if (*d < min) {
	 min = *d;
      }
   }
   sourceImage->setMaxval(max);
   sourceImage->setMinval(min);
   
   ims.Delete();
   
   ImageModel  *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Img3dsl &ims, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(1, ims.Depth(), ims.Height(), ims.Width());
   sourceImage->setType("Img3dsl");
   sourceImage->setContent("Image, Gray level 3D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows x %ld planes", (long)ims.Width(), (long)ims.Height(), (long)ims.Depth());
   sourceImage->setSize(size);
   sourceImage->setPixel("integer 32-bits");
   sourceImage->setTypeval(SourceImage::tlong);
   sourceImage->setColored(false);

   Float *d = sourceImage->Vector(0);
   Long *s = ims.Vector();
   Float max = *s;
   Float min = *s;
   const Long *end = s + ims.VectorSize();
   for ( ; s < end; s++, d++) {
      if ((*(d) = *(s)) > max) {
	 max = *d;
      } else if (*d < min) {
	 min = *d;
      }
   }
   sourceImage->setMaxval(max);
   sourceImage->setMinval(min);
   ims.Delete();
   
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Imc2dsl &ims, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(3, 1, ims.Height(), ims.Width());
   sourceImage->setType("Imc2dsl");
   sourceImage->setContent("Image, Color,  2D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows", (long)ims.Width(), (long)ims.Height());
   sourceImage->setSize(size);
   sourceImage->setPixel("integer 32-bits");
   snprintf(etc, sizeof(etc), "Color space</th><td>%s", ColorName(ims.ColorSpace()));
   etc[sizeof(etc) - 1] = 0;
   sourceImage->setEtc(etc);
   sourceImage->setTypeval(SourceImage::tlong);
   sourceImage->setColored(true);
   
   Float max = *ims.Vector(0);
   Float min = max;
   for (int b = 0; b < ims.Bands(); b++ ) { 
      Long *sx = ims.Vector(b);
      Float *dx = sourceImage->Vector(b);
      const Long *end = sx + ims.VectorSize();
      for ( ; sx < end; ) {
	 if (*sx > max) {
	    max = *sx;
	 } else if (*sx < min) {
	    min = *sx;
	 }
	 *(dx++) = (Float)*(sx++);
      }
   }
   sourceImage->setMaxval(max);
   sourceImage->setMinval(min);
   
   ims.Delete();  
   
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Imc3dsl &ims, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(3, ims.Depth(), ims.Height(), ims.Width());
   sourceImage->setType("Imc3dsl");
   sourceImage->setContent("Image, Color,  3D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows x %ld planes", (long)ims.Width(), (long)ims.Height(), (long)ims.Depth());
   sourceImage->setSize(size);
   sourceImage->setPixel("integer 32-bits");
   snprintf(etc, sizeof(etc), "Color space</th><td>%s", ColorName(ims.ColorSpace()));
   etc[sizeof(etc) - 1] = 0;
   sourceImage->setEtc(etc);
   sourceImage->setTypeval(SourceImage::tlong);
   sourceImage->setColored(true);
   
   Float max = *ims.Vector(0);
   Float min = max;
   for (int b = 0; b < ims.Bands(); b++) {
      Long *sx = ims.Vector(b);
      Float *dx = sourceImage->Vector(b);
      const Long *end = sx + ims.VectorSize();
      for ( ;sx < end; ) {
	 if (*sx > max) {
	    max = *sx;
	 } else if (*sx < min) {
	    min = *sx;
	 }
	 *(dx++) = (Float)*(sx++);
      }
   }
   sourceImage->setMaxval(max);
   sourceImage->setMinval(min);
   ims.Delete();  
   
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Img2dsf &ims, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(1, 1, ims.Height(), ims.Width());
   sourceImage->setType("Img2dsf");
   sourceImage->setContent("Image, Gray level 2D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows", (long)ims.Width(), (long)ims.Height());
   sourceImage->setSize(size);
   sourceImage->setPixel("float 32-bits");
   sourceImage->setTypeval(SourceImage::tfloat);
   sourceImage->setColored(false);
   
   Float *d = sourceImage->Vector(0);
   Float *s = ims.Vector();
   Float max = *s;
   Float min = *s;
   const Float *end = s + ims.VectorSize();
   for ( ;s < end; d++) {
      if ((*(d) = *(s++)) > max) {
	 max = *d;
      } else if (*d < min) {
	 min = *d;
      }
   }
   sourceImage->setMinval(min);
   sourceImage->setMaxval(max);
   
   ims.Delete();
   
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Img3dsf &ims, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(1, ims.Depth(), ims.Height(), ims.Width());
   sourceImage->setType("Img3dsf");
   sourceImage->setContent("Image, Gray level 3D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows x %ld planes", (long)ims.Width(), (long)ims.Height(), (long)ims.Depth());
   sourceImage->setSize(size);
   sourceImage->setPixel("float 32-bits");
   sourceImage->setTypeval(SourceImage::tfloat);
   sourceImage->setColored(false);
   
   Float *d = sourceImage->Vector(0);
   Float *s = ims.Vector();
   Float max = *s;
   Float min = *s;
   const Float *end = s + ims.VectorSize();
   for (; s < end; d++) {
      if ((*(d) = *(s++)) > max) {
	 max = *d;
      } else if (*d < min) {
	 min = *d;
      }
   }
   sourceImage->setMinval(min);
   sourceImage->setMaxval(max);
   
   ims.Delete();
   
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Imc2dsf &ims, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(3, 1, ims.Height(), ims.Width());
   sourceImage->setType("Imc2dsf");
   sourceImage->setContent("Image, Color,  2D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows", (long)ims.Width(), (long)ims.Height());
   sourceImage->setSize(size);
   sourceImage->setPixel("float 32-bits");
   snprintf(etc, sizeof(etc), "Color space</th><td>%s", ColorName(ims.ColorSpace()));
   etc[sizeof(etc) - 1] = 0;
   sourceImage->setEtc(etc);
   sourceImage->setTypeval(SourceImage::tfloat);
   sourceImage->setColored(true);

   Float max = *ims.Vector(0);
   Float min = max;
   for (int b = 0; b < ims.Bands(); b++) {
      Float *sx = ims.Vector(b);
      Float *dx = sourceImage->Vector(b);
      const Float *end = sx + ims.VectorSize();
      for ( ; sx < end; dx++) {
	 if ((*(dx) = (Float)*(sx++)) > max) {
	    max = *dx;
	 } else if (*dx < min) {
	    min = *dx;
	 }
      }
   }
   sourceImage->setMaxval(max);
   sourceImage->setMinval(min);
   
   ims.Delete();
   
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage); 
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Imc3dsf &ims, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(3, ims.Depth(), ims.Height(), ims.Width());
   sourceImage->setType("Imc3dsf");
   sourceImage->setContent("Image, Color,  3D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows x %ld planes", (long)ims.Width(), (long)ims.Height(), (long)ims.Depth());
   sourceImage->setSize(size);
   sourceImage->setPixel("float 32-bits");
   snprintf(etc, sizeof(etc), "Color space</th><td>%s", ColorName(ims.ColorSpace()));
   etc[sizeof(etc) - 1] = 0;
   sourceImage->setEtc(etc);
   sourceImage->setTypeval(SourceImage::tfloat);
   sourceImage->setColored(true);
   
   Float max = *ims.Vector(0);
   Float min = max;
   for (int b = 0; b < ims.Bands();b++) {
      Float *sx = ims.Vector(b);
      Float *dx = sourceImage->Vector(b);
      const Float *end = ims.Vector(b)+ims.VectorSize();
      for ( ; sx < end; ) {
	 if (*sx > max) {
	    max = *sx;
	 } else if (*sx < min) {
	    min = *sx;
	 }
	 *(dx++) = *(sx++);
      }
   }
   sourceImage->setMaxval(max);
   sourceImage->setMinval(min);
   ims.Delete();
   
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }

   return visual;
}

Model *Visu( char *filename, Reg2d &rgs, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(1, 1, rgs.Height(), rgs.Width());
   sourceImage->setType("Reg2d");
   sourceImage->setContent("Region map, 2D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows", (long)rgs.Width(), (long)rgs.Height());
   sourceImage->setSize(size);
   sourceImage->setLabel("integer 32-bits");
   snprintf(etc, sizeof(etc), "%ld", (long)rgs.Labels());
   sourceImage->setLabelMax(etc);
   sourceImage->setPixel(0);
   sourceImage->setMinval(0);
   sourceImage->setMaxval(rgs.Labels());
   sourceImage->setTypeval(SourceImage::tlong);
   sourceImage->setColored(false);
   sourceImage->setDynamisable(false);
   sourceImage->setRegion(true);

   Float *d = sourceImage->Vector(0);
   Ulong *s = rgs.Vector();
   for ( ; s < rgs.Vector() + rgs.Width() * rgs.Height(); *(d++) = (Float)*(s++)) ;

   rgs.Delete();
   
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Reg3d& rgs, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(1, rgs.Depth(), rgs.Height(), rgs.Width());
   sourceImage->setType("Reg3d");
   sourceImage->setContent("Region map, 3D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows x %ld planes", (long)rgs.Width(), (long)rgs.Height(), (long)rgs.Depth());
   sourceImage->setSize(size);
   sourceImage->setLabel("integer 32-bits");
   snprintf(etc, sizeof(etc), "%ld", (long)rgs.Labels());
   sourceImage->setLabelMax(etc);
   sourceImage->setPixel(0);
   sourceImage->setMinval(0);
   sourceImage->setMaxval(rgs.Labels());
   sourceImage->setTypeval(SourceImage::tlong);
   sourceImage->setColored(false);
   sourceImage->setDynamisable(false);
   sourceImage->setRegion(true);

   Float *d = sourceImage->Vector(0);
   Ulong *s = rgs.Vector();
   for ( ; s < rgs.Vector() + rgs.Depth() * rgs.Width() * rgs.Height(); *(d++) = (Float)*(s++)) ;

   rgs.Delete();

   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Graph2d &grs, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(1, 1, grs.Height(), grs.Width());
   sourceImage->setType("Graph2d");
   sourceImage->setContent("Graph, 2D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows", (long)grs.Width(), (long)grs.Height());
   sourceImage->setSize(size);
   sourceImage->setNode("float, Edge: float");
   snprintf(etc, sizeof(etc), "%ld", (long)grs.Size());
   sourceImage->setTotalNodes(etc);
   sourceImage->setPixel(0);
   sourceImage->setGraph(true);
   sourceImage->setMinval(0);
   sourceImage->setMaxval(grs.Size());
   sourceImage->setTypeval(SourceImage::tlong);
   sourceImage->setColored(false);
   sourceImage->setDynamisable(false);

   int n;
   Float *d = sourceImage->Vector(0);
   for ( ; d < sourceImage->Vector(0) + grs.Width() * grs.Height(); *(d++) = 0) ;

   // First of all : the links with color = 1.
   for (n = 0; n < grs.Size(); n++) {
      if (grs[n]) {
	 for (GEdge *ls = grs[n]->Neighbours(); ls; ls = ls->Next()) {
 	    DrawLine(sourceImage, Point3d(0, grs[n]->seed.y, grs[n]->seed.x), 
		     Point3d(0, grs[ls->Node()]->seed.y, grs[ls->Node()]->seed.x), 1.0);
	 }
      }
   }
   // Then the nodes.
   for (n = 0; n < grs.Size(); n++) {
      if (grs[n]) {
	 (*sourceImage)(0, 0, grs[n]->seed.y, grs[n]->seed.x) = n; //grs[n]->value;
      }
   }
   grs.Delete();   

   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }

   return visual;
}

Model *Visu( char *filename, Graph3d& grs, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(1, grs.Depth(), grs.Height(), grs.Width());
   sourceImage->setType("Graph3d");
   sourceImage->setContent("Graph, 3D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows x %ld planes", (long)grs.Width(), (long)grs.Height(), (long)grs.Depth());
   sourceImage->setSize(size);
   sourceImage->setNode("float, Edge: float");
   snprintf(etc, sizeof(etc), "%ld", (long)grs.Size());
   sourceImage->setTotalNodes(etc);
   sourceImage->setPixel(0);
   sourceImage->setMinval(0);
   sourceImage->setMaxval(grs.Size());
   sourceImage->setTypeval(SourceImage::tlong);
   sourceImage->setColored(false);
   sourceImage->setDynamisable(false);
   sourceImage->setGraph(true);
   int n;   
   Float *d = sourceImage->Vector(0);
   for (; d < sourceImage->Vector(0) + grs.Depth() * grs.Width() * grs.Height(); *(d++) = 0) ;
   
   // First of all : the links.
   for (n = 0; n < grs.Size(); n++) {
      if (grs[n]) {
	 for (GEdge *ls = grs[n]->Neighbours(); ls; ls = ls->Next()) {
	    DrawLine(sourceImage, grs[n]->seed, grs[ls->Node()]->seed, 1.0 /* ls->weight */);
	 }
      }
   }
   // Then the nodes.
   for (n = 0; n < grs.Size(); n++) {
      if (grs[n]) {
	 (*sourceImage)(0, grs[n]->seed.z, grs[n]->seed.y, grs[n]->seed.x) = n; // grs[n]->value;
      }
   }

   grs.Delete();   
   
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Img1duc &ims, Imx3duc *drawImage ) {
   const int height = 256;
   SourceImage *sourceImage = new SourceImage(1, 1, height + 1, ims.Width());
   sourceImage->setType("Img1duc");
   sourceImage->setContent("Image, Gray level, 1D");
   snprintf(size, sizeof(size), "%ld columns", (long)ims.Width());
   sourceImage->setSize(size);
   sourceImage->setPixel(" integer 8-bits");
   sourceImage->setMinval(0);
   sourceImage->setMaxval(MAXUINT1);
   sourceImage->setTypeval(SourceImage::tchar);
   sourceImage->setColored(false);

   for (int x = 0; x < sourceImage->Width(); x++) {
      for (int y = 0; y < sourceImage->Height(); y++) {
   	 (*sourceImage)(0, 0, y, x) = 0;
      }
   }

   int y = 0, y1;
   y = height - ims[0];
   for (int i = 1; i < ims.Width(); i++) {
      y1 = height - ims[i];
      DrawLine(sourceImage, Point3d(0, y, i - 1), Point3d(0, y1, i), 255.0F);
      y = y1;
   }

   ims.Delete();
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Img1dsl &ims, Imx3duc *drawImage ) {
   // Seek for the min and max values.  
   Long *s = ims.Vector();
   Float max = (Float)*s;
   Float min = (Float)*s;
   for ( ;s < ims.Vector() + ims.VectorSize(); s++) {
      if ((Float)(*s) > max) {
	 max = (Float)(*s);
      }
      if ((Float)(*s) < min) {
	 min = (Float)(*s);
      }
   }

   int maxHeight = 500;
   int height;
   if (min >= 0) {
      if (max + 1 < maxHeight) {
	 height = max + 1;
      } else if (max - min + 1 < maxHeight) {
	 height = max - min + 1;
      } else {
	 height = maxHeight;
      }
   } else {
      if (abs(max - min + 1) < maxHeight) {
	 height = abs(max - min + 1);
      } else {
	 height = maxHeight;
      }
   }

   SourceImage *sourceImage = new SourceImage(1, 1, height + 1, ims.Width());
   sourceImage->setType("Img1dsl");
   sourceImage->setContent("Image, Gray level 1D");
   snprintf(size, sizeof(size), "%ld columns", (long)ims.Width());
   sourceImage->setSize(size);
   sourceImage->setPixel("integer 32-bits");
   sourceImage->setTypeval(SourceImage::tlong);
   sourceImage->setColored(false);
   
   for (int x = 0; x < sourceImage->Width(); x++) {
      for (int y = 0; y < sourceImage->Height(); y++) {
	 (*sourceImage)(0, 0, y, x) = 0;
      }
   }
   
   sourceImage->setMaxval(MAXUINT1);
   sourceImage->setMinval(0);

   Float step = (Float)height / (Float)(fabs(max - min + 1));
   int y = 0, y1;
   y = (int)(height - 1 - ((ims[0] - min) * step));
   for (int i = 1; i < ims.Width(); i++) {
      y1 = (int)(height - 1 - ((ims[i] - min) * step));
      DrawLine(sourceImage, Point3d(0, y, i - 1), Point3d(0, y1, i), 255.0F);
      y = y1;
   }
   ims.Delete();

   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Img1dsf &ims, Imx3duc *drawImage ) {
   Float *s = ims.Vector();
   Float max = *s;
   Float min = *s;
   for ( ;s < ims.Vector() + ims.VectorSize(); s++) {
      if ((*s) > max) {
	 max = *s;
      }
      if ((*s) < min) {
	 min = *s;
      }
   }

   SourceImage *sourceImage = new SourceImage(1, 1, 256, ims.Width());
   sourceImage->setType("Img1dsf");
   sourceImage->setContent("Image, Gray level 1D");
   snprintf(size, sizeof(size), "%ld columns", (long)ims.Width());
   sourceImage->setSize(size);
   sourceImage->setPixel("float 32-bits");
   sourceImage->setTypeval(SourceImage::tfloat);
   sourceImage->setColored(false);
   sourceImage->setMaxval(MAXUINT1);
   sourceImage->setMinval(0);

   int height = 500;
   if (max - min + 1 < height) {
      height = max - min + 1;
   }
   if (height <= 1) {
      height = 500;
   }
   
   for (int x = 0; x < sourceImage->Width(); x++)  {
      for (int y = 0; y < sourceImage->Height(); y++) {
	 (*sourceImage)(0, 0, y, x) = 0;
      }
   }
   Float step = 255.0F / (Float)(fabs(max - min));
   for (int i = 0; i < ims.Width(); i++) {
      (*sourceImage)(0, 0, 255 - (int)((ims[i] - min) * step), i) = 255;
   }
   ims.Delete();
   
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Imx2duc &ims, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(1, ims.Bands(), ims.Height(), ims.Width());
   sourceImage->setType("Imx2duc");
   sourceImage->setContent("Image, Multispectral level, 2D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows", (long)ims.Width(), (long)ims.Height());
   sourceImage->setSize(size);
   sourceImage->setPixel("integer 8-bits");
   snprintf(etc, sizeof(etc), "Number of bands</th><td>%d", ims.Bands());
   sourceImage->setEtc(etc);
   sourceImage->setTypeval(SourceImage::tchar);
   sourceImage->setColored(false);

   Uchar min = 0;
   Uchar max = 0;
   Float *d = sourceImage->Vector();
   for (int b = 0; b < ims.Bands(); b++) {
      Uchar *s = ims.Vector(b);
      const Uchar *end = s + ims.VectorSize();
      for ( ; s < end;) {
	 if (*s > max) {
	    max = *s;
	 } else if (*s < min) {
	    min = *s;
	 }
	 *(d++) = (Float)*(s++);
      }
   }
   sourceImage->setMinval((float)min);
   sourceImage->setMaxval((float)max);

   ims.Delete();
   ImageModel *visual;
   
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Imx2dsl &ims, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(1, ims.Bands(), ims.Height(), ims.Width());
   sourceImage->setType("Imx2dsl");
   sourceImage->setContent("Image, Gray level 2D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows", (long)ims.Width(), (long)ims.Height());
   sourceImage->setSize(size);
   sourceImage->setPixel("integer 32-bits");
   snprintf(etc, sizeof(etc), "Number of bands</th><td>%d", ims.Bands());
   sourceImage->setEtc(etc);
   sourceImage->setTypeval(SourceImage::tlong);
   sourceImage->setColored(false);
 
   Uchar min = 0;
   Uchar max = 0;
   Float *d = sourceImage->Vector();
   for (int b = 0; b < ims.Bands(); b++) {
      Long *s = ims.Vector(b);
      const Long *end = s + ims.VectorSize();
      for (; s < end;) {
	 if (*s > max) {
	    max = *s;
	 } else if (*s < min) {
	    min = *s;
	 }
	 *(d++) = (Float)*(s++);
      }
   }
   sourceImage->setMinval(min);
   sourceImage->setMaxval(max);

   ims.Delete();
   ImageModel *visual;
   
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Imx2dsf &ims, Imx3duc *drawImage ) {
   SourceImage *sourceImage = new SourceImage(1, ims.Bands(), ims.Height(), ims.Width());
   sourceImage->setType("Imx2dsf");
   sourceImage->setContent("Image, Multispectral level, 2D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows", (long)ims.Width(), (long)ims.Height());
   sourceImage->setSize(size);
   sourceImage->setPixel("float 32-bits");
   snprintf(etc, sizeof(etc), "Number of bands</th><td>%d", ims.Bands());
   sourceImage->setEtc(etc);
   sourceImage->setTypeval(SourceImage::tfloat);
   sourceImage->setColored(false);

   Float max = 0;
   Float min = 0;
   Float *d = sourceImage->Vector(0);
   for (int b = 0; b < ims.Bands(); b++) {
      Float *s = ims.Vector(b);
      const Float *end = s + ims.VectorSize();
      for (; s < end; d++) {
	 if ((*(d) = *(s++)) > max)
	    max = *d;
	 else if (*d < min)
	    min = *d;
      }
   }
   sourceImage->setMinval(min);
   sourceImage->setMaxval(max);

   ims.Delete();
   ImageModel *visual;
   
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Imx3duc &ims, Imx3duc *drawImage ) {
   if (ims.Bands() <= 0 || ims.Bands() > 3) {
      char text[5120] = "Type: Imx3duc (3D multispectral image of bytes)\n";
      char tmp[100];
      snprintf(tmp, sizeof(tmp), "Number of planes: %d\n", ims.Bands());
      strncat(text, tmp, sizeof(text));
      strncat(text, "\nThis image can't be displayed as such.\n", sizeof(text));
      strncat(text, "Convert it into several viewable images (with pimx2..).\n", sizeof(text));
      return new TextModel(text, filename); 
   }

   SourceImage *sourceImage;
   if (ims.Bands() == 1) {
      sourceImage = new SourceImage(1, ims.Depth(), ims.Height(), ims.Width());
   } else {
      sourceImage = new SourceImage(3, ims.Depth(), ims.Height(), ims.Width());
   }
   sourceImage->setType("Imx3duc");
   sourceImage->setContent("Image, Multispectral level, 3D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows x %ld planes", (long)ims.Width(), (long)ims.Height(), (long)ims.Depth());
   sourceImage->setSize(size);
   sourceImage->setPixel("integer 8-bits");
   snprintf(etc, sizeof(etc), "Number of bands</th><td>%d", ims.Bands());
   sourceImage->setEtc(etc);
   sourceImage->setMinval(0);
   sourceImage->setMaxval(MAXUINT1);
   sourceImage->setTypeval(SourceImage::tchar);
   sourceImage->setColored(true);
  
   for (int b = 0; b < ims.Bands(); b++) {
      Uchar *sx = ims.Vector(b);
      Float *dx = sourceImage->Vector(b);
      const Uchar *end = sx + ims.VectorSize();
      for ( ; sx < end; ) {
 	 *(dx++) = (Float)*(sx++);
      }
   }
   ims.Delete();
  
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Imx3dsl &ims, Imx3duc *drawImage ) {
   if (ims.Bands() <= 0 || ims.Bands() > 3) {
      char text[5120] = "Type: Imx3dsl (3D multispectral image of longs)\n";
      char tmp[100];
      snprintf(tmp, sizeof(tmp), "Number of planes: %d\n", ims.Bands());
      strncat(text, tmp, sizeof(tmp));
      strncat(text, "\nThis image can't be displayed as such.\n", sizeof(text));
      strncat(text, "Convert it into several viewable images (with pimx2..).\n", sizeof(text));
      return new TextModel(text, filename); 
   }

   SourceImage *sourceImage;
   if (ims.Bands() == 1) {
      sourceImage = new SourceImage(1, ims.Depth(), ims.Height(), ims.Width());
   } else {
      sourceImage = new SourceImage(3, ims.Depth(), ims.Height(), ims.Width());
   }
   
   sourceImage->setType("Imx3dsl");
   sourceImage->setContent("Image, Multispectral level, 3D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows x %ld planes", (long)ims.Width(), (long)ims.Height(), (long)ims.Depth());
   sourceImage->setSize(size);
   sourceImage->setPixel("integer 32-bits");
   snprintf(etc, sizeof(etc), "Number of bands</th><td>%ld", (long)ims.Bands());
   sourceImage->setEtc(etc);
   sourceImage->setMinval(0);
   sourceImage->setMaxval(MAXLONG);
   sourceImage->setTypeval(SourceImage::tlong);
   sourceImage->setColored(true);

   Float max = *ims.Vector(0);
   Float min = max;
   for (int b = 0; b < ims.Bands(); b++) {
      Long *sx = ims.Vector(b);
      Float *dx = sourceImage->Vector(b);
      const Long *end = sx + ims.VectorSize();
      for ( ; sx < end; ) {
	 if (*sx > max) {
	    max = *sx;
	 } else if (*sx < min) {
	    min = *sx;
	 }
	 *(dx++) = (Float)*(sx++);
      }
   }
   sourceImage->setMaxval(max);
   sourceImage->setMinval(min);
   ims.Delete();
   
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

Model *Visu( char *filename, Imx3dsf &ims, Imx3duc *drawImage ) {
   if (ims.Bands() <= 0 || ims.Bands() > 3) {
      char text[5120] = "Type: Imx3dsf (3D multispectral image of floats)\n";
      char tmp[100];
      snprintf(tmp, sizeof(tmp), "Number of planes: %d\n", ims.Bands());
      strncat(text, tmp, sizeof(tmp));
      strncat(text, "\nThis image can't be displayed as such.\n", sizeof(text));
      strncat(text, "Convert it into several viewable images (with pimx2..).\n", sizeof(text));
      return new TextModel(text, filename); 
   }

   SourceImage *sourceImage;
   if (ims.Bands() == 1) {
      sourceImage = new SourceImage(1, ims.Depth(), ims.Height(), ims.Width());
   } else {
      sourceImage = new SourceImage(3, ims.Depth(), ims.Height(), ims.Width());
   }
   
   sourceImage->setType("Imx3dsf");
   sourceImage->setContent("Image, Multispectral level, 3D");
   snprintf(size, sizeof(size), "%ld columns x %ld rows x %ld planes", (long)ims.Width(), (long)ims.Height(), (long)ims.Depth());
   sourceImage->setSize(size);
   sourceImage->setPixel("float 32-bits");
   snprintf(etc, sizeof(etc), "Number of bands</th><td>%d", ims.Bands());
   sourceImage->setEtc(etc);
   sourceImage->setTypeval(SourceImage::tfloat);
   sourceImage->setColored(true);

   Float max = *ims.Vector(0);
   Float min = max;
   for (int b = 0; b < ims.Bands(); b++) {
      Float *sx = ims.Vector(b);
      Float *dx = sourceImage->Vector(b);
      const Float *end = sx + ims.VectorSize();
      for ( ; sx < end; dx++) {
	 if ((*(dx) = *(sx++)) > max) {
	    max = *dx;
	 } else if (*dx < min) {
	    min = *dx;
	 }
      }
   }
   sourceImage->setMaxval(max);
   sourceImage->setMinval(min);
   
   ims.Delete();
  
   ImageModel *visual;
   if (DRAWINGMODE) {
      *drawImage = 0;
      visual = new ImageModel(sourceImage, filename, drawImage);
   } else {
      visual = new ImageModel(sourceImage, filename);
   }
   return visual;
}

/**
 * Collection.
 */
Model *Visu( char *filename, Collection &cols ) {
   return new TextModel(cols, filename); 
}

/**
 * Reads input file in objs[0], 
 * and builds output file in objd[0] (for dessin only).
 * @param objs the array of input images.
 * @param objd the array of output images.
 * @return the process number of the viewer or FAILURE.
 */
Model *OpenFile( char *filename, Pobject *objs, Pobject **objd ) {
   Model *visual = NULL;

#ifdef DEBUG
   clock_t deb, fin;
   deb = clock();
#endif

   switch (objs->Type()) {
   case Po_Img2duc: {
      Img2duc* const ims = (Img2duc*)objs;
      Imx3duc* imd = NULL;
      if (ims->Width() <= 0 || ims->Height() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      if (DRAWINGMODE) {
	 objd[0] = new Img2duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   } break;
   case Po_Img2dsl: {
      Img2dsl* const ims = (Img2dsl*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img2duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Img2dsf:{
      Img2dsf* const ims = (Img2dsf*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img2duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Reg2d:{
      Reg2d* const rgs = (Reg2d*)objs;
      if (rgs->Width() <= 0 || rgs->Height() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input region map size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img2duc(rgs->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *rgs, imd);
   }break;
   case Po_Reg3d:{
      Reg3d* const rgs = (Reg3d*)objs;
      if (rgs->Width() <= 0 || rgs->Height() <= 0 || rgs->Depth() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input region map size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img3duc(rgs->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *rgs, imd);
   }break;
   case Po_Graph2d:{
      Graph2d* const grs = (Graph2d*)objs;
      if (grs->Width() <= 0 || grs->Height() <= 0 ) {
	 std::cerr << "Error " << progname << ": Bad input graph size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img2duc(grs->Height(), grs->Width());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *grs, imd);
   }break;
   case Po_Graph3d:{
      Graph3d* const grs = (Graph3d*)objs;
      if (grs->Width() <= 0 || grs->Height() <= 0 || grs->Depth() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input graph size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img3duc(grs->Depth(), grs->Height(), grs->Width());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *grs, imd);
   }break;
   case Po_Imc2duc:{
      Imc2duc* const ims = (Imc2duc*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img2duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Imc2dsl:{
      Imc2dsl* const ims = (Imc2dsl*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img2duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Imc2dsf:{
      Imc2dsf* const ims = (Imc2dsf*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img2duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Img3duc:{
      Img3duc* const ims = (Img3duc*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0 || ims->Depth() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img3duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Img3dsl:{
      Img3dsl* const ims = (Img3dsl*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0 || ims->Depth() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img3duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Img3dsf:{
      Img3dsf* const ims = (Img3dsf*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0 || ims->Depth() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img3duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Imc3duc:{
      Imc3duc* const ims = (Imc3duc*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0 || ims->Depth() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img3duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Imc3dsl:{
      Imc3dsl* const ims = (Imc3dsl*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0 || ims->Depth() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img3duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Imc3dsf:{
      Imc3dsf* const ims = (Imc3dsf*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0 || ims->Depth() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img3duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Img1duc:{
      Img1duc* const ims = (Img1duc*)objs;
      if (ims->Width() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img2duc(256, ims->Width());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Img1dsl:{
      Img1dsl* const ims = (Img1dsl*)objs;
      if (ims->Width() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img2duc(256, ims->Width());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Img1dsf:{
      Img1dsf* const ims = (Img1dsf*)objs;
      if (ims->Width() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img2duc(256, ims->Width());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Imx2duc: {
      Imx2duc* const ims = (Imx2duc*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img2duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   } break;
   case Po_Imx2dsl: {
      Imx2dsl* const ims = (Imx2dsl*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      // can't display more than 3 bands...
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
 	 objd[0] = new Img2duc(ims->Size());
 	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Imx2dsf: {
      Imx2dsf* const ims = (Imx2dsf*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      // can't display more than 3 bands...
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
 	 objd[0] = new Img2duc(ims->Size());
 	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Imx3duc: {
      Imx3duc* const ims = (Imx3duc*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0 || ims->Depth() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      // can't display more than 3 bands...
      if (ims->Bands() <= 0 || ims->Bands()> 3) return NULL;
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img3duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Imx3dsl: {
      Imx3dsl* const ims = (Imx3dsl*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0 || ims->Depth() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      // can't display more than 3 bands...
      if (ims->Bands() <= 0 || ims->Bands() > 3) return NULL;
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img3duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   }break;
   case Po_Imx3dsf: {
      Imx3dsf* const ims = (Imx3dsf*)objs;
      if (ims->Width() <= 0 || ims->Height() <= 0 || ims->Depth() <= 0) {
	 std::cerr << "Error " << progname << ": Bad input image size\n";
	 exit(2);
      }
      // can't display more than 3 bands...
      if (ims->Bands() <= 0 || ims->Bands() > 3) return NULL;
      Imx3duc* imd = NULL;
      if (DRAWINGMODE) {
	 objd[0] = new Img3duc(ims->Size());
	 imd = (Imx3duc*)objd[0];
      }
      visual = Visu(filename, *ims, imd);
   } break;
   case Po_Collection: {
      Collection* const cols = (Collection*)objs;
      visual = Visu(filename, *cols);
   } break;
   default: break;
   }
#ifdef DEBUG
   fin = clock();
   printf("OpenFile.... : %.2fs\n", (float)(fin - deb) / CLOCKS_PER_SEC);
#endif
   return visual;
}

#ifdef MAIN

/*
 * Modify only the following constants, and the function calls.
 */
int main( int argc, char *argv[]) {
   // Check Qt options.
   int argcQt = 1;
   char **argvQt = new char*[argc + 1];
   argvQt[0] = argv[0];
   int argcVisu = 1;
   char **argvVisu = new char*[argc + 1];
   argvVisu[0] = argv[0];
   progname = argv[0];
   
#ifdef DEBUG
   clock_t deb, fin;
   deb = clock();
#endif
   
   // Parses the command line to get the Qt options on the one hand
   // and the visu options on the other hand.
   // Qt options -> all parameters such -xxxx other than -p and -h
   // Visu options: -p -h and the remainder.
   bool noFork = false;
   
   for (int i = 1; i < argc; ) {
      if (argv[i][0] == '-') {
	 if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "-p") || !strcmp(argv[i], "-h")) {
	    argvVisu[argcVisu++] = argv[i++];
	 } else if (!strcmp(argv[i], "-nofork")) {
	    noFork = true;
	    i++;
	 } else {
	    argvQt[argcQt++] = argv[i++];
	    argvQt[argcQt++] = argv[i++];
	 }
      } else {
	 argvVisu[argcVisu++] = argv[i++];
      }
   }
   
   // Whether the program is named visu or dessin
   // the number of output images and usage are different.
#define PROGNAME "pdraw"
   int realFOUTC;
   char usage[256];
   int realFINC;

   if (strlen(argvVisu[0])>= strlen(PROGNAME) &&
       !strcmp(&argvVisu[0][strlen(argvVisu[0]) - strlen(PROGNAME)], PROGNAME)) {
      //cas dessin
      realFINC = 1;
      realFOUTC = 1;
      DRAWINGMODE = true;
      snprintf(usage, sizeof(usage), "usage: %s [option]* [im_in|-] [im_out|-]\n  option:\n    -nofork: place the display as a foreground process", argvVisu[0]);
      usage[sizeof(usage) - 1] = 0;
   } else {
      // cas visu
#ifdef STANDALONE
      if (argcVisu > 1) {
	 realFINC = 1;
      } else {
	 realFINC = 0;
      }
#else
      realFINC = 1;
#endif
      realFOUTC = 0;
      DRAWINGMODE = false;
      snprintf(usage, sizeof(usage), "usage: %s [option]* [im_in|-]\n  option:\n    -nofork: place the display as a foreground process", argvVisu[0]);
      usage[sizeof(usage) - 1] = 0;
   }

   char error[256];
   
   // Check the number of arguments or -p option.
   // Print PROTOTYPE : (name - number of parameters - number of inputs - number of outputsk).
   if ((argcVisu >= 2) && (!strcmp(argvVisu[1], "-p"))) {
      snprintf(error, sizeof(error), "%s %d %d %d", argv[0], 0, realFINC, realFOUTC);
      error[sizeof(error) - 1] = 0;
      std::cout << error << std::endl;
      exit(0);
   }
   
   // Check the number of arguments or -h option.
   // Print USAGE.
   if ((argcVisu <= 0) || ((argcVisu >= 2) && (!strcmp(argvVisu[1], "-h")))) {
      snprintf(error, sizeof(error), usage, argv[0]);
      error[sizeof(error) - 1] = 0;
      std::cerr << error << std::endl;
      exit(0);
   }
   
#if defined _WIN32 || defined WIN32
   if (!strcmp(argv[0], PROGNAME) && argc < 2) {
      Exit(FAILURE);
   }
#endif
   
#undef PROGNAME
   
#ifndef STANDALONE
   if (! (DRAWINGMODE || noFork)) {
      if (argcVisu == 1) {
	 Long pid = fork();
	 switch (pid) {
	 case 0: // child
	    execlp(argv[0], argv[0], "-nofork", NULL);
	    exit(0);
	    break;
	 case -1:
	    exit(-1);
	    break;
	 }
      } else {
	 for (int i = 1; i < argcVisu; i++) {
	    Long pid = fork();
	    switch (pid) {
	    case 0: // child
	       execlp(argv[0], argv[0], "-nofork", argvVisu[i], NULL);
	       exit(0);
	       break;
	    case -1:
	       exit(-1);
	       break;
	    }
	 }
      }
      Exit(SUCCESS);
   }
#endif
   
   // Get the input image name or stdin
   Pobject *mask = NULL;
   Pobject *objin[1];
   Pobject *objs[1];
   Pobject *objout[1];
   Pobject *objd[1];
   char * filename = "";

   int k = 1;
   objs[0] = NULL;

   // If the argument is a directory.
#ifdef sun
   freopen("/dev/null", "r", stderr);
#endif
   if (argcVisu > 1) {
      QDir *directory = new QDir(argvVisu[k]);
      if (directory->exists()) {
	 directory->setFilter(QDir::Files);
	 QStringList filters;
	 filters << "*.pan" << "*.png" << "*.jpg" << "*.jpeg" << "*.JPG" << "*.bmp" << "*.tif" << "*.tiff" << "*.gif"; 
	 QStringList files = directory->entryList(filters);
	 QStringListIterator iterator(files);
	 QString file = 0;
	 int imageCount = files.size();
	 int index = 0;
	 if (iterator.hasNext()) {
	    QString f =  directory->absoluteFilePath(iterator.next());
	    QByteArray ba = f.toLocal8Bit();
	    filename = strdup(ba.data());
	 }
      } else {
	 filename = argvVisu[k];
      }
   }
#ifdef sun
   freopen("/dev/stderr", "r", stderr);
#endif

#ifdef STANDALONE
   if (argcVisu > 1) {
#endif
      objs[0] = LoadFile(((k >= argcVisu) || (!strcmp(argvVisu[k], "-"))) ? NULL : filename, false);
      if (objs[0] == NULL) {
	 if (argcVisu < 2 || (!strcmp(argvVisu[1], "-"))) {
	    fprintf(stderr, "Error %s: can't read standard input with image other than Pandore format\n", progname);
	    Exit(FAILURE);
	 }

#ifdef sun
	 freopen("/dev/null", "r", stderr);
#endif
	 QImage *data = new QImage(filename);
#ifdef sun
	 freopen("/dev/stderr", "r", stderr);
#endif
	 if (!data->isNull()) {
	    if (data->allGray()) {
	       Img2duc *ims = new Img2duc(data->height(), data->width());
	       objs[0] = ims;
	       for (int y = 0; y < data->height(); y++) {
		  for (int x = 0; x < data->width(); x++) {
		     QRgb pixel = data->pixel(x, y);
		     (*ims)(y, x) = qRed(pixel);
		  }
	       }
	    } else {
	       Imc2duc *ims = new Imc2duc(data->height(), data->width());
	       objs[0] = ims;
	       for (int y = 0; y < data->height(); y++) {
		  for (int x = 0; x < data->width(); x++) {
		     QRgb pixel = data->pixel(x, y);
		     (*ims)(0, y, x) = qRed(pixel);
		     (*ims)(1, y, x) = qGreen(pixel);
		     (*ims)(2, y, x) = qBlue(pixel);
		  }
	       }
	    }
	    delete data;
	 }
      }
#ifdef STANDALONE
   }
#endif
   
#if defined _WIN32 || defined WIN32
   if (realFINC > 0 && objs[0] == NULL) {
      std::cerr << "Error " << progname << ": Bad Pandore format: " << filename << std::endl;
      Exit(FAILURE);
   }
#else
   if (objs[0] == NULL) {
      std::cerr << "Error " << progname << ": Bad Pandore format: " << filename << std::endl;
      Exit(FAILURE);
   }
#endif
   
#ifdef sun
   freopen("/dev/null", "r", stderr);
#endif
   
   QApplication app(argcQt, argvQt);
   
   Frame *frame;
   Model *visual = 0;
   
#ifdef DEBUG
   fin = clock();
   printf("\nPrologue..... : %.2fs\n", (float)(fin - deb) / CLOCKS_PER_SEC);
#endif
   if (realFINC > 0) {
      visual = OpenFile(filename, objs[0], objd);
      if (visual) {
	 frame = new Frame();
	 visual->setParent(frame);
	 frame->setVisible(true);
#ifdef DEBUG
	 fin = clock();
	 printf("\nTotal........ : %.2fs\n", (float)(fin - deb) / CLOCKS_PER_SEC);
#endif
	 app.exec();
	 delete frame;
      }
   } else {
      visual = Visu( filename );
      frame = new Frame();
      visual->setParent(frame);
      frame->setVisible(true);
      
      app.exec();
      delete frame;
   }
   
   if (visual) {
      WriteArgs(argcVisu, argvVisu, 0, realFINC, realFOUTC, &mask, objin, objs, objout, objd);
   } else {
      PrintErrorFormat(objin, realFINC);
   }
   
   delete objs[0];
   delete[] argvQt;
   delete[] argvVisu;
   return (visual == NULL);
}

#endif
