/* -*- mode: c++; c-basic-offset: 3 -*-
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
 * @file imagemodel.cpp
 *
 * Defines the classe that manipulates image content
 * with observers and tools.
 */

#if QT_VERSION >= 0x050000
// Qt5 code
#include <QtWidgets>
#else
// Qt4 code
#include <QtGui>
#endif
#include <QFileDialog>
#include <limits>

#ifdef DEBUG
#include <time.h>
#endif

#include "imagemodel.h"
#include "canvasview.h"
#include "thresholdselector.h"
#include "colorselector.h"
#include "valuesview.h"
#include "propertiesview.h"
#include "histogramview.h"
#include "profileview.h"

using namespace std;

ImageModel::ImageModel( const SourceImage *ims, const char *filename, Imx3duc *imd ): _currentPosition(0,0), _rubberBand(0,0,0,0), _zoom(1.0f), _sourceImage(ims), _logicImage(0), _lastDrawingPoints(), _propertiesWindow(0), _colorWindow(0), _valuesWindow(0), _histogramWindow(0), _thresholdWindow(0), _profileRWindow(0), _profileCWindow(0), _dynamicFunction(Frame::linear) {
#ifdef DEBUG
   clock_t deb, fin;
   deb = clock();
#endif
   if (imd != 0) {
      _outputImage = imd;
      _drawImage = new Img3duc(_outputImage->Props());
      *_drawImage = 0;
      _lastDraw = new Img2duc(_outputImage->Props());
      *_lastDraw = 0;

      _vectX[0] = _vectY[1] = _vectZ[2] = 1;
      _vectX[1] = _vectX[2] = _vectY[0] = _vectY[2] = _vectZ[0] = _vectZ[1] = 0;
   }

   storeFilename(filename);

   if (isnan(_sourceImage->getMinval())
       || isnan(_sourceImage->getMaxval())
       || (_sourceImage->getMinval()==-numeric_limits<float>::infinity())
       || (_sourceImage->getMaxval()==+numeric_limits<float>::infinity())) {
      cout << "Invalid image data: NaN pixel value detected" << endl;
      exit(0); // TODO: do not exit!!!!
   }
   _currentPlane = 0;
   _red = 1;
   _green = 1;
   _blue = 1;
   _brightness = 0;
   _contrast = 0;
   _thresholdL[0] = _thresholdL[1] = _thresholdL[2] = _sourceImage->getMinval();
   _thresholdH[0] = _thresholdH[1] = _thresholdH[2] = _sourceImage->getMaxval();
   buildLogicImage();
   buildOriginalLut();
   buildInitialPhysicImage();

   QVBoxLayout *position = new QVBoxLayout(this);
   position->setContentsMargins(0, 0, 0, 0);

   _scrollImage = new QScrollArea(this);
   CanvasView *imageShow = new CanvasView(this, this);
   _scrollImage->setWidget(imageShow);
   _scrollImage->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
   _scrollImage->setHorizontalScrollBarPolicy (Qt::ScrollBarAsNeeded);
   _scrollImage->viewport()->setBackgroundRole(QPalette::Dark);
   position->addWidget(_scrollImage);

   connect(imageShow, SIGNAL(zoom(bool)),
           this, SIGNAL(zoom(bool)));

   // Set the right size
   const int margin = 40;
   int width = _logicImage->Width() + 10;
   int height = _logicImage->Height() + 2 * _scrollImage->horizontalScrollBar()->height() + 40 / 2;
   if (width > QApplication::desktop()->width() - margin) {
      width = QApplication::desktop()->width() - margin;
   }
   if (height > QApplication::desktop()->height() - margin) {
      height = QApplication::desktop()->height() - margin;
      width += 15;
   }
   resize(width, height);

#ifdef DEBUG
   fin = clock();
   printf("ImageModel.. : %.2fs\n", (float)(fin-deb) / CLOCKS_PER_SEC);
#endif
}

void ImageModel::buildOriginalLut( ) {
   if (!_sourceImage->isGraph() && !_sourceImage->isRegion()) {
      for (int i = 0; i < MAXCOLS; i++) {
	 _lut[0][i] = _lut[1][i] = _lut[2][i] = (Uchar)i;
      }
   } else {
      buildRegionLut();
   }
}

void ImageModel::buildRandomLut( ) {
   for (int i = 0; i < MAXCOLS; i++) {
      _lut[0][i] = (Uchar)rand() % MAXCOLS;
      _lut[1][i] = (Uchar)rand() % MAXCOLS;
      _lut[2][i] = (Uchar)rand() % MAXCOLS;
   }
}

const int COLORS_FOR_REGION = 142;

#include <regionlut.h>

void ImageModel::buildRegionLut( ) {
   ::buildRegionLut(_lut);
}

void ImageModel::buildRainbowLut( ) {
   for (int i = 0; i < MAXCOLS; i++) {
      int j =  MAXCOLS - 1 - i;
      double d = cos((double)((j - MAXCOLS * 0.16) * (M_PI / MAXCOLS)));
      d =  (d < 0.0) ? 0.0 : d;
      _lut[2][i] =  (Uchar)(d * MAXCOLS);
      d =  (d = cos((double)((j - MAXCOLS * 0.52) * (M_PI / MAXCOLS)))) < 0.0 ? 0.0 : d;
      _lut[1][i] =  (Uchar)(d * MAXCOLS);
      d =  (d = cos((double)((j - MAXCOLS * 0.82) * (M_PI / MAXCOLS)))) < 0.0 ? 0.0 : d;
      double e = cos((double)((j  * (M_PI / MAXCOLS))));
      e =  (e < 0.0) ? 0.0 : e;
      _lut[0][i] =  (Uchar)(d * MAXCOLS + e *(MAXCOLS / 2));
   }
}

