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
 * @file histogramview.cpp
 *
 * Creates a window for displaying an histogram, where
 * the values of pixels are represented according to their occurences. 
 */

#include "histogramview.h"

/**
 * Need initialization of _currentPlane to -1.
 */
HistogramView::HistogramView( const ImageModel* model ): _model(model), _thresholdL(0), _thresholdH(0), _currentPlane(-1) {
   Qt::WindowFlags flags = this->windowFlags();
   this->setWindowFlags(flags|Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
   QFont f = font();
   f.setPointSize(10);
   setFont(f);

   setAttribute(Qt::WA_QuitOnClose, false);
   setMinimumWidth(80 + HISTWIDTH + 40);
   setMinimumHeight(10 + (15 + HISTHEIGHT + 40) * _model->getSourceImage()->Bands());
   _status = statusBar();

   // Title
   QString t = _model->windowTitle();
   t.append(" - Histogram");
   setWindowTitle(t);
   
   if (_model->getSourceImage()->getTypeval() == SourceImage::tfloat) {
      strncpy(_format, "\%.3g", sizeof(_format));
   } else {
      strncpy(_format, "\%.0f", sizeof(_format));
   }
   connect(_model, SIGNAL(askForUpdate()), this, SLOT(update()));
   // Force update() to build the first histogram (need that currentPlane = -1).
   update();
}

/**
 * Modifies the current histogram from a new plane or new threshold bounds.
 */
void HistogramView::update( ) {
   if (_thresholdL != _model->getThresholdL(0) || _thresholdH != _model->getThresholdH(0) || _currentPlane != _model->getPlane()) {
      const SourceImage *ims = _model->getSourceImage();
      const float min = ims->getMinval();
      const float max = ims->getMaxval();

      _thresholdL = _model->getThresholdL(0);
      _thresholdH = _model->getThresholdH(0);
      _currentPlane = _model->getPlane();
      
      if (_thresholdH > max) {
	 _thresholdH = max; // Possible? TO BE DELETE
      }

#ifdef DEBUG
      fprintf(stderr, "Threshold low = %f Threshold high = %f\n", _thresholdL, _thresholdH);
#endif

      if (ims->getTypeval() == SourceImage::tchar) {
	 _hbeg = 0;
 	 _hend = HISTWIDTH - 1;
	 _step = 1.0F;
      } else if (ims->getTypeval() == SourceImage::tfloat) {
	 _hbeg = _thresholdL;
	 _hend = _thresholdH;
	 _step = (_hend - _hbeg + 1) / HISTWIDTH;
      } else { // slong
	 // Calculate "bin" width.
	 if (min >= 0 && max  < (float)HISTWIDTH) { // Best case = Uchar case!
	    _hbeg = 0;
	    _hend = HISTWIDTH - 1;
	    _step = 1.0F;
	 } else if (_thresholdL >= 0 && _thresholdH < (float)HISTWIDTH) {
	    _hbeg = 0;
	    _hend = HISTWIDTH - 1;
	    _step = 1.0F;
	 } else if (_thresholdH - _thresholdL <= (float)HISTWIDTH) {
	    _hbeg = _thresholdL;
	    _hend = _thresholdL + HISTWIDTH;
	    _step = 1.0F;
	 } else { // worse case.
	    _hbeg = _thresholdL;
	    _hend = _thresholdH;
	    _step = (_hend-_hbeg+1)/HISTWIDTH;
	 }
      }
      
#ifdef DEBUG
      printf("hbeg = %f hend = %f step(bin width) = %f\n", _hbeg, _hend, _step);
#endif
      
      for (int b = 0; b < ims->Bands(); b++) {
	 memset(_histDraw[b], 0, HISTWIDTH*sizeof(int));
	 //compute histogram
	 const Float *ps = &(*ims)[b][_currentPlane][0][0];
	 for (int i = 0; i < ims->Height()*ims->Width(); i++, ps++) {
	    if (*ps >= _thresholdL && *ps <= _thresholdH) {
	       int x = (int)((*ps - _hbeg)/_step);
	       _histDraw[b][x]++;
	    }
	 }

	 _hmax[b] = _histDraw[b][0];
	 _imax[b] = 0;
	 for (int w = 1; w < HISTWIDTH; w++) {
	    if (_histDraw[b][w] > _hmax[b]) {
	       _hmax[b] = _histDraw[b][w];
	       _imax[b] = w;
	    }
#ifdef DEBUG
	    printf("histo[%d] = %d\n", w, _histDraw[b][w]);
#endif
	 }
	 if (isVisible()) {
	    repaint();
	 }
      }
   }
}

/**
 * Needs computation of _histDraw.
 */
void HistogramView::paint( ) {
   QPainter painter(this);
   painter.eraseRect(0, 0, width(), height());

   for (int b = 0; b < _model->getSourceImage()->Bands(); b++) {
      
      const int X0 = 80;
      const int Y0 = 115 + (15 + HISTHEIGHT + 40) * b;
      
      // HISTOGRAM(S)
      if (_hmax[b] != 0) {
	 for (int i = 0; i < HISTWIDTH; i++) {
	    if (_model->getSourceImage()->Bands() == 1) { // Gray levels.
	       painter.setPen(QColor(0, 0, 255));
	    } else { // Colors
	       if (b == 0) {
		  painter.setPen(QColor(255, 0, 0));
	       } else if (b == 1) {
		  painter.setPen(QColor(0, 255, 0));
	       } else {
		  painter.setPen(QColor(0, 0, 255));
	       }
	    }
	    int val = (int)((float)(_histDraw[b][i] * 100.0) / (float)_hmax[b]);
	    painter.drawLine(X0 + i, Y0, X0 + i, Y0 - val);
	 }
      }
      // AXES:  keep them here to overload the border line of the histogram...
      painter.setPen(QColor(0, 0, 0));

      char tmp[20];
      // 1) y-axis
      painter.drawLine(X0 - 1, Y0 - HISTHEIGHT - 5, X0 - 1, Y0 + 4);
      // min legend
      painter.drawText(0, Y0 - 8, 65, Y0 - 8, Qt::AlignRight, QString("0"));

      // 2) x-axis
      painter.drawLine(X0 - 4, Y0, X0 + HISTWIDTH + 4, Y0);
      // xtics (min and max)
      painter.drawLine(HISTWIDTH+80, Y0 + 1, HISTWIDTH + 80, Y0 + 5);
      snprintf(tmp, sizeof(tmp), _format, _hbeg);
      tmp[sizeof(tmp) - 1] = 0;
      painter.drawText(0, Y0, X0, Y0 + 10, Qt::AlignRight, QString(tmp));
      snprintf(tmp, sizeof(tmp), _format, _hend);
      tmp[sizeof(tmp) - 1] = 0;
      painter.drawText(0, Y0, X0 + HISTWIDTH  +16, Y0 + 10, Qt::AlignRight, QString(tmp));

      // MAX VALUES
      painter.setPen(QColor(255, 0, 0));
      // max y-axis
      painter.drawLine(X0  - 6,
		       Y0 - HISTHEIGHT,
		       X0 - 2,
		       Y0 - HISTHEIGHT);
      snprintf(tmp, sizeof(tmp), "%d", _hmax[b]);
      tmp[sizeof(tmp) - 1] = 0;
      painter.drawText(0,
		       4 + Y0 - 115,
		       X0 - 8,
		       25 + Y0 - 115,
		       Qt::AlignRight, QString(tmp));
      // max x-axis
      painter.drawLine(X0  +_imax[b], Y0 + 1, X0 + _imax[b], Y0 + 6);
      snprintf(tmp, sizeof(tmp), _format, _imax[b] * _step + _hbeg);
      tmp[sizeof(tmp) - 1] = 0;
      painter.drawText(0,
		       Y0 + 11,
		       X0 + 20 + _imax[b],
		       Y0 + 30,
		       Qt::AlignRight, QString(tmp));
   }
}

/**
 *
 */
void HistogramView::paintEvent( QPaintEvent *e ) {
   e->accept();
   paint();
}

/**
 * 
 */
void HistogramView::mousePressEvent( QMouseEvent *e ) {
   const int X0 = 80;

   int x = e->x()-X0;
   int y = e->y();
   
   if (x < 0) {
      x = 0;
   }
   if (x >= HISTWIDTH) {
      x = HISTWIDTH - 1;
   }
   
   char tmp1[120];
   char tmp2[64];
   
   float hx1 = x * _step + _hbeg;
   if (_step == 1.0F) {
      snprintf(tmp2, sizeof(tmp2), "histogram[%s] = %%d", _format);
      tmp2[sizeof(tmp2) - 1] = 0;

      if (y <= 115 && y >= 15 ) {
	 snprintf(tmp1, sizeof(tmp1), tmp2, hx1, _histDraw[0][x]);
      } else if (y >= 170 && y <= 270) {
	 snprintf(tmp1, sizeof(tmp1), tmp2, hx1, _histDraw[1][x]);
      } else if (y >= 325 && y <= 425) {
	 snprintf(tmp1, sizeof(tmp1), tmp2, hx1, _histDraw[2][x]);
      } else {
	 strncpy(tmp1, "\0", sizeof(tmp1));
      }
      tmp1[sizeof(tmp1) - 1] = 0;
   } else {
      float hx2 = hx1 + _step;
      snprintf(tmp2, sizeof(tmp2), "histogram[%s, %s] = %%d", _format, _format);
      if (y <= 115 && y >= 15) {
	 snprintf(tmp1, sizeof(tmp1), tmp2, hx1, hx2, _histDraw[0][x]);
      } else if (y >= 170 && y <= 270) {
	 snprintf(tmp1, sizeof(tmp1), tmp2, hx1, hx2, _histDraw[1][x]);
      } else if (y >= 325 && y <= 425) {
	 snprintf(tmp1, sizeof(tmp1), tmp2, hx1, hx2, _histDraw[2][x]);
      } else {
	 strncpy(tmp1, "\0", sizeof(tmp1));
      }
      tmp1[sizeof(tmp1) - 1] = 0;
   }
   _status->showMessage(tr(tmp1));
}

void HistogramView::mouseMoveEvent( QMouseEvent *e ) {
   mousePressEvent(e);
}

void HistogramView::mouseReleaseEvent( QMouseEvent *e ) {
   e->accept();
   _status->clearMessage();
}
