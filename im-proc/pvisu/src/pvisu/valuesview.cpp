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
 * @file valuesview.cpp
 *
 */

#include "valuesview.h"

ValuesView::ValuesView( const ImageModel *model ): _model(model) {
   setAttribute(Qt::WA_QuitOnClose,false);
   Qt::WindowFlags flags = this->windowFlags();
   this->setWindowFlags(flags|Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
   QFont f = font();
   f.setPointSize(10);
   setFont(f);
   
   _hFont = font().pointSize();
   if (_hFont == -1) {
      _hFont = font().pixelSize();
   } else {
      _hFont += 2;
   }
   _yshift = _hFont;
   setMinimumWidth(10 + (80 * NBCOL) + 2 * (NBCOL - 1));
   setMaximumWidth(10 + (80 * NBCOL) + 2 * (NBCOL - 1));
   setMinimumHeight(_yshift + 10 + 2 * (NBROW - 1) + (3 * _hFont + 1) * NBROW);
   setMaximumHeight(_yshift + 10 + 2 * (NBROW - 1) + (3 * _hFont + 1) * NBROW);
   
   _title = _model->windowTitle() + (" - Values");
   switch (_model->getSourceImage()->getTypeval()) {
   case SourceImage::tchar:
   case SourceImage::tlong:
      strncpy(_format, "\%11.0f\0", sizeof(_format));
      break;
   case SourceImage::tfloat:
      strncpy(_format, "\%# 11.5g\0", sizeof(_format));
      break;
   }
   
   setWindowTitle(_title);
   connect(_model, SIGNAL(askForUpdate()), this, SLOT(update()));
}

void ValuesView::update( ) {
   if (isVisible()) {
      repaint();
   }
}

void ValuesView::paint( ) {
   float zoom = _model->getZoom();
   int plane = _model->getPlane();
   QPoint position = _model->getCurrentPosition();
   const SourceImage *ims = _model->getSourceImage();

   char tmp[120];
   if (isVisible()) {
      QPainter painter(this);
      // Display coordinates
      painter.setPen(QColor(0, 0, 0));
      snprintf(tmp, sizeof(tmp), "[ x: %d", position.x(), sizeof(tmp));
      painter.drawText(30, _hFont, QString(tmp));

      snprintf(tmp, sizeof(tmp), ", y: %d", position.y(), sizeof(tmp));
      painter.drawText(100, _hFont, QString(tmp));

      snprintf(tmp, sizeof(tmp), ", z: %d ]", plane, sizeof(tmp));
      painter.drawText(170, _hFont, QString(tmp));

      for (int i = 0; i < NBCOL; i++) {
	 for (int j = 0; j < NBROW; j++) {
	    int vx;
	    int vy;
	    int px;
	    int py;
	    if (zoom  <1.0f) {
	       vx = (int)(position.x() + (i / zoom));
	       vy = (int)(position.y() + (j / zoom));
	       px = position.x() * zoom + i;
	       py = position.y() * zoom + j;
	    } else {
	       vx = position.x() + i;
	       vy = position.y() + j;
	       px = (position.x() + i) * zoom;
	       py = (position.y() + j) * zoom;
	    }
	    if (px >= _model->getPhysicImage().width()) {
	       px = _model->getPhysicImage().width() - 1;
	    }
	    if (py >= _model->getPhysicImage().height()) {
	       py = _model->getPhysicImage().height() - 1;
	    }
 	    // If the pixels are out of bounds.
 	    if (vx >= ims->Width() || vy >= ims->Height()) {
	       painter.fillRect(5  + (82 * i),
				_yshift + 5 + ((3 * _hFont + 3) * j),
				80,
				(3 * _hFont + 1), Qt::white);
	       for (int b = 0; b < ims->Bands(); b++) {
		  painter.setPen(QColor(0, 0, 0));
		  painter.drawText(5 + (82 * i),
				   _yshift + 5 + 2 * _hFont + ((3 * _hFont + 3) * j),
				   QString("-"));
	       }
	    } else {
  	       QRgb color = _model->getPhysicImage().pixel(px, py);
 	       painter.fillRect(5 + (82 * i),
				_yshift + 5 + ((3 * _hFont + 3) * j),
				80,
				(3 * _hFont + 1),
				QColor(color));
	       char tm[20];
	       for (int b = 0; b < ims->Bands(); b++) {
		  snprintf(tmp, sizeof(tmp), _format, (*ims)[b][plane][vy][vx], 15);

		  if (qGray(color) < 127) {
		     painter.setPen(QColor(255, 255, 255));
		  } else {
		     painter.setPen(QColor(0, 0, 0));
		  }
		  if (!ims->isColored()) {
		     painter.drawText(5 + (82 * i), _yshift + 4 + 2 * _hFont + ((3 * _hFont + 3) * j), QString(tmp));
		  } else {
		     painter.drawText(5 + (82 * i),
				      _yshift + 4 + (b + 1) * _hFont + ((3 * _hFont + 3) * j),
				      QString(tmp));
		  }
	       }
	    }
	 }
      }
   }
}

void ValuesView::paintEvent( QPaintEvent *e ) {
   e->accept();
   paint();
}