void ImageModel::buildNoiseLut( ) {
   for (int i = 0; i < MAXCOLS; i++) {
      switch (i % 3) {
      case 0:
    	 _lut[0][i] = 255;
    	 _lut[1][i] = 0;
    	 _lut[2][i] = 0;
    	 break;
      case 1:
    	 _lut[0][i] = 0;
    	 _lut[1][i] = 255;
    	 _lut[2][i] = 0;
    	 break;
      case 2:
    	 _lut[0][i] = 0;
    	 _lut[1][i] = 0;
    	 _lut[2][i] = 255;
    	 break;
      }
   }
}

void ImageModel::buildNegativeLut( ) {
   for (int i = 0; i < MAXCOLS; i++) {
      _lut[0][i] = _lut[1][i] = _lut[2][i] = 255 - i;
   }
}

void ImageModel::buildLogicImage( ) {
   _logicImage = new LogicImage(_sourceImage->Bands(), _sourceImage->Size());
   if (_sourceImage->isGraph()|| _sourceImage->isRegion()) {
      regionEncoding();
   } else {
      switch (_dynamicFunction) {
      case Frame::linear:
      	 linearTransform(MAXUCHAR);
         break;
      case Frame::logarithmic:
      	 logarithmicTransform(MAXUCHAR);
         break;
      case Frame::exponential:
       	 exponentialTransform(MAXUCHAR);
	 break;
      case Frame::equalization:
       	 histogramEqualization(MAXUCHAR);
	 break;
      }
   }
}

void ImageModel::regionEncoding( ) {
   Float *ps = _sourceImage->Vector(0);
   Uchar *pl = _logicImage->Vector(0);
   const Float *end = ps + _sourceImage->VectorSize();
   for (; ps < end; ps++, pl++) {
      unsigned long v = (unsigned long)*ps;
      *pl = (Uchar)(v > 0) ? ((v - 1) % (MAXUCHAR - 1)) + 1 : 0;
   }
}

void ImageModel::linearTransform( int outbands ) {
   Float smin = _sourceImage->getMinval();
   Float smax = _sourceImage->getMaxval();

   // Calculate slope (for all bands).
   Float slope;
   if (smin >= 0.0F) {
      smin = 0.0F;
      if (smax <= outbands)  {
    	 slope = 1.0F;
      } else {
    	 slope = (float)outbands / smax;
      }
   } else {
      if (smax == smin) {
    	 slope = 1.0F;
      } else {
    	 slope = (float)(outbands) / (smax - smin);
      }
   }
   // Linear transform.
   for (int b = 0; b < _sourceImage->Bands();b++) {
      Float *ps = _sourceImage->Vector(b);
      Uchar *pl = _logicImage->Vector(b);
      // Rem: With this constant -> we earn half time.
      const Float *end = ps + _sourceImage->VectorSize();
      for ( ;ps < end; ps++, pl++) {
	    *pl = (Uchar)(slope*(*ps - smin));
      }
   }
}

void ImageModel::logarithmicTransform( int outbands ) {
   Float smin = _sourceImage->getMinval();
   Float smax = _sourceImage->getMaxval();

   const double c = ((double)outbands / (log(smax - smin + 1.0)));
   for (int b = 0; b < _sourceImage->Bands(); b++) {
      Float *ps = _sourceImage->Vector(b);
      Uchar *pl = _logicImage->Vector(b);
      const Float *end = ps + _sourceImage->VectorSize();
      for (; ps < end; ps++, pl++) {
    	 *pl = (Uchar)(c * log(*ps - smin + 1.0));
      }
   }
}

void ImageModel::exponentialTransform( int outbands ) {
   Float smin = _sourceImage->getMinval();
   Float smax = _sourceImage->getMaxval();

   const double c = (((double)smax - smin) / (log((double)outbands + 1.0)));
   for (int b = 0; b < _sourceImage->Bands(); b++) {
      Float *ps = _sourceImage->Vector(b);
      Uchar *pl = _logicImage->Vector(b);
      const Float *end = ps + _sourceImage->VectorSize();
      for (; ps < end; ps++, pl++) {
    	 *pl = (Uchar)(exp((double)(*ps - smin) / c) - 1.0);
      }
   }
}

void ImageModel::histogramEqualization( int outbands ) {
   map<Float, float> histo;
   map<Float, float> histoc;

   // Compute histogram and min-max values.
   for (int b = 0; b < _sourceImage->Bands();b++) {
      Float *ps = _sourceImage->Vector(b);
      const Float *end = ps + _sourceImage->VectorSize();
      for (; ps < end; ps++) {
    	 histo[*ps]++;
      }
      // Compute cumulated histogram.
      // (discard h[min] -> set histoc[min] = 0)
      // This is usefull to begin exactly the output histogram
      // at min (either minout or minval)
      float sum = 0;
      map<Float, float>::const_iterator h;
      h = histo.begin();
      float h0 = histo[h->first];
      histoc[h->first] = 0.0F;
      h++;
      float total = _sourceImage->VectorSize() - h0;
      for (; h != histo.end(); h++) {
    	 sum = histoc[h->first] = sum + h->second;
    	 histoc[h->first] = histoc[h->first] / total;
      }
      ps = _sourceImage->Vector(b);
      Uchar *pl = _logicImage->Vector(b);
      for (; ps < end; ps++, pl++) {
    	 *pl = (Uchar)(histoc[*ps] * (float)outbands);
      }
      histo.clear();
      histoc.clear();
   }
}


/*
 * Thresholding:
 * if thresholdLow <= ThresholdHigh then thresholding out.
 * if thresholdLow > ThresholdHigh then thresholding in.
 */
