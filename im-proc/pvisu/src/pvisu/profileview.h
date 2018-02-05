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

/*
 * @file profileview.h
 *
 * Creates a window for displaying an histogram,
 * where the values of pixel of the column or the row selected are represented.
 */

#ifndef PROFIL_H
#define PROFIL_H

#include "imagemodel.h"
#include "viewer.h"

/**
 * This class is a window with histogram that displays the values of pixel.
 */
class ProfileView : public QMainWindow, public Viewer {
   Q_OBJECT

private:
   
   /** The model that calls the view. */
   const ImageModel *_model;

   /** The flag for profile for row or column. */
   bool _row;

   /** The height of one histogram. */
   int _h;

   /** The height in pixel of the font. */
   int _hFont;

   /** The title of the window without the positon of the row or the column. */
   QString _title;
   
   /**
    * Paints the widget.
    */
   void paint( );

public:
   
   /**
    * Creates the window for displaying histograms.
    * @param model The model that calls the Viewer.
    * @param row true : profil on row, false : profil on column.
    */
   ProfileView( const ImageModel *model, bool row );

public slots:

   /**
    * Update the histogram from threshold, column and row selected.
    */
   void update();

protected:

   /**
    * Called when the window must be repaint.
    * @param e The paint event.
    */
   void paintEvent( QPaintEvent *e );

};

#endif
