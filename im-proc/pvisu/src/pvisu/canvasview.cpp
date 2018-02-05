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
 * @file canvasview.cpp
 *
 * Defines the class that paints the physic image.
 */

#include "canvasview.h"

/**
 * Tries to open a window from the the size of the input image.
 */
CanvasView::CanvasView( ImageModel *model, QWidget *parent ): QWidget(parent), _model(model), _currentPosX(0), _currentPosY(0) {
   _pixmap = new QPixmap(QPixmap::fromImage(_model->getPhysicImage(), Qt::ColorOnly));
   this->resize((int)(_pixmap->width()), (int)(_pixmap->height()));
   connect(_model, SIGNAL(askForUpdate()), this, SLOT(update()));
   connect(_model, SIGNAL(draw()), this, SLOT(repaint()));
   setAcceptDrops(false);
}

void CanvasView::update() {
   QPoint position = _model->getCurrentPosition();
   if (_currentPosX != position.x() || _currentPosY != position.y()) {
      // Just redraw the cursor.
      _currentPosX = position.x();
      _currentPosY = position.y();
   } else {
      delete _pixmap;
      // Build a new pixmap from new visualisation parameters.
      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
      _pixmap = new QPixmap(QPixmap::fromImage(_model->getPhysicImage(), Qt::ColorOnly));
      this->resize ((int)(_pixmap->width()), (int)(_pixmap->height()));
      QApplication::restoreOverrideCursor();
   }
   if (isVisible()) {
      repaint();
   }
}

void CanvasView::paint( QPaintEvent *e ) {
   QPainter painter(this);
   painter.drawPixmap(e->rect().topLeft(), *_pixmap, e->rect());
   painter.setPen(QColor(255, 0, 0));
   float zoom = _model->getZoom();
   // Draw the cursor.
   if (_model->isCursorVisible()) {
      float magnify;
      if (zoom < 1) {
	 magnify = 1;
      } else {
	 magnify = zoom;
      }
      painter.drawLine((int)(_currentPosX * zoom),
		       0,
		       (int)(_currentPosX * zoom),
		       _pixmap->height());
      painter.drawLine(0,
		       (int)(_currentPosY * zoom),
		       _pixmap->width(),
		       (int)(_currentPosY * zoom));
      painter.drawRect((int)(_currentPosX * zoom),
 		       (int)(_currentPosY * zoom),
 		       (int)(6 * magnify),
 		       (int)(6 * magnify));
   }
   // TODO draw rubband band.
   if (_model->getRubberBand().width() > 0) {
      QRect rubber = _model->getRubberBand();
      painter.drawRect((int)(rubber.x() * zoom),
 		       (int)(rubber.y() * zoom),
 		       (int)(rubber.width() * zoom),
 		       (int)(rubber.height() * zoom));
   }

   // Draw the current drawing.
   if (DRAWINGMODE) {
      Imx3duc *imd = _model->getDrawImage();
      long plane = _model->getPlane();
      int h = (int)((e->rect().top() + e->rect().height()) / zoom);
      if (h > imd->Height()) {
	 h = imd->Height();
      }
      int w = (int)((e->rect().left() + e->rect().width()) / zoom);
      if (w > imd->Width()) {
	 w = imd->Width();
      }

      for (int y = (int)(e->rect().top() / zoom); y < h; y++) {
	 for (int x = (int)(e->rect().left()  /zoom); x < w; x++) {
	    if ((*imd)(0, plane, y, x) != 0) {
	       for (int i = 0; i < zoom; i++) {
		  for (int j = 0; j < zoom; j++) {
 		     painter.drawPoint((int)(x * zoom + i), (int)(y * zoom + j));
		  }
	       }
	    }
	 }
      }
   }
}

void CanvasView::paintEvent( QPaintEvent *e ) {
   paint(e);
}

static char message[256];

