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
 * @file dynamicselector.h
 *
 * Defines a menu for changing the image dynamic. This corresponds 
 * to different normalizations of data, for instance the linear transfrom
 * the histogram equalization and the logarithmic transfrom
 * (mainly for for frequency image).
 */

#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "frame.h"
#include "controller.h"

/**
 * This class is a menu that is used to select the typed of normalization
 * between linear transfrom and histogram equalization
 * and logarithmic transform.
 */
class DynamicSelector : public QMenu, public Controller {
   Q_OBJECT

private:

   /** The index of the current normalization type. */
   QAction *_currentSelection;
   
   /** The action to select the linear transfrom normalization. */
   QAction *_st;

   /** The action to select the histogram equalization normalization. */
   QAction *_eq;
   
   /** The action to select the logarithmic transfom normalization. */
   QAction *_lt;

   /** The action to select the exponential transfom normalization. */
   QAction *_et;
   
private slots:

   /**
    * Checks the selected item and actives the related normalization.
    * @param act The action of the chosen normalization.
    */
   void checkItem( QAction* act );
   
public:

   /**  
    * Creates a menu for selecting the normalization type.
    * @param frame The frame that contains the data to be normalized.
    */
   DynamicSelector( const Frame *frame );

   void reset();

   signals:

   /**
    * Signal emitted when the selection is changed.
    * @param id The index of choosen normalisation.
    */
   void dynamic( int id );
};

#endif