void ImageModel::buildInitialPhysicImage( ) {
   // Tip: Use b0, b1 and b2 for each plane: b0 = b1 = b2 for gray images.
   int b0 = 0, b1 = 0, b2 = 0;
   const int w = _logicImage->Width();
   const int h = _logicImage->Height();

   if (_logicImage->Bands() > 1) {
      b1 = 1;
      b2 = 2;
   }
   _physicImage = QImage(w, h, QImage::Format_RGB32);

   const Uchar *pixelLR = &((*_logicImage)(b0, _currentPlane, 0, 0));
   const Uchar *pixelLG = &((*_logicImage)(b1, _currentPlane, 0, 0));
   const Uchar *pixelLB = &((*_logicImage)(b2, _currentPlane, 0, 0));

   for (int y = 0; y < h; y++) {
      uint *p = reinterpret_cast<uint *>(_physicImage.scanLine(y));
      for (int x = 0; x < w; x++) {
     	 int r = 0; int g = 0; int b = 0;
	 r = _lut[0][*pixelLR++];
	 g = _lut[1][*pixelLG++];
	 b = _lut[2][*pixelLB++];
	 *(p++) = qRgb(r, g, b);
      }
   }
}

/*
 * Thresholding:
 * if thresholdLow <= ThresholdHigh then thresholding out.
 * if thresholdLow > ThresholdHigh then thresholding in.
 */
void ImageModel::buildPhysicImage( ) {
   // Tip: Use b0, b1 and b2 for each plane: b0 = b1 = b2 for gray images.
   int b0 = 0, b1 = 0, b2 = 0;
   const int w = _logicImage->Width();
   const int h = _logicImage->Height();

   if (_logicImage->Bands() > 1) {
      b1 = 1;
      b2 = 2;
   }
   _physicImage = QImage(w, h, QImage::Format_RGB32);

   const Float *pixelSR = &((*_sourceImage)(b0, _currentPlane, 0, 0));
   const Float *pixelSG = &((*_sourceImage)(b1, _currentPlane, 0, 0));
   const Float *pixelSB = &((*_sourceImage)(b2, _currentPlane, 0, 0));

   const Uchar *pixelLR = &((*_logicImage)(b0, _currentPlane, 0, 0));
   const Uchar *pixelLG = &((*_logicImage)(b1, _currentPlane, 0, 0));
   const Uchar *pixelLB = &((*_logicImage)(b2, _currentPlane, 0, 0));

   for (int y = 0; y < h; y++) {
      uint *p = reinterpret_cast<uint *>(_physicImage.scanLine(y));
      for (int x = 0; x < w; x++) {
     	 int r = 0; int g = 0; int b = 0;

	 if (_thresholdL[b0] <= _thresholdH[b0]) {
	    if (*pixelSR < _thresholdL[b0] || *pixelSR > _thresholdH[b0]) {
	       r = _lut[0][0];
	    } else {
	       r = _lut[0][*pixelLR];
	    }
	 } else {
	    if (*pixelSR > _thresholdL[b0] || *pixelSR < _thresholdH[b0]) {
	       r = _lut[0][0];
	    } else {
	       r = _lut[0][*pixelLR];
	    }
	 }
	 pixelSR++;
	 pixelLR++;
	 if (_thresholdL[b1] <= _thresholdH[b1]) {
	    if (*pixelSG < _thresholdL[b1] || *pixelSG > _thresholdH[b1]) {
	       g = _lut[1][0];
	    } else {
	       g = _lut[1][*pixelLG];
	    }
	 } else {
	    if (*pixelSG > _thresholdL[b1] || *pixelSG < _thresholdH[b1]) {
	       g = _lut[1][0];
	    } else {
	       g = _lut[1][*pixelLG];
	    }
	 }
	 pixelSG++;
	 pixelLG++;
	 if (_thresholdL[b2] <= _thresholdH[b2]) {
	    if (*pixelSB < _thresholdL[b2] || *pixelSB > _thresholdH[b2]) {
	       b = _lut[2][0];
	    } else {
	       b = _lut[2][*pixelLB];
	    }
	 } else {
	    if (*pixelSB > _thresholdL[b2] || *pixelSB < _thresholdH[b2]) {
	       b = _lut[2][0];
	    } else {
	       b = _lut[2][*pixelLB];
	    }
	 }
	 pixelSB++;
	 pixelLB++;
	 *(p++) = setRgb(r * _red, g * _green, b * _blue);
      }
   }
   if (_zoom != 1) {
      // Recreate the image with the specified factor zoom
      _physicImage = _physicImage.scaled((int)(w * _zoom),
					 (int)(h * _zoom),
					 Qt::KeepAspectRatioByExpanding, Qt::FastTransformation);
   }
}