void CanvasView::mousePressEvent( QMouseEvent *e ) {
   setFocus();
   float z = _model->getZoom();
   int x = (int)(e->x() /z);
   int y = (int)(e->y() / z);
   if (DRAWINGMODE) {
      _lastDrawingX = x;
      _lastDrawingY = y;
      _model->cleanLastDraw(); // For undo!
      _model->drawLine(x, y, x, y);
      if (z < 1.0F) {
	 z = 1.0F;
      }
      repaint((int)(x * z), (int)(y * z), (int)z, (int)z);
   } else {
      if (x != _currentPosX || y != _currentPosY) {
	 _model->setPosition(x, y);
	 _model->startRubberBand(x, y);
      }
   }
   snprintf(message, sizeof(message), " (%d, %d)", x, y);
   _model->displayStatus(message);
}

void CanvasView::mouseMoveEvent( QMouseEvent *e ) {
   float z = _model->getZoom();
   int x = (int)(e->x() / z);
   int y = (int)(e->y() / z);
   if (DRAWINGMODE) {
      Imx3duc *imd = _model->getDrawImage();
      if (x < 0 || y < 0 || x >= imd->Width()|| y >= imd->Height()) {
	 return;
      }
      _model->drawLine(_lastDrawingX, _lastDrawingY, x, y);

      // redraw only the line.
      if (z < 1.0F) {
	 z = 1.0F;
      }
      int xr, yr, wr, hr;
      if (_lastDrawingX >= x) {
	 xr = (int)(x*z);
	 wr = (int)((_lastDrawingX - x + 1) * z);
      } else {
	 xr = (int)(_lastDrawingX*  z);
	 wr = (int)((x - _lastDrawingX + 1) * z);
      }
      if (_lastDrawingY >= y) {
	 yr = (int)(y * z);
	 hr = (int)((_lastDrawingY - y + 1) * z);
      } else {
	 yr = (int)(_lastDrawingY * z);
	 hr = (int)((y - _lastDrawingY + 1) * z);
      }
      repaint(xr, yr, wr, hr);
      _lastDrawingX = x;
      _lastDrawingY = y;
   } else {
      _model->endRubberBand(x, y);
      repaint();
   }
   snprintf(message, sizeof(message), " (%d, %d) -> (%d %d) : (%d, %d)",
	   _model->getCurrentPosition().x(),
	   _model->getCurrentPosition().y(), x, y,
	   abs(_model->getCurrentPosition().x() - x),
	   abs( _model->getCurrentPosition().y() - y));
   _model->displayStatus(message);
}

void CanvasView::mouseReleaseEvent( QMouseEvent *e ) {
   float z = _model->getZoom();
   long x = (int)(e->x() / z);
   long y = (int)(e->y() / z);
   if (x != _currentPosX || y != _currentPosY) {
      _model->setPosition(x, y);
      _model->endRubberBand(x, y);
   }
   repaint();
}

void CanvasView::keyPressEvent( QKeyEvent *e ) {
   switch (e->key()) {
   case Qt::Key_Left :
      _model->setPosition(_currentPosX - 1, _currentPosY);
      snprintf(message, sizeof(message), " (%d, %d)", _currentPosX - 1, _currentPosY);
      _model->displayStatus(message);
      break;
   case Qt::Key_Right :
      _model->setPosition(_currentPosX + 1, _currentPosY);
      snprintf(message, sizeof(message), " (%d, %d)", _currentPosX + 1, _currentPosY);
      _model->displayStatus(message);
      break;
   case Qt::Key_Up :
      _model->setPosition(_currentPosX, _currentPosY - 1);
      snprintf(message, sizeof(message), " (%d, %d)", _currentPosX, _currentPosY - 1);
      _model->displayStatus(message);
      break;
   case Qt::Key_Down :
      _model->setPosition(_currentPosX, _currentPosY + 1);
      snprintf(message, sizeof(message), " (%d, %d)", _currentPosX, _currentPosY + 1);
      _model->displayStatus(message);
      break;
   default:
      e->ignore();
   }
}

void CanvasView::wheelEvent(QWheelEvent *event)
{
   if (event->modifiers().testFlag(Qt::ControlModifier)) {
      if (event->delta() < 0)
	 emit zoom(false);
      else
	 emit zoom(true);
      event->accept();
   } else {
      event->ignore();
   }
}
