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
 * @file colorselector.cpp
 *
 * Defines a controller for modifying colors.
 */

#include "colorselector.h"

ColorSelector::ColorSelector( const ImageModel *model ) {
   setAttribute(Qt::WA_QuitOnClose, false);
   Qt::WindowFlags flags = this->windowFlags();
   this->setWindowFlags(flags|Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);

   QString temp = model->windowTitle();
   temp.append(" - Colors");
   setWindowTitle(temp);
   QWidget *central = new QWidget(this);
   setCentralWidget(central);
   QVBoxLayout *position = new QVBoxLayout(central);
  
   _brightTitle = new QLabel("brightness", central);
   _brightTitle->setAlignment(Qt::AlignHCenter);
   position->addWidget(_brightTitle);
   _brightNumber = new QLCDNumber(4, central);
   _brightNumber->setSegmentStyle(QLCDNumber::Flat);
   _brightNumber->display(0);
   position->addWidget(_brightNumber);
   _brightSlider = new QSlider(Qt::Horizontal, central);
   _brightSlider->setMinimum(-127);
   _brightSlider->setMaximum(127);
   _brightSlider->setValue(0);
   _brightSlider->setSingleStep(1);
   _brightSlider->setTracking(false);
   position->addWidget(_brightSlider);
  
   _contrastTitle = new QLabel("contrast", central);
   _contrastTitle->setAlignment(Qt::AlignHCenter);
   position->addWidget(_contrastTitle);
   _contrastNumber = new QLCDNumber(4, central);
   _contrastNumber->setSegmentStyle(QLCDNumber::Flat);
   _contrastNumber->display(0);
   position->addWidget(_contrastNumber);
   _contrastSlider = new QSlider(Qt::Horizontal, central);
   _contrastSlider->setMinimum(-127);
   _contrastSlider->setMaximum(127);
   _contrastSlider->setValue(0);
   _contrastSlider->setSingleStep(1);
   _contrastSlider->setTracking(false);
   position->addWidget(_contrastSlider);
  
   QWidget *buttons = new QWidget(central);
  
   QHBoxLayout *color = new QHBoxLayout(buttons);
   _red = new QCheckBox("Red", buttons);
   color->addWidget(_red);
   _green = new QCheckBox("Green", buttons);
   color->addWidget(_green);
   _blue = new QCheckBox("Blue", buttons);
   color->addWidget(_blue);
   position->addWidget(buttons);
   _red->setChecked(true);
   _green->setChecked(true);
   _blue->setChecked(true);

   QWidget *lut = new QWidget(central);
   _luts = new QButtonGroup(central);
   _luts->setExclusive(true);

   QGridLayout *grid = new QGridLayout(lut);
   _original = new QRadioButton("Original  ", lut);
   _luts->addButton(_original);
   grid->addWidget(_original, 0, 0);
   _negative = new QRadioButton("Negative  ", lut);
   _luts->addButton(_negative);
   grid->addWidget(_negative, 0, 1);
   _random = new QRadioButton("Random  ", lut);
   _luts->addButton(_random);
   grid->addWidget(_random, 0, 2);
   _region = new QRadioButton("Region  ", lut);
   _luts->addButton(_region);
   grid->addWidget(_region, 1, 0);
   _rainbow = new QRadioButton("Rainbow  ", lut);
   _luts->addButton(_rainbow);
   grid->addWidget(_rainbow, 1, 1);
   _noise = new QRadioButton("Noise  ", lut);
   _luts->addButton(_noise);
   grid->addWidget(_noise, 1, 2);
   _original->setChecked(true);
   position->addWidget(lut);
  
   _reset = new QPushButton("Reset", central);
   position->addWidget(_reset);
 
   connect(_red, SIGNAL(toggled(bool)), model, SLOT(setRed(bool)));
   connect(_green, SIGNAL(toggled(bool)), model, SLOT(setGreen(bool)));
   connect(_blue, SIGNAL(toggled(bool)), model, SLOT(setBlue(bool)));
   connect(_brightSlider, SIGNAL(valueChanged(int)), _brightNumber, SLOT(display(int)));
   connect(_contrastSlider, SIGNAL(valueChanged(int)), _contrastNumber, SLOT(display(int)));
   connect(_brightSlider, SIGNAL(valueChanged(int)), model, SLOT(setBrightness(int)));
   connect(_contrastSlider, SIGNAL(valueChanged(int)), model, SLOT(setContrast(int)));
   connect(_luts, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(selectLut(QAbstractButton*)));
   connect(_reset, SIGNAL(pressed()), this, SLOT(resetSettings()));
   connect(this, SIGNAL(reset()), model, SLOT(resetColor()));
   connect(this, SIGNAL(newLut(int)), model, SLOT(changeLut(int)));
}

void ColorSelector::selectLut( QAbstractButton *button ) {
   if (button == _original) {
      emit newLut(Frame::original);
   } else if (button == _negative) {
      emit newLut(Frame::negative);
   } else if (button == _region) {
      emit newLut(Frame::region);
   } else if (button == _random) {
      emit newLut(Frame::random);
   } else if (button == _rainbow) {
      emit newLut(Frame::rainbow);
   } else if (button == _noise) {
      emit newLut(Frame::noise);
   }
}

void ColorSelector::resetSettings( ) {
   _brightSlider->blockSignals(true);
   _brightSlider->setValue(0);
   _brightNumber->display(0);
   _brightSlider->blockSignals(false);
   _contrastSlider->blockSignals(true);
   _contrastSlider->setValue(0);
   _contrastNumber->display(0);
   _contrastSlider->blockSignals(false);
   _red->blockSignals(true);
   _green->blockSignals(true);
   _blue->blockSignals(true);
   _red->setChecked(true);
   _green->setChecked(true);
   _blue->setChecked(true);
   _red->blockSignals(false);
   _green->blockSignals(false);
   _blue->blockSignals(false);
   if (!_original->isChecked()) {
      _original->setChecked(true);
   }
   emit reset();
}
