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
 * @file frame.h
 *
 * Defines the class that supports the user interface.
 */

#ifndef FRAME_H
#define FRAME_H

#if QT_VERSION >= 0x050000
// Qt5 code
#include <QtWidgets>
#else
// Qt4 code
#include <QtGui>
#endif
#include <QDir>

#include "model.h"

class PlaneSelector;
class ImageModel;
class TextModel;

/**
 * This class defines the user interface.
 * It containts the image data and the visualization parameters
 * such as the zoom, the plane, the brightness values...
 * Each related Viewer is a viewpoint on the image data.
 */
class Frame : public QMainWindow {
   Q_OBJECT
   
public:
   
   /**
    * The enumeration of the supported luts.
    */
   typedef enum {
      original, /**< The original lut. */  
      random,   /**< A lut generates randomly. */
      region,   /**< The lut that representes the different region of the image. */
      rainbow,  /**< The rainbow lut. */
      negative,  /**< The lut where colors are inversed. */
      noise     /**< The lut that represente the noise of the image.*/
   } Lut;

   /**
    * The enumeration of the supported different types of data normalizations.
    */
   typedef enum {
      linear,       /**< The linear transform of the gray levels. */
      equalization, /**< The histogram equalization dynamic. */
      logarithmic,  /**< The logarithmic transform of the gray levels. */
      exponential   /**< The exponential transform of the gray levels. */
   } Normalization;

private:
   
   /** The menu bar. */
   QMenuBar *_menu;

   QAction *_properties;
   QAction *_saveCurrentSelection;
   QAction *_saveAs;
   QAction *_openFile;

   QWidget *_canvas;

   QMenu *_viewsMenu;
   QMenu *_toolsMenu;
   QMenu *_drawingMenu;

   /** The controller for selecting the zoom. */
   QMenu *_zoomMenu;

   /** The controller for selecting the dynamic. */
   QMenu *_dynamicMenu;

   /** The controller for changing the point of view for 3D image. */
   QMenu *_rotateMenu;
   
   QDir* _currentDirectory;

   QLabel *_statusBar;
   QLabel *_counterBar;

   PlaneSelector *_planeSelector;

   /** This property holds whether the cursor is visible. */
   bool _cursor;

   /** The flag between pencil or erase mode. */
   bool _pencilMode;

   /** The image displayed. */
   Model * _model;
		 
private slots:

   void showHistogramWindow();

   void showRowProfileWindow();

   void showColumnProfileWindow();

   void showValuesWindow();

   void showPropertiesWindow();

   void showColorsWindow();

   void showThresholdingWindow();

   void changeCursorVisibility();

   void changePencilMode();

   void openPreviousImage();

   void openNextImage();

   void undoDraw();

   void clearDraw();

public slots:

   void openImage();

   void saveAsImage();

   void saveCurrentSelection();

   void closeApplication() const;

   void changeZoom( float zoom );

   void changeDynamic( int id );

   void rubberBandSelection( bool enabled );

private:

   /**
    * Builds the menu with all items.
    */
   void buildMenu();

   void loadImage( const char *filename );

public: 

   /**
    * Creates a window that contains the menu, displays the image,
    * and the user can draw in the image.
    * @param ims The image that contains the source data.
    * @param file The name of input file.
    * @param imd Tne image that is used to draw.
    */
   Frame();
   
   /**
    * Quits the application.
    */
   ~Frame() {
      delete _currentDirectory;
      delete _model;
   }

   void displayStatus( char * message );

   void displayCounter( char * message );

   void setContent( TextModel *model );

   void setContent( ImageModel *model );

   Model *getImage( ) const { return _model; }

   /**
    * Sets all values of _lastDraw to zero.
    */
   void cleanLastDraw( );

   bool isCursorVisible( ) const { return _cursor; }

   bool isPencilMode();

   void dropEvent( QDropEvent * evt);

   void dragEnterEvent(QDragEnterEvent *event);
};

#endif
