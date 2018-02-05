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
 * @file histogramview.h
 *
 * Creates a window for displaying an histogram,where
 * the values of pixels are represented according to their occurences. 
 */

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "imagemodel.h"
#include "viewer.h"

/**
 * This class displayed a histogram, where the values of pixels are represented
 * according to their occurences.
 */
class HistogramView : public QMainWindow, public Viewer {
   Q_OBJECT

private:
   
   /** The model that contains the data. */
   const ImageModel *_model;

   /** The width of the histogram. */
   const static int HISTWIDTH = 256;
   
   /** The height of the histogram. */
   const static int HISTHEIGHT = 100;

   /** The table that contains the values of the histogram. */
   int _histDraw[3][HISTWIDTH];
   
   /** The begin of histogram. */
   float _hbeg;

   /** The end of histogram. */
   float _hend;

   /** The max value. */
   int _hmax[3];

   /** The index of the max value. */
   int _imax[3];

   /** The step between two bars of the histogram. */
   float _step;

   /** The format of the displayed numbers. */
   char _format[16];

   /** The low threshold. */
   float _thresholdL;

   /** The high threshold. */
   float _thresholdH;
   
   /** The current plane displayed. */
   long _currentPlane;

   /** The status bar that displays the values of one bar of the histogram . */
   QStatusBar *_status;

   /** 
    * Paints histogram.
    */
   void paint( );

private slots:

   /**
    * Calls for updating the view.
    * Draws the histogram from thresholding and plane.
    */
   void update( );

protected:
   
   /**
    * Calls when the window has to be repaint.
    * @param e The paint event.
    */
   void paintEvent( QPaintEvent *e );

   /**
    * Calls when the mouse buttons are pressed.
    * @param e The mouse event.
    */
   void mousePressEvent( QMouseEvent *e );
   
   /**
    * Calls when the mouse buttons are release.
    * @param e The mouse event.
    */
   void mouseReleaseEvent( QMouseEvent *e );

   /**
    * Calls when the mouse buttons are clicked and moved.
    * @param e The mouse event.
    */
   void mouseMoveEvent( QMouseEvent *e );

public:
   
   /**
    * Builds the window that contains the histogram.
    * @param model The model that contains the data.
    */
   HistogramView( const ImageModel *model );

   /**
    * Destroys the widget.
    */
   ~HistogramView( ){
      delete _status;
   }
};

#endif
