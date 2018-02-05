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
 * @file dynamicselector.cpp
 *
 * A menu for changing the image dynamic.
 */

#include "dynamicselector.h"

DynamicSelector::DynamicSelector( const Frame *frame ): QMenu("&Dynamic") {
   _st = addAction(tr("&Linear transform"));
   _st->setCheckable(true);
   _eq = addAction(tr("&Histogram equalization"));
   _eq->setCheckable(true);
   _lt = addAction(tr("&Logarithmic transform"));
   _lt->setCheckable(true);
   _et = addAction(tr("&Exponential transform"));
   _et->setCheckable(true);
   
   reset();

   connect(this, SIGNAL(triggered(QAction*)), this, SLOT(checkItem(QAction*)));
   connect(this, SIGNAL(dynamic(int)), frame, SLOT(changeDynamic(int)));
}

void DynamicSelector::checkItem( QAction *act ) {
   act->setChecked(true);
   if (act != _currentSelection) {
      _currentSelection->setChecked(false);
      _currentSelection = act;
      if (act == _st) {
	 emit dynamic(Frame::linear);
      } else if (act == _eq) {
	 emit dynamic(Frame::equalization);
      } else if (act == _lt) {
	 emit dynamic(Frame::logarithmic);
      } else  {
	 emit dynamic(Frame::exponential);
      }
   }
}

void DynamicSelector::reset() {
   _currentSelection = _st;
   _currentSelection->setChecked(true);
   _eq->setChecked(false);
   _lt->setChecked(false);
   _et->setChecked(false);
}
