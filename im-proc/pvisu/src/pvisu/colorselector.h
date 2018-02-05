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
 * @file colorselector.h
 *
 * Defines a controller for modifying colors.
 */

#ifndef COLORSELECTOR_H
#define COLORSELECTOR_H

#include "frame.h"
#include "imagemodel.h"
#include "controller.h"

/**
 * This class is a Controller for changing the colors of the displayed 
 * image like brightness, contrast, lut...
 */
class ColorSelector : public QMainWindow, public Controller {

   Q_OBJECT

private :

   /** The buttons for displaying only the red band. */
   QCheckBox *_red;

   /** The buttons for displaying only the green band. */
   QCheckBox *_green;

   /** The button for displaying only the blue band. */
   QCheckBox *_blue;

   /** The label that contains the word brightness. */
   QLabel *_brightTitle;

   /** The slider for selecting the brightness. */
   QSlider *_brightSlider;

   /** The value of the brightness. */
   QLCDNumber *_brightNumber;

   /** The label that contains the word contrast. */
   QLabel *_contrastTitle;
   
   /** The slider for selecting the contrast. */
   QSlider *_contrastSlider;

   /** The value of the contrast. */
   QLCDNumber *_contrastNumber;

   /** The button for resetting all modifications. */
   QPushButton *_reset;
   
   /** The group of buttons that contains the different choise of lut. */
   QButtonGroup *_luts;

   /** The button for selecting the orignal lut. */
   QAbstractButton *_original;

   /** The button for selecting the negative lut. */
   QAbstractButton *_negative;

   /** The button for selecting the random lut. */
   QAbstractButton *_random;

   /** The button for selecting the region lut. */
   QAbstractButton *_region;

   /** The button for selecting the rainbow lut. */
   QAbstractButton *_rainbow;

   /** The button for selecting the noise lut. */
   QAbstractButton *_noise;
      
private slots :

   /**
    * Returns to the initial settings.
    */
   void resetSettings( );

   /**
    * Select the lut from the button clicked.
    * @param button The clicked button,
    */
   void selectLut( QAbstractButton *button );

public:
   
   /**
    * Creates a window for changing colors of the displayed image.
    * @param model The model that contains the data.
    */
   ColorSelector( const ImageModel *model );

   /**
    * Deletes this window.
    */
   ~ColorSelector(){
      delete _reset;
      delete _contrastNumber;
      delete _contrastSlider;
      delete _brightNumber;
      delete _brightSlider;
      delete _blue;
      delete _green;
      delete _red;
      delete _brightTitle;
      delete _contrastTitle;
      delete _luts;
      delete _original;
      delete _negative;
      delete _region;
      delete _random;
      delete _rainbow;
      delete _noise;
   }

signals:
   
   /**
    * Emitted when the button reset is pressed.
    */
   void reset( );

   /**
    * Emitted when a new lut is selected.
    * @param id The index of the lut.
    */
   void newLut( int id );
};

#endif
