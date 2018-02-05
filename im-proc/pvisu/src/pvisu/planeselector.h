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
 * @file planeselector.h
 *
 * Defines a controller for changing the current plane 
 * of a 3D image.
 */

#ifndef PLANESELECTOR_H
#define PLANESELECTOR_H

#include <QToolButton>
#include "controller.h"

class ImageModel;
class TextModel;

/**
 * This class contains a slider for changing the current plane
 * of the displayed image.
 */
class PlaneSelector : public QWidget, public Controller {
   Q_OBJECT

private:

   /** The slider for changing the plane. */
   QSlider *_slider;

   /** Displays the current plane number. */
   QLCDNumber *_number;

   QToolButton *_previous;

   QToolButton *_next;

private slots:

   /**
    * Sets the keyboard focus to the slider.
    */
   void addFocus( );

public:
   
   /**
    * Creates a slider to change the current plane.
    * @param model The model that calls the controller.
    * @param parent The parent widget.
    */
   PlaneSelector( QWidget *parent );

   void setPreviousButtonEnabled( bool state ) {
      _previous->setEnabled(state);
   }

   void setNextButtonEnabled( bool state ) {
      _next->setEnabled(state);
   }

   void setModel( const ImageModel * model );

   void setModel( const TextModel * model );

public slots:

   /**
    * Changes the maximum value, called when the image is rotated.
    * @param nbPlane The new number of planes.
    */
   void updateNbPlane( long nbPlane );

protected:
   
   /**
    * Calls when the mouse is pressed.
    * Gives the keyboard focus to the slider.
    * @param e The mouse event.
    */
   void mousePressEvent( QMouseEvent *e );
};

#endif