QRgb ImageModel::setRgb( Uchar r, Uchar g, Uchar b ) {
   float mx = (float)r;
   int maxVal = 0;
   if (g > mx) {
      mx = (float)g;
      maxVal = 1;
   }
   if (b > mx) {
      mx = (float)b;
      maxVal = 2;
   }
   float mn = (float)r;
   if (g < mn) {
      mn = (float)g;
   }
   if (b < mn) {
      mn = (float)b;
   }
   float  delta = mx - mn;

   int v = (int)mx;
   float h = 0;
   float s;
   if (mx != 0) {
      s = delta / mx;
      if (s == 0) {
    	 h = -1;
      } else {
    	 switch (maxVal) {
    	 case 0: { // yel < h < mag
	    h = ( g - b ) / delta;
	    break;
	 }
    	 case 1: { // cyan < h < yel
	    h = 2 + ( b - r ) / delta;
	    break;
	 }
    	 case 2: {  // mag < h < cyan
	    h = 4 + ( r - g ) / delta;
	    break;
	 }
    	 }
      }
   } else {
      s = 0;
      h = 0;
   }
   h *= 60;
   if (h < 0) {
      h += 360;
   }

   // Apply brightness
   v += _brightness >> 1;
   if (v >= MAXCOLS) {
      v = MAXCOLS - 1;
   }
   if (v < 0) {
      v = 0;
   }

   if (s == 0.0 || h == -1.0) {
      // s == 0? Totally unsaturated = grey so R, G and B all equal value
      r = g = b = v;
      // Apply contrast
      if (r < 127) {
    	 if (r - _contrast < 0) {
	    r = 0;
	 } else if (r - _contrast>127) {
	    r = 127;
	 } else {
	    r -= _contrast;
	 }
      } else if (r > 127) {
    	 if (r + _contrast < 127) {
	    r = 127;
	 } else if (r + _contrast >= MAXCOLS) {
	    r = MAXCOLS - 1;
	 } else {
	    r += _contrast;
	 }
      }
      return qRgb(r, r, r);
   }
   float hTemp = h / 60.0f;
   int   i = (int)floor(hTemp);    // which sector
   float f = hTemp - i;            // how far through sector
   float p = v  *(1 - s);
   float q = v  *(1 - s  * f);
   float t = v  *(1 - s  * (1 - f));

   switch (i) {
      case 0: {
	 r = (Uchar)v;
	 g = (Uchar)t;
	 b = (Uchar)p;
	 break;
      }
      case 1: {
	 r = (Uchar)q;
	 g = (Uchar)v;
	 b = (Uchar)p;
	 break;
      }
      case 2: {
	 r = (Uchar)p;
	 g = (Uchar)v;
	 b = (Uchar)t;
	 break; }
      case 3: {
	 r = (Uchar)p;
	 g = (Uchar)q;
	 b = (Uchar)v;
	 break; }
      case 4: {
	 r = (Uchar)t;
	 g = (Uchar)p;
	 b = (Uchar)v;
	 break;
      }
      case 5: {
	 r = (Uchar)v;
	 g = (Uchar)p;
	 b = (Uchar)q;
	 break;
      }
   }

   // apply the contrast
   if (r < 127) {
      if (r - _contrast < 0) {
	 r = 0;
      } else if (r - _contrast > 127) {
	 r = 127;
      } else {
	 r -= _contrast;
      }
   } else if (r > 127) {
      if (r + _contrast < 127) {
	 r = 127;
      } else if (r + _contrast >= MAXCOLS) {
	 r = MAXCOLS - 1;
      } else {
	 r += _contrast;
      }
   }
   if (g < 127) {
      if (g - _contrast < 0) {
	 g = 0;
      } else if (g  -_contrast > 127) {
	 g = 127;
      } else {
	 g -= _contrast;
      }
   } else if (g > 127) {
      if (g + _contrast < 127) {
	 g = 127;
      } else if (g + _contrast >= MAXCOLS) {
	 g = MAXCOLS-1;
      } else {
	 g += _contrast;
      }
   }
   if (b < 127) {
      if (b - _contrast < 0) {
	 b = 0;
      } else if (b - _contrast > 127) {
	 b = 127;
      } else {
	 b -= _contrast;
      }
   } else if (b > 127) {
      if (b + _contrast < 127) {
	 b = 127;
      } else if (b  +_contrast >= MAXCOLS) {
	 b = MAXCOLS - 1;
      } else {
	 b += _contrast;
      }
      }

   return qRgb(r, g, b);
}

void ImageModel::callUpdate() {
   emit askForUpdate();
}

void ImageModel::startRubberBand( int x, int y ) {
   if (x < _sourceImage->Width() && x >= 0) {
      _rubberBand.setX(x);
   }
   if (y < _sourceImage->Height() && y >= 0) {
      _rubberBand.setY(y);
   }
   _rubberBand.setWidth(0);
   _rubberBand.setHeight(0);
   emit rubberBandSelection(false);
}

void ImageModel::endRubberBand( int x, int y ) {
   if (x < 0) {
      x = 0;
   }
   if (x >= _sourceImage->Width()) {
      x = _sourceImage->Width() - 1;
   }
   if (y < 0) {
      y = 0;
   }
   if (y >= _sourceImage->Height()) {
      y = _sourceImage->Height() - 1;
   }
   int width = x - _rubberBand.x();
   int height = y - _rubberBand.y();
   _rubberBand.setWidth(width);
   _rubberBand.setHeight(height);
   if (width > 0 && height > 0) {
      emit rubberBandSelection(true);
   }
}

QRect ImageModel::getRubberBand( float zoom ) {
   QRect rect;
   rect.setX((int)(_rubberBand.x() * zoom));
   rect.setY((int)(_rubberBand.y() * zoom));
   rect.setWidth((int)(_rubberBand.width() * zoom));
   rect.setHeight((int)(_rubberBand.height() * zoom));
   return rect;
}

