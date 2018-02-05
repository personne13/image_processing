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
 * @file model.h
 *
 * Defines the class with the data.
 */

#ifndef MODEL_H
#define MODEL_H

#if QT_VERSION >= 0x050000
// Qt5 code
#include <QtWidgets>
#else
// Qt4 code
#include <QtGui>
#endif

class Frame;

/**
 * The class model of the MVC architectural Design Pattern.
 */
class Model: public QWidget {
private:
   char *_directory;

public:
   ~Model() {
      delete[] _directory;
   }

   virtual void setParent(Frame*) = 0;

   char * getDirectory() const { return _directory; }

   virtual QRect getRubberBand( float zoom = 1.0f ) {
      return QRect(0,0,0,0);
   }
   void storeFilename( const char *filename ) {
      // Directory
      int i = strlen(filename);
      for ( ; i >= 0; i--) {
	 if (filename[i]=='/' || filename[i] == '\\') 
	    break;
      }
      _directory = new char[i + 2];
      memcpy(_directory, filename, (i + 1) * sizeof(char));
      _directory[i + 1] = '\0';
      // Filename
      i = strlen(filename);
      for ( ; i >= 0; i--) {
	 if (filename[i] == '/' || filename[i] == '\\')  {
	    break;
	 }
      }
      setWindowTitle(QString(&filename[++i]));
   }

   virtual void changeCursorVisibility() = 0;
   
   virtual void save( const QString filename, const QString extension ) = 0;

   virtual void saveSelection( const QString filename, const QString extension ) { }
};

#endif
