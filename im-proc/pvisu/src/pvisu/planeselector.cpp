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
 * @file planeselector.cpp
 *
 * Defines a controller for changing the current plane of a 3D image.
 */

#include "planeselector.h"

PlaneSelector::PlaneSelector( QWidget* parent ): QWidget(parent) {
   setFixedHeight(20);
   QGridLayout *grille = new QGridLayout(this);
   grille->setContentsMargins(2, 1, 2, 0);
 
   _slider = new QSlider(Qt::Horizontal, this);
   _slider->setFixedHeight(8);
   _slider->setValue(0);
   _slider->setSingleStep(1);
   _slider->setPageStep(1);
   grille->addWidget(_slider, 0, 0, 0);
   
   _number = new QLCDNumber(4, this);
   _number->setSegmentStyle(QLCDNumber::Flat);
    grille->addWidget(_number, 0, 1, 0);
   _number->display(0);

   _previous = new QToolButton(parent);
   _previous->setArrowType(Qt::LeftArrow);
   connect(_previous, SIGNAL(pressed()), parent, SLOT(openPreviousImage()));
   grille->addWidget(_previous,0,2,0);

   _next = new QToolButton(parent);
   _next->setArrowType(Qt::RightArrow);
   connect(_next, SIGNAL(pressed()), parent, SLOT(openNextImage()));
   grille->addWidget(_next,0,3,0);
}

void PlaneSelector::addFocus() {
   _slider->setFocus();
}

void PlaneSelector::setModel( const TextModel * model ) { 
   _slider->setMaximum(0);
   _slider->setMinimum(0);
}

void PlaneSelector::setModel( const ImageModel *image ) {
   _slider->setMaximum(image->getSourceImage()->Depth() - 1);
   _slider->setMinimum(0);

   // Sets to the displayed number with the new value of the plane.
   connect(_slider, SIGNAL(valueChanged(int)), _number, SLOT(display(int)));
   connect(_slider, SIGNAL(valueChanged(int)), image, SLOT(changePlane(int)));
   connect(image, SIGNAL(changeNumberPlane(long)), this, SLOT(updateNbPlane(long)));
   connect(_slider, SIGNAL(sliderPressed()), this, SLOT(addFocus())); 
}

void PlaneSelector::mousePressEvent( QMouseEvent *e ) {
   e->accept();
   _slider->setFocus();
}

void PlaneSelector::updateNbPlane( long nbPlane ) {
   _slider->setMaximum((int)nbPlane - 1); 
   _slider->setValue(0);
}