void ImageModel::saveSelection( const QString filename, const QString extension ) {
   if (QString::compare(extension, "bmp") == 0) {
      _physicImage.copy(getRubberBand(_zoom)).save(filename, "BMP");
   } else if ((QString::compare(extension, "jpg") == 0)
	      || (QString::compare(extension, "jpeg") == 0)) {
      _physicImage.copy(getRubberBand(_zoom)).save(filename, "JPG");
   } else if ((QString::compare(extension, "tif") == 0)
	      || (QString::compare(extension, "tiff") == 0)) {
      _physicImage.copy(getRubberBand(_zoom)).save(filename, "TIF");
   } else if (QString::compare(extension, "png") == 0) {
      _physicImage.copy(getRubberBand(_zoom)).save(filename, "PNG");
   } else if (QString::compare(extension, "pan") == 0) {
      if (_logicImage->Bands() == 1 &&
	  strcmp("Reg2d", _sourceImage->getType())) {
	 QRect selection = getRubberBand(_zoom);
	 Img2duc *tmp = new Img2duc(selection.height(), selection.width());
	 for (int y = 0; y < selection.height(); y++) {
	    for (int x = 0; x < selection.width(); x++) {
	       QRgb pixel = _physicImage.pixel(x+selection.x(), y + selection.y());
	       (*tmp)(y, x) = qRed(pixel);
	    }
	 }
	 QByteArray ba = filename.toLocal8Bit();
	 tmp->SaveFile(ba.data());
	 delete tmp;
      } else {
	 QRect selection = getRubberBand(_zoom);
	 Imc2duc *tmp = new Imc2duc(selection.height(), selection.width());
	 for (int y = 0; y < selection.height(); y++) {
	    for (int x = 0; x < selection.width(); x++) {
	       QRgb pixel = _physicImage.pixel(x + selection.x(), y + selection.y());
	       (*tmp)(0, y, x) = qRed(pixel);
	       (*tmp)(1, y, x) = qGreen(pixel);
	       (*tmp)(2, y, x) = qBlue(pixel);
	    }
	 }
	 QByteArray ba = filename.toLocal8Bit();
	 tmp->SaveFile(ba.data());
	 delete tmp;
      }
   } else {
      QMessageBox::critical(this, "Save current image",
			    QString("Unsupported file extension:")
			    + extension
			    +"\nOnly pan, jpg, gif, bmp, tiff, ppm and png formats are supported.\n"
			    +"Image not saved.");
   }
}

void ImageModel::save( const QString filename, const QString extension ) {
   if (QString::compare(extension, "bmp") == 0) {
      _physicImage.save(filename, "BMP");
   } else if ((QString::compare(extension, "jpg") == 0)
	      || (QString::compare(extension, "jpeg") == 0)) {
      _physicImage.save(filename, "JPG");
   } else if ((QString::compare(extension, "tif") == 0)
	      || (QString::compare(extension, "tiff") == 0)) {
      _physicImage.save(filename, "TIF");
   } else if (QString::compare(extension, "png") == 0) {
      _physicImage.save(filename, "PNG");
   } else if (QString::compare(extension, "pan") == 0) {
      if (_logicImage->Bands() == 1 &&
	  strcmp("Reg2d", _sourceImage->getType())) {
	 Img2duc *tmp = new Img2duc(_logicImage->Height() * _zoom,
				    _logicImage->Width() * _zoom);
	 for (int y = 0; y < _physicImage.height(); y++) {
	    for (int x = 0; x < _physicImage.width(); x++) {
	       QRgb pixel = _physicImage.pixel(x, y);
	       (*tmp)(y, x) = qRed(pixel);
	    }
	 }
	 QByteArray ba = filename.toLocal8Bit();
	 tmp->SaveFile(ba.data());
	 delete tmp;
      } else {
	 Imc2duc *tmp = new Imc2duc(_logicImage->Height() * _zoom,
				    _logicImage->Width() * _zoom);
	 for (int y = 0; y < _physicImage.height(); y++) {
	    for (int x = 0; x < _physicImage.width(); x++) {
	       QRgb pixel = _physicImage.pixel(x, y);
	       (*tmp)(0, y, x) = qRed(pixel);
	       (*tmp)(1, y, x) = qGreen(pixel);
	       (*tmp)(2, y, x) = qBlue(pixel);

	    }
	 }
	 QByteArray ba = filename.toLocal8Bit();
	 tmp->SaveFile(ba.data());
	 delete tmp;
      }
   } else {
      QMessageBox::critical(this, "Save current image",
			    QString("Unsupported file extension:")
			    + extension
			    +"\nOnly pan, jpg, gif, bmp, tiff, ppm and png formats are supported.\n"
			    +"Image not saved.");
   }
}

void ImageModel::showPropertiesWindow( ) {
   if (!_propertiesWindow) {
      _propertiesWindow = new PropertiesView(this);
      _propertiesWindow->move(_frame->pos());
   }
   _propertiesWindow->show();
   _propertiesWindow->raise();
}

void ImageModel::showThresholdingWindow( ) {
   if (!_thresholdWindow) {
      _thresholdWindow = new ThresholdSelector(this);
      _thresholdWindow->move(_frame->pos());
   }
   _thresholdWindow->show();
   _thresholdWindow->raise();
}

void ImageModel::showColorsWindow( ) {
   if (!_colorWindow) {
      _colorWindow = new ColorSelector(this);
      _colorWindow->move(_frame->pos());
   }
   _colorWindow->show();
   _colorWindow->raise();
}

void ImageModel::showValuesWindow( ) {
   if (!_valuesWindow) {
      _valuesWindow = new ValuesView(this);
      _valuesWindow->move(_frame->pos());
   }
   _valuesWindow->show();
   _valuesWindow->raise();
}

void ImageModel::showHistogramWindow( ) {
   if (!_histogramWindow) {
      _histogramWindow = new HistogramView(this);
      _histogramWindow->move(_frame->pos());
   }

   _histogramWindow->show();
   _histogramWindow->raise();
}

void ImageModel::showRowProfileWindow( ) {
   if (!_profileRWindow) {
      _profileRWindow = new ProfileView(this, true);
      _profileRWindow->move(_frame->pos());
   }
   _profileRWindow->show();
   _profileRWindow->raise();
}

