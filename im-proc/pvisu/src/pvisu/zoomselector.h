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
 * @file zoomselector.h
 * 
 * Defines a submenu for selecting the factor zoom.
 */

#ifndef ZOOM_H
#define ZOOM_H

#include "frame.h"
#include "controller.h"


/**
 * This class is a Controller for selecting the zoom factor in a menu.
 * The current zoom factor is marked as checked in the submenu.
 */
class ZoomSelector: public QMenu, public Controller {
   Q_OBJECT

private:
   
   /** The index in the menu of the current zoom. */
   QAction *_currentZoom;

   /** The action for seting the zoom factor to 400%. */
   QAction *_z400;

   /** The action for seting the zoom factor to 300%. */
   QAction *_z300;

   /** The action for seting the zoom factor to 200%. */
   QAction *_z200;

   /** The action for seting the zoom factor to 100%. */
   QAction *_z100;

   /** The action for seting the zoom factor to 50%. */
   QAction *_z50;

   /** The action for seting the zoom factor to 33%. */
   QAction *_z33;

   /** The action for seting the zoom factor to 25%. */
   QAction *_z25;

public:

   /**
    * Creates a menu for selecting the zoom.
    * @param model The model that contains the data.
    */
   ZoomSelector(const Frame *model);

   signals:
   
   /**
    * Emited for changing the zoom factor of the model.
    * @param factor The new factor zoom.
    */
   void newZoom( float factor );

private slots:

   /**
    * Sets the selected zoom item checked and emits the related signal.
    * @param id The index of item selected.
    */
   void zoomSelected( QAction* id );
   
   /**
    * Increments or decrements the zoom factor.
    * @param increment True: increment. False: decrement.
    */
   void keyZoom( bool increment );
};

#endif
