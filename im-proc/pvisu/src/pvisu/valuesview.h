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
 * @file valuesview.h
 *
 * Creates a window that displays the pixel values
 * of the current selected area. 
 */

#ifndef VALUES_H
#define VALUES_H

#include "imagemodel.h"
#include "viewer.h"

/**
 * This class displays the value and the related color of the pixels within 
 * the current selected area. This area is AxA pixels rectangle and its upper
 * left corner is located from the mouse pointer coordinates.
 */
class ValuesView : public QMainWindow, public Viewer {
   Q_OBJECT

private :
 
   /** The model that contains the image data. */
   const ImageModel *_model;

   /** The title of the window without the position of the cursor. */
   QString _title;

   /** The height in pixel of the font. */
   int _hFont;

   /** The vertical shift between the top of the window and the values rectangle. */
   int _yshift;

   /** The number of rows of the selected area. */
   static const int NBROW = 6;

   /** The number of column of the selected area. */
   static const int NBCOL = 6;
   
   /** The number format for displaying a pixel value. */
   char _format[16];

   /**
    * Paints the widget.
    */
   void paint();

protected:
   
   /**
    * Called when something must be drawn.
    * Repaints the colors cases and the values.
    * @param e The paint event.
    */
   void paintEvent( QPaintEvent *e );
   
public:
   
   /**
    * Creates a window where the pixels values are displayed.
    * @param model The model that contains the data.
    */
   ValuesView( const ImageModel *model );

private slots:

   /**
    * Updates the widget content from the new pixel values.
    */
   void update( );
};

#endif
