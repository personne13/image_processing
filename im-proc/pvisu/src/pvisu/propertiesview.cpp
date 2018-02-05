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
 * @file propertiesview.cpp
 * 
 * Defines the class which shows in a window the information
 * about the input image.
 */

#include "propertiesview.h"

PropertiesView::PropertiesView( const ImageModel *model ) {
   setAttribute(Qt::WA_QuitOnClose, false);
   QString temp = model->windowTitle();
   temp.append(" - Properties");
   setWindowTitle(temp);
   const SourceImage *ims = model->getSourceImage();
   temp = QString("<table border = 0>");
   temp.append("<tr><th>Type</th><td><font color = red>");
   temp.append(ims->getType());
   temp.append("</font></td></tr><tr><th>Content</th><td>");
   temp.append(ims->getContent());
   temp.append("</td></tr><tr><th>Size</th><td>");
   temp.append(ims->getSize());

   char tempString[256];
   if (ims->getPixel() != 0) {
      temp.append("</td></tr><tr><th>Pixel</th><td>");
      temp.append(ims->getPixel());
      temp.append("</td></tr><tr><th>Min value</th><td>");
      if (ims->getTypeval() == SourceImage::tfloat) {
	 snprintf(tempString, sizeof(tempString), "%#.3g", ims->getMinval());
      } else {
	 snprintf(tempString, sizeof(tempString), "%.0f", ims->getMinval());
      }
      temp.append(tempString);
      temp.append("</td></tr><tr><th>Max value</th><td>");
      if (ims->getTypeval() == SourceImage::tfloat) {
	 snprintf(tempString, sizeof(tempString), "%#.3g", ims->getMaxval());
      } else {
	 snprintf(tempString, sizeof(tempString), "%.0f", ims->getMaxval());
      }
      temp.append(tempString);
      if (ims->getEtc() != 0) {
	 temp.append("</td></tr><tr><th>");
	 temp.append(ims->getEtc());
      }
   } else if (ims->getLabel() != 0) {
      temp.append("</td></tr><tr><th>Label</th><td>");
      temp.append(ims->getLabel());
      temp.append("</td></tr><tr><th>Label max</th><td>");
      temp.append(ims->getLabelMax());
   } else {
      temp.append("</td></tr><tr><th>Node</th><td>");
      temp.append(ims->getNode());
      temp.append("</td></tr><tr><th>Total nodes</th><td>");
      temp.append(ims->getTotalNodes());
   }
   temp.append("</td></tr></table>");
   QTextEdit *properties = new QTextEdit(this);
   properties->setReadOnly(true);
   properties->setHtml(temp);
   setCentralWidget(properties);
}
