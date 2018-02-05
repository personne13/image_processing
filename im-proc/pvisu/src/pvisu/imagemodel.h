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
 * @file imagemodel.h
 *
 * This class manipulates the image content from Viewer
 * and Controller
 */

#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QtGui>
#include <pandore.h>

#include <stack>
#include <cmath>

#include "model.h"
#include "imagevisu.h"

extern bool DRAWINGMODE;

/**
 * This structure defines the coordinate
 * and the color of a drawing point.
 * for 
 */
class DrawingPoint {
public:
   int x;
   int y;
   DrawingPoint() {
   }
   DrawingPoint(int xx, int yy): x(xx), y(yy) {
   }
};

class Frame;

/**
 * This class defines the user interface.
 * It containts the image data and the visualization parameters
 * such as the zoom, the plane, the brightness values...
 * Each related Viewer is a viewpoint on the image data.
 */
class ImageModel : public Model {   
   Q_OBJECT

private :
   /** The reference frame. */
   Frame *_frame;

   /** The source image which contains the real data. */
   const SourceImage *_sourceImage;

   /** The logic image which contains the image to be displayed. */
   const LogicImage *_logicImage;       
   
   /** The physic image which contains the image actually displayed. */
   QImage _physicImage;

   /** The scrollarea that supports the image displayed. */
   QScrollArea *_scrollImage;
   
   /** The window for thresholding the display image. */
   QMainWindow *_thresholdWindow;
   
   /** The window that displays the properties of the input image. */
   QMainWindow *_propertiesWindow;

   /** The histogram viewer. */
   QMainWindow *_histogramWindow;
   
   /** The row profile viewer. */
   QMainWindow *_profileRWindow;
   
   /** The column profile viewer. */
   QMainWindow *_profileCWindow;
   
   /** The controller for changing colors of the display image. */
   QMainWindow *_colorWindow;

   /** The pixel value viewer. */
   QMainWindow *_valuesWindow;

   /** The current position of the cursor. */
   QPoint _currentPosition;

   /** The rubber band coordinates. */
   QRect _rubberBand;
   
   /** The current zoom factor. */
   float _zoom;
   
   /** The current plane for 3D images. */
   long _currentPlane;

   /** The low threshold value. */
   float _thresholdL[3];
   
   /** The high threshold value. */
   float _thresholdH[3];

   /** The flag for displaying the red band. */
   int _red;

   /** The flag for displaying the green band. */
   int _green;

   /** The flag for displaying the blue band. */
   int _blue;

   /** The current brightness value. */
   int _brightness;
   
   /** The current contrast value. */
   int _contrast;

   /** The current lut. */
   Uchar _lut[3][MAXCOLS];

   /** The pointer to the output image. */
   Imx3duc *_outputImage;

   /** The image that contains the drawn points. */
   Imx3duc *_drawImage;

   /** The image that contains the last drawing. */
   Imx3duc *_lastDraw;

   /** The stack of the last drawing points - used by undo() */
   std::stack<DrawingPoint> _lastDrawingPoints;

   /** The direction of the original x axis in the new locate; */
   int _vectX[3];

   /** The direction of the original y axis in the new locate; */
   int _vectY[3];

   /** The direction of the original z axis in the new locate; */
   int _vectZ[3];

   int _dynamicFunction;

private slots:

   /**
    * Draws a line onto the current image,
    * from the an initial point to a final point.
    * @param xb the initial x cordinate.
    * @param yb the initial y cordinate.
    * @param xe the final x cordinate.
    * @param ye the final y cordinate.
    * @param col the color.
    */
   void drawLine( int xb, int yb, int xe, int ye, Uchar col );

private:
   
   /**
    * Rotate the image draw around the x axis.
    */
   void rotateDrawingX();

   /**
    * Rotate the image draw around the y axis.
    */
   void rotateDrawingY();

   /**
    * Rotate the image draw around the z axis.
    */
   void rotateDrawingZ();

   /**
    * Builds the physic image from the source image, the logic image,
    * the plane, the invert flag, the colors flags, and the zoom factor.
    */
   void buildPhysicImage();

   /**
    * A speedup version of buildPhysicImage() for the initial creation.
    */
   void buildInitialPhysicImage();
   
   /**
    * Builds the logic image from the source image.
    */
   void buildLogicImage();

   /**
    * Emits the Update signal when data or visualization parameters
    * have changed.
    */
   void callUpdate();

   /**
    * Builds the triplet RGB from the brightness and contrast values.
    * @param r The red value.
    * @param g The green value.
    * @param b The blue value.
    */
   QRgb setRgb( Uchar r, Uchar g, Uchar b );

