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
 * @file thresholdselector.h
 *
 * Defines a class for thresholding the displayed gray image.
 */

#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "imagemodel.h"
#include "controller.h"

#define MAXNSTEP 500	// Max number of step in the slider

/**
 * This class is a Controller that contains two sliders for
 * thresholding gray image, one for low thresholding and one for high thresholding.
 */
class ThresholdSelector: public QMainWindow, public Controller {
   Q_OBJECT
   
private :
   
   /** The low threshold boundary. */
   float _thresholdL[3];
   
   /** The high threshold boundary. */
   float _thresholdH[3];
   
   /** The normalization value for float values. */
   int _normseuil;
   
   /** The slider for selecting the high thresholding. */
   QSlider *_sliderH[3];
   
   /** The Slider for selecting the low thresholding. */
   QSlider *_sliderL[3];
   
   /** The number displayed for the low thresholding. */
   QLabel *_numberL[3];
   
   /** The number displayed for the high thresholding. */
   QLabel *_numberH[3];
   
   /** The format of the displayed numbers. */
   char _format[16];
   
   /** The step for big float. */
   float _step;
   
   /** The minimum pixel value of the image. */
   float _min;
	     
private slots:
   
   void thresholdL0( int threshold );
   void thresholdL1( int threshold );
   void thresholdL2( int threshold );
   void thresholdH0( int threshold );
   void thresholdH1( int threshold );
   void thresholdH2( int threshold );

   /**
    * Thresholds the displayed image from the low threshold.
    * @param band an image band.
    * @param threshold The value of the threshold.
    */
   void thresholdL( int band, int threshold );
   
   /**
    * Thresholds the displayed image from the high threshold.
    * @param band an image band.
    * @param threshold The value of the threshold.
    */
   void thresholdH( int band, int threshold );
   
public:
   
   /**
    * Creates a window for selecting the thresholding
    * with two sliders.
    * @param model The model that contains the image data.
    */
   ThresholdSelector( const ImageModel *model );
   
   /**
    * Deletes this controller.
    */
   ~ThresholdSelector() {
      for (int b = 0; b < 3; b++) {
	 if (_sliderH[b] != 0) {
	    delete _sliderH[b];
	 }
	 if (_sliderL[b] != 0) {
	    delete _sliderL[b];
	 }
	 if (_numberL[b] != 0) {
	    delete _numberL[b];
	 }
	 if (_numberH[b] != 0) {
	    delete _numberH[b];
	 }
      }
   }
   
signals:
   
   /** Emited when changing the low thresholding. */
   void lowThreshold( int, float );
   
   /** Emited when changing the hight thresholding. */
   void highThreshold( int, float );

};

#endif