void ImageModel::showColumnProfileWindow( ) {
   if (!_profileCWindow) {
      _profileCWindow = new ProfileView(this, false);
      _profileCWindow->move(_frame->pos());
   }
   _profileCWindow->show();
   _profileCWindow->raise();
}

// TODO
void ImageModel::changeZoom( float zoom ) {
   if (_zoom == zoom) { return; }
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   _zoom = zoom;
   buildPhysicImage();
   callUpdate();
   int x, y;
   if (_physicImage.width() <= _scrollImage->width()
       || (int)(_currentPosition.x() * _zoom) < _scrollImage->width() / 2) {
      x = 0;
   } else if (_physicImage.width()-(int)(_currentPosition.x() * _zoom) >= _scrollImage->width() / 2) {
      x = (int)(_currentPosition.x() * _zoom)-_scrollImage->width() / 2;
   } else {
      x = (_physicImage.width() - _scrollImage->width() + _scrollImage->verticalScrollBar()->width() + 2);
   }

   if (_physicImage.height() <= _scrollImage->height()
       || (int)(_currentPosition.y()* _zoom) < _scrollImage->height() / 2) {
      y = 0;
   } else if (_physicImage.height()  -(int)(_currentPosition.y() * _zoom) >= _scrollImage->height() / 2) {
      y = (int)(_currentPosition.y() * _zoom) - _scrollImage->height() / 2;
   } else {
      y = (_physicImage.height() - _scrollImage->height() + _scrollImage->horizontalScrollBar()->height() + 2);
   }
   _scrollImage->horizontalScrollBar()->setValue(x);
   _scrollImage->verticalScrollBar()->setValue(y);
   QApplication::restoreOverrideCursor();
}

void ImageModel::changePlane( int p ) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   _currentPlane = (long)p;
   buildPhysicImage();
   callUpdate();
   QApplication::restoreOverrideCursor();
}

void ImageModel::changeThresholdL( int band, float thresholdL ) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   _thresholdL[band] = thresholdL;
   buildPhysicImage();
   callUpdate();
   QApplication::restoreOverrideCursor();
}

void ImageModel::changeThresholdH( int band, float thresholdH ) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   _thresholdH[band] = thresholdH;
   buildPhysicImage();
   callUpdate();
   QApplication::restoreOverrideCursor();
}

void ImageModel::changeDynamicFunction( int id ) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   _dynamicFunction = id;
   buildLogicImage();
   buildPhysicImage();
   callUpdate();
   QApplication::restoreOverrideCursor();
}

void ImageModel::rotateX( ) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   if (DRAWINGMODE) {
      rotateDrawingX();
      delete _lastDraw;
      _lastDraw = new Imx3duc(1, 1, _sourceImage->Depth(), _sourceImage->Width());
      cleanLastDraw();
   }
   SourceImage *temp = new SourceImage(_sourceImage->Bands(), _sourceImage->Height(), _sourceImage->Depth(), _sourceImage->Width());
   for (int b = 0; b < temp->Bands(); b++) {
      float *pt = temp->Vector(b);
      for (int z = 0; z < temp->Depth(); z++) {
    	 for (int y = 0; y < temp->Height(); y++) {
    	    for (int x = 0; x < temp->Width(); x++) {
    	       *(pt++) = (*_sourceImage)(b, y, _sourceImage->Height() - (z + 1), x);
	    }
	 }
      }
   }
   temp->setInfo(_sourceImage->getInfo());
   temp->setMaxval(_sourceImage->getMaxval());
   temp->setMinval(_sourceImage->getMinval());
   temp->setGraph(_sourceImage->isGraph());
   temp->setRegion(_sourceImage->isRegion());
   temp->setColored(_sourceImage->isColored());
   temp->setTypeval(_sourceImage->getTypeval());
   temp->setDynamisable(_sourceImage->isDynamisable());
   delete _sourceImage;
   _sourceImage = temp;
   _currentPosition.setX(0);
   _currentPosition.setY(0);
   emit changeNumberPlane(_sourceImage->Depth());
   buildLogicImage();
   buildPhysicImage();
   QApplication::restoreOverrideCursor();
   callUpdate();
   callUpdate();
}

void ImageModel::rotateY( ) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   if (DRAWINGMODE) {
      rotateDrawingY();
      delete _lastDraw;
      _lastDraw = new Imx3duc(1, 1, _sourceImage->Height(), _sourceImage->Depth());
      cleanLastDraw();
   }
   SourceImage *temp = new SourceImage(_sourceImage->Bands(), _sourceImage->Width(), _sourceImage->Height(), _sourceImage->Depth());
   for (int b = 0; b < temp->Bands(); b++) {
      float *pt = temp->Vector(b);
      for (int z = 0; z < temp->Depth(); z++) {
    	 for (int y = 0; y < temp->Height(); y++) {
    	    for (int x = 0; x < temp->Width(); x++) {
    	       *(pt++) = (*_sourceImage)(b, _sourceImage->Depth() - (x + 1), y, z);
	    }
	 }
      }
   }
   temp->setInfo(_sourceImage->getInfo());
   temp->setMaxval(_sourceImage->getMaxval());
   temp->setMinval(_sourceImage->getMinval());
   temp->setGraph(_sourceImage->isGraph());
   temp->setRegion(_sourceImage->isRegion());
   temp->setColored(_sourceImage->isColored());
   temp->setTypeval(_sourceImage->getTypeval());
   temp->setDynamisable(_sourceImage->isDynamisable());
   delete _sourceImage;
   _sourceImage = temp;
   _currentPosition.setX(0);
   _currentPosition.setY(0);
   emit changeNumberPlane(_sourceImage->Depth());
   buildLogicImage();
   buildPhysicImage();
   QApplication::restoreOverrideCursor();
   callUpdate();
   callUpdate();
}

