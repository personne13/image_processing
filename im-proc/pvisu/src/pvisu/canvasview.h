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
 * @file canvasview.h
 *
 * Defines the class that displays the physic image.
 */

#ifndef CANVAS_H
#define CANVAS_H

#include "viewer.h"
#include "imagemodel.h"

/**
 * The CanvasView class provides a 2D area
 * that displays the physic image with the current vizualisation parameters.
 */
class CanvasView: public QWidget, public Viewer {
   Q_OBJECT

private:

   /** The model that contains the image data. */
   ImageModel *_model;

   /** The Qt Pixmap which is displayed. */
   QPixmap *_pixmap;

   /** The current X position of the cursor. */
   int _currentPosX;

   /** The current Y position of cursor. */
   int _currentPosY;

   /** The x coordinate of the last drawing point. */
   int _lastDrawingX;

   /** The y coordinate of the last drawing point. */
   int _lastDrawingY;

   /**
    * Repaints the pixmap in the rectangle given by the PaintEvent.
    * @param e The paint event that contains the rectangle.
    */
   void paint( QPaintEvent *e = 0 );

protected:

   /**
    * Repaints the current content of the widget.
    * @param paintEvent	The event handler.
    */
   void paintEvent( QPaintEvent* paintEvent );

   /**
    * Called when a mouse button is released.
    * Sets the cursor coordinate to the mouse pointer location.
    * @param e The mouse event.
    */
   void mouseReleaseEvent( QMouseEvent *e );

   /**
    * Called when a mouse button is pressed and the mouse moved.
    *
    * @param e The mouse event.
    */
   void mouseMoveEvent( QMouseEvent *e );

   /**
    * Called when a mouse button is pressed.
    * Sets the cursor coordinates to the mouse pointer location.
    * @param e The mouse event.
    */
   void mousePressEvent( QMouseEvent *e );

   /**
    * Called when a key is pressed.
    * Translates the cursor according to key value with a step of one pixel.
    * @param e The key event.
    */
   void keyPressEvent( QKeyEvent *e );

   /**
    * Called when the mouse wheel is scrolled.
    * When Ctrl key is down, a zoom signal is emitted and the event is accepted.
    * Thus, the event is no longer sent to the scolling area.
    */
   void wheelEvent(QWheelEvent * event);

signals:

   /**
    * Emitted when the mouse wheel is actioned while Control key is down.
    */
    void zoom(bool);

public:

   /**
    * Creates the window that supports the image.
    * @param model The model contains data.
    * @param parent The parent widget.
    */
   CanvasView( ImageModel *model, QWidget *parent );

   /**
    * Deletes this view.
    */
   ~CanvasView( ) {
      delete _pixmap;
   }

private slots:

   /**
    * Updates the image from new vizualisation parameters.
    */
   void update( );
};

#endif
