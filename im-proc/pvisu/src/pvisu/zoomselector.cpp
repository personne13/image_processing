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
> * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 *
 *
 * For more information, refer to:
 * https://clouard.users.greyc.fr/Pandore
 */

/**
 * @file zoomselector.cpp
 *
 * Menu for selecting the factor zoom.
 */

#include "zoomselector.h"

ZoomSelector::ZoomSelector( const Frame *frame ): QMenu("Zoom") {
   _z400 = addAction("400%");
   _z400->setCheckable(true);
   _z300 = addAction("300%");
   _z300->setCheckable(true);
   _z200 = addAction("200%");
   _z200->setCheckable(true);
   _z100 = addAction("100%");
   _z100->setCheckable(true);
   _z50 = addAction("50%");
   _z50->setCheckable(true);
   _z33 = addAction("33%");
   _z33->setCheckable(true);
   _z25 = addAction("25%");
   _z25->setCheckable(true);
   _currentZoom = _z100;
   _currentZoom->setChecked(true);

   connect(this, SIGNAL(triggered(QAction*)), this, SLOT(zoomSelected(QAction*)));
   connect(this, SIGNAL(newZoom(float)), frame, SLOT(changeZoom(float)));
}

void ZoomSelector::zoomSelected( QAction *id ) {
   id->setChecked(true);
   if (id != _currentZoom) {
      _currentZoom->setChecked(false);
      _currentZoom = id;
      if (_currentZoom ==  _z400) {
	 emit newZoom(4.0F);
      } else if (_currentZoom == _z300) {
	 emit newZoom(3.0F);
      } else if (_currentZoom == _z200) {
	 emit newZoom(2.0F);
      } else if (_currentZoom == _z100) {
	 emit newZoom(1.0F);
      } else if (_currentZoom == _z50) {
	 emit newZoom(0.5F);
      } else if (_currentZoom == _z33) {
	 emit newZoom(0.33F);
      } else if (_currentZoom == _z25) {
	 emit newZoom(0.25F);
      }
   }
}

void ZoomSelector::keyZoom( bool increment ) {
   if (increment) {
      if (_currentZoom == _z300) {
	 zoomSelected(_z400);
      } else if (_currentZoom == _z200) {
	 zoomSelected(_z300);
      } else if (_currentZoom == _z100) {
      	 zoomSelected(_z200);
      } else if (_currentZoom == _z50) {
	 zoomSelected(_z100);
      } else if (_currentZoom == _z33) {
	 zoomSelected(_z50);
      } else if (_currentZoom == _z25) {
	 zoomSelected(_z33);
      }
   } else {
      if (_currentZoom == _z400) {
	 zoomSelected(_z300);
      } else if (_currentZoom == _z300) {
	 zoomSelected(_z200);
      } else if (_currentZoom == _z200) {
	 zoomSelected(_z100);
      } else if (_currentZoom == _z100) {
      	 zoomSelected(_z50);
      } else if (_currentZoom == _z50) {
      	 zoomSelected(_z33);
      } else if (_currentZoom == _z33) {
	 zoomSelected(_z25);
      }
   }
}