void ImageModel::rotateZ( ) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   if (DRAWINGMODE) {
      rotateDrawingZ();
      delete _lastDraw;
      _lastDraw = new Imx3duc(1, 1, _sourceImage->Width(), _sourceImage->Height());
      cleanLastDraw();
   }
   SourceImage *temp = new SourceImage(_sourceImage->Bands(),
				       _sourceImage->Depth(),
				       _sourceImage->Width(),
				       _sourceImage->Height());
   for (int b = 0; b < temp->Bands(); b++) {
      float *pt = temp->Vector(b);
      for (int z = 0; z < temp->Depth();z++) {
	 for (int y = 0; y < temp->Height(); y++) {
	    for (int x = 0; x < temp->Width(); x++) {
	       *(pt++) = (*_sourceImage)(b, z, x, _sourceImage->Width() - 1 - y);
	    }
	 }
      }
   }
   temp->setInfo(_sourceImage->getInfo());
   temp->setMaxval(_sourceImage->getMaxval());
   temp->setMinval(_sourceImage->getMinval());
   temp->setGraph(_sourceImage->isGraph());
   temp->setRegion(_sourceImage->isRegion());
   temp->setColored(_sourceImage->isColored());
   temp->setTypeval(_sourceImage->getTypeval());
   temp->setDynamisable(_sourceImage->isDynamisable());
   delete _sourceImage;
   _sourceImage = temp;
   _currentPosition.setX(0);
   _currentPosition.setY(0);
   emit changeNumberPlane(_sourceImage->Depth());
   buildLogicImage();
   buildPhysicImage();
   QApplication::restoreOverrideCursor();
   callUpdate();
   callUpdate();
}

void ImageModel::setRed( bool on ) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   _red = on ? 1 : 0;
   buildPhysicImage();
   QApplication::restoreOverrideCursor();
   callUpdate();
}

void ImageModel::setBlue( bool on ) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   _blue = on ? 1 : 0;
   buildPhysicImage();
   QApplication::restoreOverrideCursor();
   callUpdate();
}

void ImageModel::setGreen( bool on ) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   _green = on ? 1 : 0;
   buildPhysicImage();
   QApplication::restoreOverrideCursor();
   callUpdate();
}

void ImageModel::resetColor( ) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   _red = _green = _blue = 1;
   _brightness = 0;
   _contrast = 0;
   buildOriginalLut();
   buildPhysicImage();
   QApplication::restoreOverrideCursor();
   callUpdate();
}

void ImageModel::changeLut( int id ) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   switch (id) {
   case Frame::original:
      buildOriginalLut();
      break;
   case Frame::random:
      buildRandomLut();
      break;
   case Frame::region:
      buildRegionLut();
      break;
   case Frame::rainbow:
      buildRainbowLut();
      break;
   case Frame::negative:
      buildNegativeLut();
      break;
   case Frame::noise:
      buildNoiseLut();
      break;
   }
   buildPhysicImage();
   callUpdate();
   QApplication::restoreOverrideCursor();
}

void ImageModel::setBrightness( int val ) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   _brightness = val;
   buildPhysicImage();
   callUpdate();
   QApplication::restoreOverrideCursor();
}

void ImageModel::setContrast( int val ) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   _contrast = val;
   buildPhysicImage();
   callUpdate();
   QApplication::restoreOverrideCursor();
}

void ImageModel::keyPressEvent( QKeyEvent *e ) {
   switch (e->key()) {
   case Qt::Key_Left :
      if (_currentPosition.x() > 0) {
    	 _currentPosition.rx()--;
    	 callUpdate();
      }
      break;
   case Qt::Key_Right :
      if (_currentPosition.x() <= _logicImage->Width() - 6) {
    	 _currentPosition.rx()++;
    	 callUpdate();
      }
      break;
   case Qt::Key_Up :
      if (_currentPosition.y() > 0) {
    	 _currentPosition.ry()++;
    	 callUpdate();
      }
      break;
   case Qt::Key_Down :
      if (_currentPosition.y() <= _logicImage->Height() - 6) {
    	 _currentPosition.ry()++;
    	 callUpdate();
      }
      break;
   case Qt::Key_Plus :
      emit zoom(true);
      break;
   case Qt::Key_Minus :
      emit zoom(false);
      break;
   case Qt::Key_O :
      _frame->openImage();
      break;
   case Qt::Key_S :
      _frame->saveAsImage();
      break;
   case Qt::Key_Q :
      _frame->closeApplication();
      break;
   case Qt::Key_T:
      showThresholdingWindow();
      break;
   case Qt::Key_P:
      showPropertiesWindow();
      break;
   case Qt::Key_L:
      showColorsWindow();
      break;
   case Qt::Key_V:
      showValuesWindow();
      break;
   case Qt::Key_H:
      showHistogramWindow();
      break;
   case Qt::Key_R:
      showRowProfileWindow();
      break;
   case Qt::Key_C:
      showColumnProfileWindow();
      break;
   case Qt::Key_X:
      if (_sourceImage->Depth()>1)
    	 rotateX();
      break;
   case Qt::Key_Y:
      if (_sourceImage->Depth()>1)
    	 rotateY();
      break;
   case Qt::Key_Z:
      rotateZ();
      break;
   case Qt::Key_U:
      undoDraw();
      break;
   default :
      break;
   }
}

/**
 * Rotations
 */