   /**
    * Equalizes the histogram to reduce
    * the number of colors from maxval-minval to outbands.
    * @param outbands The number of levels for the output image.
    */
   void histogramEqualization( int outbands );

   /**
    * Normalizes values from 0 to outbands of the input data vector
    * into the output image.
    * @param outbands The number of levels for the output image.
    */
   void linearTransform( int outbands );

   /**
    * Builds the image logic from a region map labels.
    */
   void regionEncoding( );

   /**
    * Normalizes values from 0 to outbands of the input data vector
    * into the output image by using the exp transform. That is:
    * output=exp(input/c)-1 where c is a normalization factor
    * to map values between 0 and outband.
    * @param outbands The number of levels for the output image.
    */
   void exponentialTransform( int outbands );

   /**
    * Normalizes values from 0 to outbands of the input data vector
    * into the output image by using the log transform. That is:
    * output=c.log(1+input) where c is a normalization factor
    * to map values between 0 and outband.
    * @param outbands The number of levels for the output image.
    */
   void logarithmicTransform( int outbands );

   /**
    * Creates the original lut.
    */
   void buildOriginalLut();

   /**
    * Creates the random lut.
    */
   void buildRandomLut();

   /**
    * Creates the region lut.
    */
   void buildRegionLut();

   /**
    * Creates the rainbow lut.
    */
   void buildRainbowLut();

   /**
    * Creates the negative lut.
    */
   void buildNegativeLut();

   /**
    * Creates the noise lut.
    */
   void buildNoiseLut();

public: 

   /**
    * Creates a window that contains the menu, displays the image,
    * and the user can draw in the image.
    * @param ims The image that contains the source data.
    * @param file The name of input file.
    * @param imd Tne image that is used to draw.
    */
   ImageModel( const SourceImage *ims, const char *file, Imx3duc *imd=0 );
   
   /**
    * Quits the application.
    */
   ~ImageModel() {
      if (_thresholdWindow) {
	 delete _thresholdWindow;
      }
      if (_propertiesWindow) {
	 delete _propertiesWindow;
      }
      if (_colorWindow) {
	 delete _colorWindow;
      }
      if (_valuesWindow) {
	 delete _valuesWindow;
      }
      if (_histogramWindow) {
	 delete _histogramWindow;
      }
      if (_profileRWindow) {
	 delete _profileRWindow;
      }
      if (_profileCWindow) {
	 delete _profileCWindow;
      }
      delete _sourceImage;
      delete _logicImage;

      if (DRAWINGMODE) {
	 delete _lastDraw;
	 QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	 if(_vectX[2] == 1) {
	    rotateDrawingY();
	    rotateDrawingY();
	    rotateDrawingY();
	 } else if(_vectX[2] == -1) {
	    rotateDrawingY();
	 } else if(_vectY[2] == 1) {
	    rotateDrawingX();
	 } else if(_vectY[2] == -1) {
	    rotateDrawingX();
	    rotateDrawingX();
	    rotateDrawingX();
	 } else if(_vectZ[2] == -1) {
	    rotateDrawingY();
	    rotateDrawingY();
	 }
	 if(_vectX[1] == -1) {
	    rotateDrawingZ();
	    rotateDrawingZ();
	    rotateDrawingZ();
	 } else if(_vectX[1] == 1) {
	    rotateDrawingZ();
	 }else if(_vectY[1] == -1) {
	    rotateDrawingZ();
	    rotateDrawingZ();
	 }

	 Uchar *d = _outputImage->Vector(0);
	 Uchar *s = _drawImage->Vector(0);
	 for( ; s < _drawImage->Vector(0) + _drawImage->VectorSize(); ) {
	    *(d++) = *(s++);
	 }
	 delete _drawImage;
	 QApplication::restoreOverrideCursor();
      }
   }

   void setParent( Frame *frame );

   QImage getIcon();

   void undoDraw();

   void clearDraw();

   void changeCursorVisibility();

   void changeDynamicFunction( int id );

   bool isCursorVisible() const;

   bool isDynamisable() const;

   const SourceImage* getSourceImage() const {
      return _sourceImage;
   }

   const LogicImage* getLogicImage() const {
      return _logicImage;
   }

   QImage getPhysicImage( ) const {
      return _physicImage;
   }
   
   float getZoom() const {
      return _zoom;
   }

   float getThresholdL( int band ) const {
      return _thresholdL[band];
   }

   /**
    * @return The high threshold factor.
    */
   float getThresholdH( int band ) const {
      return _thresholdH[band];
   }

