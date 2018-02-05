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
 * @file textmodel.cpp
 *
 * This widget displays a text or a collection contents.
 */

#include <QtGui>
#include <list>
#include <pandore.h>
using namespace pandore;

#include "frame.h"
#include "textmodel.h"

void TextModel::Init( const char *filename ) {
   setMaximumSize(1000,10000);
   storeFilename(filename);

   _contents = new QTextEdit();
   _contents->setReadOnly(true);
   QFont f = _contents->font();
   f.setPointSize(8);
   _contents->setFont(f); 
   _contents->setWordWrapMode(QTextOption::WrapAnywhere);

   QVBoxLayout *layout = new QVBoxLayout(this);
   layout->setContentsMargins(0,0,0,0);
   layout->addWidget(_contents);
}

TextModel::TextModel( const char *text, const char *filename ) {
   Init(filename);
   if (text != NULL) {
      _contents->insertPlainText(QString(text));
   }
}

/**
 * Comparison :  consider the second part of the name that are numbers.
 */
bool lowerFunction( const std::string& first, const std::string& second ) {
   unsigned pos1 =  first.find_first_of('.');
   unsigned pos2 =  second.find_first_of('.');

   // No numerical part in the name.
   if (pos1 == std::string::npos || pos2 == std::string::npos) {
      return first.compare(second) <= 0;
   }

   std::string first1 = first.substr(0,pos1);
   std::string second1 = second.substr(0,pos2);

   std::string first2 = first.substr(pos1);
   std::string second2 = second.substr(pos2);

   int order = first1.compare(second1);
   if (order == 0 ) {
      if (first2.length() < second2.length()) {
	 return true;
      } else if (first2.length() > second2.length()) {
	 return false;
      } else {
	 return first2.compare(second2) < 0;
      }
   } else {
      return order < 0;
   }
}

