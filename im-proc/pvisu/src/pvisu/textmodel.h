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
 * @file textmodel.h
 *
 * This class supports a text widget.
 */

#ifndef TEXTMODEL_H
#define TEXTMODEL_H

#include <QtGui>
#include <pandore.h>

#include "model.h"

/**
 * This class defines the user interface to display texts.
 */
class TextModel: public Model {
   Q_OBJECT

private:

   QTextEdit *_contents;

   Frame* _frame;

public:

   /**
    * Creates a window that contains the menu,
    * and displays the specified text.
    * @param text a text.
    * @param file the name of input file.
    */
   TextModel( const char *text, const char *file );

   /**
    * Creates a window that contains the menu,
    * and displays the collection contents.
    * @param cols the input collection.
    * @param file the name of input file.
    */
   TextModel( const pandore::Collection &cols, const char *file );
   
private:
   /**
    * Creates a window that contains the menu,
    * and displays the colection contents.
    * @param file The name of input file.
    */
   void Init( const char *file );
   
   /**
    *  Displays the contents of the window.
    */
   void displayContents();

   /**
    * Sets the title of the window with the given filename
    * without its extension and directory.
    * @param file The absolute filename.
    */
   void CreateTitle( const char *file );

   void setParent( Frame *frame );

   void save( const QString filename, const QString extension ) { }

   void changeCursorVisibility() { }

   /**
    * Called when a key is pressed.
    * Opens a window, or a menu, or moves the cursor.
    * @param e The key event
    */
   void keyPressEvent( QKeyEvent *e );  
};

#endif