void ImageModel::rotateDrawingX( ) {
   cleanLastDraw();
   Imx3duc *temp = new Img3duc( _drawImage->Height(), _drawImage->Depth(), _drawImage->Width());
   Uchar *pt = temp->Vector(0);
   for (int z = 0; z < temp->Depth();z++) {
      for (int y = 0; y < temp->Height(); y++) {
    	 for (int x = 0; x < temp->Width(); x++) {
    	    *(pt++) = (*_drawImage)(0, y, _drawImage->Height() - (z + 1), x);
	 }
      }
   }
   delete _drawImage;
   _drawImage = temp;
   int vectTemp[3];
   vectTemp[0] = _vectY[0];
   vectTemp[1] = _vectY[1];
   vectTemp[2] = _vectY[2];
   _vectY[0] = -_vectZ[0];
   _vectY[1] = -_vectZ[1];
   _vectY[2] = -_vectZ[2];
   _vectZ[0] = vectTemp[0];
   _vectZ[1] = vectTemp[1];
   _vectZ[2] = vectTemp[2];
}

void ImageModel::rotateDrawingY( ) {
   cleanLastDraw();
   Imx3duc *temp = new Img3duc( _drawImage->Width(), _drawImage->Height(), _drawImage->Depth());
   Uchar *pt = temp->Vector(0);
   for (int z = 0; z < temp->Depth();z++) {
      for (int y = 0; y < temp->Height(); y++) {
    	 for (int x = 0; x < temp->Width(); x++) {
    	    *(pt++) = (*_drawImage)(0, _drawImage->Depth() - (x + 1), y, z);
	 }
      }
   }
   delete _drawImage;
   _drawImage = temp;
   int vectTemp[3];
   vectTemp[0] = _vectZ[0];
   vectTemp[1] = _vectZ[1];
   vectTemp[2] = _vectZ[2];
   _vectZ[0] = -_vectX[0];
   _vectZ[1] = -_vectX[1];
   _vectZ[2] = -_vectX[2];
   _vectX[0] = vectTemp[0];
   _vectX[1] = vectTemp[1];
   _vectX[2] = vectTemp[2];
}

void ImageModel::rotateDrawingZ() {
   cleanLastDraw();
   Imx3duc *temp = new Img3duc( _drawImage->Depth(), _drawImage->Width(), _drawImage->Height());
   Uchar *pt = temp->Vector(0);
   for (int z = 0; z < temp->Depth(); z++) {
      for (int y = 0; y < temp->Height(); y++) {
    	 for (int x = 0; x < temp->Width(); x++) {
     	    *(pt++) = (*_drawImage)(0, z, x, _drawImage->Width() - ( y + 1));
	 }
      }
   }
   delete _drawImage;
   _drawImage = temp;
   int vectTemp[3];
   vectTemp[0] = _vectX[0];
   vectTemp[1] = _vectX[1];
   vectTemp[2] = _vectX[2];
   _vectX[0] = -_vectY[0];
   _vectX[1] = -_vectY[1];
   _vectX[2] = -_vectY[2];
   _vectY[0] = vectTemp[0];
   _vectY[1] = vectTemp[1];
   _vectY[2] = vectTemp[2];
}

void ImageModel::changeCursorVisibility() {
   callUpdate();
}

bool ImageModel::isCursorVisible() const {
   return _frame->isCursorVisible();
}

/**
 * Drawing facilities
 */
void ImageModel::drawLine( int xb, int yb, int xe, int ye, Uchar coul ) {
   int x, y, max, min;
   float pente;

   if (xb != xe) {
      pente = (float)(yb - ye) / (xb -  xe);
   } else {
      pente = MAXFLOAT;
      }
   if ((pente < -1) || (pente > 1)) {
      if (ye < yb) {
    	 min = ye;
	 max = yb;
      } else {
    	 min = yb;
	 max = ye;
      }
      for (y = max; y >= min; y--) {
    	 x = (int)((y - ye) / pente + xe);
    	 (*_drawImage)(0, _currentPlane, y, x) = coul;
      }
   } else {
      if (xe < xb) {
    	 min = xe;
	 max = xb;
      } else {
    	 min = xb;
	 max = xe;
      }
      for (x = max; x >= min; x--) {
    	 y = (int)(pente*(x - xe) + ye);
    	 (*_drawImage)(0, _currentPlane, y, x) = coul;
      }
   }
}

void ImageModel::drawLine( int xb, int yb, int xe, int ye ) {
   if (_frame->isPencilMode()) {
      drawLine(xb, yb, xe, ye, 255);
      _lastDrawingPoints.push(DrawingPoint(xe, ye));
   } else {
      drawLine(xb, yb, xe, ye, 0);
   }
}

void ImageModel::cleanLastDraw( ) {
   while (!_lastDrawingPoints.empty()) _lastDrawingPoints.pop();
}

void ImageModel::clearDraw( ) {
   Uchar *p = &((*_drawImage)(0, _currentPlane, 0, 0));
   memset(p, 0, _drawImage->Width() * _drawImage->Height());
   cleanLastDraw();
   emit draw();
}

void ImageModel::undoDraw( ) {
   if (!_lastDrawingPoints.empty()) {
      DrawingPoint ptb, pte;
      pte = _lastDrawingPoints.top();
      while (!_lastDrawingPoints.empty()) {
	 ptb = pte;
	 pte = _lastDrawingPoints.top();
	 _lastDrawingPoints.pop();
	 drawLine(ptb.x, ptb.y, pte.x, pte.y, 0);
      }
      emit draw();
   }
}

void ImageModel::displayStatus( char * message ) {
   _frame->displayStatus(message);
}

QImage ImageModel::getIcon() {
    return _physicImage.scaled(64, 64, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
}

bool ImageModel::isDynamisable() const {
   return _sourceImage->isDynamisable();
}

void ImageModel::setParent( Frame *frame ) {
   _frame = frame;
   frame->setContent(this);
}