TextModel::TextModel( const Collection &cols, const char *filename ) {
   Init(filename);
   char tmp[512];
   const int MAXLENGTH = 500;
   std::list<std::string> l = cols.List();

   // Change the default string order....
   l.sort(lowerFunction);

   _contents->setTextColor(Qt::red);
   snprintf(tmp, sizeof(tmp), "COLLECTION\nNumber of elements: %d\n", (int)l.size());
   _contents->insertPlainText(QString(tmp));

   std::list<std::string>::const_iterator i;
   for (i = l.begin(); i != l.end(); ++i) {
      BundledObject* bo = cols.Get(*i);
      char attributeType[256];
      char attributeName[256];

      strncpy(attributeType, bo->Type().c_str(), sizeof(attributeType));
      strncpy(attributeName, i->c_str(), sizeof(attributeName));
      _contents->setTextColor(Qt::blue);
      if (bo->NbrElements() < 2) {
	 snprintf(tmp, sizeof(tmp), "  * %s (%s): %d item\n", attributeName, attributeType, bo->NbrElements());
      } else {
	 snprintf(tmp, sizeof(tmp), "  * %s (%s): %d items\n", attributeName, attributeType, bo->NbrElements());
      }
      tmp[sizeof(tmp) - 1]= 0;
      _contents->insertPlainText(QString(tmp));
      _contents->setTextColor(Qt::black);
      const std::string name(attributeName);
      if (!strcasecmp(attributeType,"Double")) {
         Double &f = cols.GetValue(name, (Double*)0);
         snprintf(tmp, sizeof(tmp), "%f\n", f);
         _contents->insertPlainText(QString(tmp));
      } else if (!strcasecmp(attributeType,"Float")) {
	 Float &f = cols.GetValue(name, (Float*)0);
	 snprintf(tmp, sizeof(tmp), "%f\n", f);
	 _contents->insertPlainText(QString(tmp));
      } else if (!strcasecmp(attributeType,"Long")) {
	 Long &l = cols.GETVALUE(name, Long);
	 snprintf(tmp, sizeof(tmp), "%d\n", l);
	 _contents->insertPlainText(QString(tmp));
      } else if (!strcasecmp(attributeType,"Ulong")) {
	 Ulong &l = cols.GETVALUE(name, Ulong);
	 snprintf(tmp, sizeof(tmp), "%u\n", l);
	 _contents->insertPlainText(QString(tmp));
      } else if (!strcasecmp(attributeType,"Short")) {
	 Short &l = cols.GETVALUE(name, Short);
	 snprintf(tmp, sizeof(tmp), "%hd\n", l);
	 _contents->insertPlainText(QString(tmp));
      } else if (!strcasecmp(attributeType,"Ushort")) {
	 Ushort &l = cols.GETVALUE(name, Ushort);
	 snprintf(tmp, sizeof(tmp), "%hu\n", l);
	 _contents->insertPlainText(QString(tmp));
      } else if (!strcasecmp(attributeType,"Char")) {
	 Char &uc = cols.GETVALUE(name, Char);
	 snprintf(tmp, sizeof(tmp), "%d\n", (int)uc);
	 _contents->insertPlainText(QString(tmp));
      } else if (!strcasecmp(attributeType,"Uchar")) {
	 Uchar &uc = cols.GETVALUE(name, Uchar);
	 snprintf(tmp, sizeof(tmp), "%d\n",  (int)uc);
	 _contents->insertPlainText(QString(tmp));
      } else if (!strcasecmp(attributeType,"Array:Double")) {
	 Double *tab = cols.GETARRAY(name, Double);
	 Long taille = cols.GETARRAYSIZE(name, Double);
	 for (int j = 0; j < MIN(taille, MAXLENGTH); j++) {
            snprintf(tmp, sizeof(tmp), "%f\t",  tab[j]);
            _contents->insertPlainText(QString(tmp));
	 }
	 if (taille>MAXLENGTH) {
	    _contents->insertPlainText(QString(" (... file too large)"));
	 }
	 _contents->insertPlainText(QString("\n"));
      } else if (!strcasecmp(attributeType, "Array:Float")) {
	 Float *tab = cols.GETARRAY(name, Float);
	 Long taille = cols.GETARRAYSIZE(name, Float);
	 for (int j = 0; j < MIN(taille, MAXLENGTH); j++) {
	    snprintf(tmp, sizeof(tmp), "%f\t",  tab[j]);
	    _contents->insertPlainText(QString(tmp));
	 }
	 if (taille>MAXLENGTH) {
	    _contents->insertPlainText(QString(" (... file too large)"));
	 }
	 _contents->insertPlainText(QString("\n"));
      } else if (!strcasecmp(attributeType, "Array:Long")) {
	 Long *tab = cols.GETARRAY(name, Long);
	 Long taille = cols.GETARRAYSIZE(name, Long);
	 for (int j = 0; j < MIN(taille, MAXLENGTH); j++) {
	    snprintf(tmp, sizeof(tmp), "%d\t", tab[j]);
	    _contents->insertPlainText(QString(tmp));
	 }
	 if (taille>MAXLENGTH) {
	    _contents->insertPlainText(QString(" (... file too large)"));
	 }
	 _contents->insertPlainText(QString("\n"));
      } else if (!strcasecmp(attributeType, "Array:Ulong")) {
	 Ulong *tab = cols.GETARRAY(name, Ulong);
	 pandore::Long taille = cols.GETARRAYSIZE(name, Ulong);
	 for (int j = 0; j < MIN(taille, MAXLENGTH); j++) {
	    snprintf(tmp, sizeof(tmp), "%u\t",  tab[j]);
	    _contents->insertPlainText(QString(tmp));
	 }
	 if (taille>MAXLENGTH) {
	    _contents->insertPlainText(QString(" (... file too large)"));
	 }
	 _contents->insertPlainText(QString("\n"));
      } else if (!strcasecmp(attributeType, "Array:Short")) {
	 Short *tab = cols.GETARRAY(name, Short);
	 Long taille = cols.GETARRAYSIZE(name, Short);
	 for (int j = 0; j < MIN(taille, MAXLENGTH); j++) {
	    snprintf(tmp, sizeof(tmp), "%hd\t", tab[j]);
	    _contents->insertPlainText(QString(tmp));
	 }
	 if (taille>MAXLENGTH) {
	    _contents->insertPlainText(QString(" (... file too large)"));
	 }
	 _contents->insertPlainText(QString("\n"));
      }  else if (!strcasecmp(attributeType, "Array:UShort")) {
         Ushort *tab = cols.GETARRAY(name, Ushort);
         Long taille = cols.GETARRAYSIZE(name, Ushort);
	 for (int j = 0; j < MIN(taille, MAXLENGTH); j++) {
	    snprintf(tmp, sizeof(tmp), "%hu\t", tab[j]);
	    _contents->insertPlainText(QString(tmp));
         }
	 if (taille>MAXLENGTH) {
	    _contents->insertPlainText(QString(" (... file too large)"));
	 }
         _contents->insertPlainText(QString("\n"));
      } else if (!strcasecmp(attributeType, "Array:Char")) {
	 Char *tab = cols.GETARRAY(name, Char);
	 Long taille = cols.GETARRAYSIZE(name, Char);
	 for (int j = 0; j < MIN(taille, MAXLENGTH); j++) {
	    snprintf(tmp, sizeof(tmp), "%d\t", (int)tab[j]);
	    _contents->insertPlainText(QString(tmp));
	 }
	 if (taille>MAXLENGTH) {
	    _contents->insertPlainText(QString(" (... file too large)"));
	 }
	 _contents->insertPlainText(QString("\n"));
      } else if (!strcasecmp(attributeType, "Array:Uchar")) {
	 Uchar *tab = cols.GETARRAY(name, Uchar);
	 Long taille = cols.GETARRAYSIZE(name, Uchar);
	 for (int j = 0; j < MIN(taille, MAXLENGTH); j++) {
	    snprintf(tmp, sizeof(tmp), "%d\t", (int)tab[j]);
	    _contents->insertPlainText(QString(tmp));
	 }
	 if (taille>MAXLENGTH) {
	    _contents->insertPlainText(QString(" (... file too large)"));
	 }
	 _contents->insertPlainText(QString("\n"));
      } else {
	 _contents->insertPlainText(QString(" unprintable value(s)\n"));
      }
   }
   _contents->moveCursor(QTextCursor::Start);
}

void TextModel::keyPressEvent( QKeyEvent *e ) {
   switch (e->key()) {
   case Qt::Key_O :
      _frame->openImage();
      break;
   case Qt::Key_Q :
      _frame->closeApplication();
   default :
      break;
   }
}

void TextModel::setParent( Frame *frame ) {
   _frame = frame;
   frame->setContent(this);
}