   /**
    * @return The current plane.
    */
   long getPlane() const {
      return _currentPlane;
   }
   
   /**
    * Sets the x and y positions of cursor and updates the related views.
    * @param x The x-coordinate in input image coordinate without zoom factor.
    * @param y The y-coordinate in input image coordinate without zoom factor.
    */
   void setPosition( long x, long y ) {
      if (x < _sourceImage->Width() && x >= 0) {
	 _currentPosition.setX(x);
      }
      if (y < _sourceImage->Height() && y >= 0) {
	 _currentPosition.setY(y);
      }
      callUpdate();
   }

   QPoint getCurrentPosition() const {
      return _currentPosition;
   }

   void startRubberBand( int x, int y );

   void endRubberBand( int x, int y );

   QRect getRubberBand( float zoom = 1.0f );

   /**
    * Sets all values of _lastDraw to zero.
    */
   void cleanLastDraw( );

   /**
    * @return The draw image.
    */
   Imx3duc* getDrawImage() const {
      return _drawImage;
   }

   /**
    * Draws a line between the last drawing point
    * (xb,yb) and the current point (xe,ye);
    * @param xb The x position of the start point.
    * @param yb The y position of the start point.
    * @param xe The x position of the end point.
    * @param ye The y position of the end point.
    */
   void drawLine( int xb, int yb, int xe, int ye );

   /**
    * Displays the specified string tin the status bar.
    */
   void displayStatus( char * message );

   void save( const QString filename, const QString extension );

   void saveSelection( const QString filename, const QString extension );

signals:
 
   /** 
    * Emited when updating all Viewer.
    */
   void askForUpdate();

   /**
    * Emited when the user wants to change the zoom factor by the keyboard.
    * @param plus True:increment the zoom. False: decrement the zoom.
    */
   void zoom( bool plus );

   /**
    * Emited when the user rotate the image and so change the number of plane.
    * @param nbPlane: The number of plane.
    */
   void changeNumberPlane( long nbPlane );

   /** Emited when something is draw. */
   void draw();

   /** Emited when a selection rectnagle is drawn. */
   void rubberBandSelection( bool enabled );

public slots:

   /**
    * Shows the histogram window.
    */
   void showHistogramWindow();
      
   /**
    * Shows the row profil window.
    */
   void showRowProfileWindow();

   /**
    * Shows the column profil window.
    */
   void showColumnProfileWindow();

   /**
    * Shows the pixel values window.
    */
   void showValuesWindow();

   /**
    * Shows the properties window.
    */
   void showPropertiesWindow();

   /**
    * Shows the colors window.
    */
   void showColorsWindow();

   /**
    * Shows the thresholding window.
    */
   void showThresholdingWindow();

   /**
    * Displays or not the red color.
    * @param on True/False.
    */
   void setRed( bool on );
   
   /**
    * Displays or not the green color.
    * @param on True/False.
    */
   void setGreen( bool on );

   /**
    * Displays or not the blue color.
    * @param on True/False.
    */
   void setBlue( bool on );

   /**
    * Sets the values for thresholding the display image,
    * and updates all related views.
    * @param band the band number.
    * @param thresholdL The low threshold value.
    */
   void changeThresholdL( int band, float thresholdL );

   /**
    * Sets the values for thresholding the display image,
    * and updates all related views.
    * @param band the band number.
    * @param thresholdH The high threshold value.
    */
   void changeThresholdH( int band, float thresholdH );

   /**
    * Changes the zoom factor
    * and updates display.
    * @param zoom The zoom factor.
    */
   void changeZoom( float zoom );

   /**
    * Changes the current plane of the image
    * @param p The new plane number.
    */
   void changePlane( int p );

   /**
    * Changes the current lut.
    * @param id The index of the lut.
    */
   void changeLut( int id );

   /**
    * Sets the brightness value to the specified value.
    * Rebuilds the display image from visualization parameters.
    * @param val The value of the brightness.
    */
   void setBrightness( int val );

   /**
    * Sets the contrast value to the specified value.
    * Rebuilds the display image from visualization parameters.
    * @param val The value of the contrast.
    */
   void setContrast( int val );

   /**
    * Rotate the source image around the x axis.
    */
   void rotateX();

   /**
    * Rotate the source image around the y axis.
    */
   void rotateY();

   /**
    * Rotate the source image around the z axis.
    */
   void rotateZ();


   /**
    * Sets the colors configuration to the default settings.
    */
   void resetColor();

protected :

   /**
    * Called when a key is pressed.
    * Opens a window, or a menu, or moves the cursor.
    * @param e The key event
    */
   void keyPressEvent( QKeyEvent *e );  
};

#endif
