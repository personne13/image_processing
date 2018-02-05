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
 * @file profileview.cpp
 *
 * Creates a window for displaying an image profile,
 * where the values of pixel of the column or the row  selected are represented
 * as a bar.
 */

#include "profileview.h"

ProfileView::ProfileView( const ImageModel *model, bool row ): _model(model), _row(row) {
   const SourceImage *ims = _model->getSourceImage();
   bool colored = ims->isColored();

   setAttribute(Qt::WA_QuitOnClose, false);
   Qt::WindowFlags flags = this->windowFlags();
   this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);

   QFont f = font();
   f.setPointSize(10);
   setFont(f);

   _hFont = font().pointSize();
   if (_hFont == -1) {
      _hFont = font().pixelSize();
   } else {
      _hFont += 2;
   }

   if (_row) {
      setMinimumWidth(ims->Width()+10);
      if (colored) {
	 _h = 64;
	 setMinimumHeight(_h * 3 + 20 + _hFont);
      } else {
	 _h = 128;
	 setMinimumHeight(_h + 10);
      }
   } else {
      setMinimumHeight(ims->Height() + 10 + _hFont);
      if (colored) {
	 _h = 64;
	 setMinimumWidth(_h * 3 + 20);
      } else {
	 _h = 128;
	 setMinimumWidth(_h + 10);
      }
   }
   
   _title = _model->windowTitle()+" - Profile";
   
   setWindowTitle(QString(_title));
   connect(_model, SIGNAL(askForUpdate()), this, SLOT(update()));
}

void ProfileView::update( ) {
   if (isVisible()) {
      repaint();
   }
}

void ProfileView::paint( ) {
   QPainter painter(this);
   painter.eraseRect(0, 0, width(), height());
   const SourceImage* ims = _model->getSourceImage();
   int plane = _model->getPlane();
   QPoint position = _model->getCurrentPosition();
   long posX = position.x();
   long posY = position.y();
   float max = ims->getMaxval();
   float min = ims->getMinval();
   float step;
   if (max == min) {
      step = _h;
   } else {
      step = _h / (max - min);
   }
   
   char tmp1[128];
   if (_row) {
      snprintf(tmp1, sizeof(tmp1), "row: %ld", posY);
   } else {
      snprintf(tmp1, sizeof(tmp1), "column: %ld", posX);
   }
   painter.setPen(QColor(0, 0, 0));
   painter.drawText(30, _hFont, QString(tmp1));

   for (int b = 0; b < ims->Bands(); b++) {
      if (ims->isColored()) {
	 if (b == 0) {
	    painter.setPen(QColor(255, 0, 0));
	 } else if (b == 1) {
	    painter.setPen(QColor(0, 255, 0));
	 } else {
	    painter.setPen(QColor(0, 0, 255));
	 }
      } else {
	 painter.setPen(QColor(0, 0, 0));
      }
      if (_row) {
	 for (int x = 0; x < ims->Width(); x++) {
	    float niv = (*ims)[b][plane][posY][x];
	    painter.drawLine(5 + x,
			     _hFont + (b + 1) * (_h + 5),
			     5 + x,
			     _hFont + (int)((float)((b + 1) * (_h + 5)) - (niv - min) * step));
	 }
      } else {
	 for (int y = 0; y < ims->Height(); y++) {
	    float niv = (*ims)[b][plane][y][posX];
	    painter.drawLine(5 + (_h + 5) * b,
			     _hFont + 5 + y,
			     (int)(5 + (_h + 5) * b + (niv - min) * step),
			     _hFont + 5 + y);
	 }
      }
   }
} 

void ProfileView::paintEvent( QPaintEvent *e ) {
   e->accept();
   paint();
}
