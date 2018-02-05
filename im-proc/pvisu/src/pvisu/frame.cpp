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
 * @file frame.cpp
 */

#include <QtGui>
#include <QFileDialog>
#include <QStringList>
#include <QStringListIterator>
#include <QSettings>

#include "frame.h"
#include "textmodel.h"
#include "zoomselector.h"
#include "dynamicselector.h"
#include "rotateselector.h"
#include "planeselector.h"

#define ORGANIZATION "Greyc"
#define PROGRAM "pvisu"
#define ATTRIBUTE "cursor"

Frame::Frame( ): _drawingMenu(0), _pencilMode(true), _currentDirectory(0) {
   QSettings settings(ORGANIZATION, PROGRAM);
   if (settings.contains(ATTRIBUTE)) {
      _cursor = settings.value(ATTRIBUTE, true).toBool();
   }

   setAttribute(Qt::WA_QuitOnClose, true);
   setMinimumSize(220, 150);
   buildMenu();
   
   QWidget *central = new QWidget(this);
   QVBoxLayout *layout = new QVBoxLayout(central);
   layout->setContentsMargins(1, 0, 1, 0);

   _planeSelector = new PlaneSelector(this);
   layout->addWidget(_planeSelector);

   _canvas = new QWidget(this);
   layout->addWidget(_canvas);
   _canvas->setAcceptDrops(false);

   QWidget *footer = new QWidget();
   QHBoxLayout *layout2 = new QHBoxLayout(footer);
   layout2->setContentsMargins(0, 0, 0, 0);
   _statusBar = new QLabel(footer);
   layout2->addWidget(_statusBar);
   _counterBar = new QLabel(footer);
   _counterBar->setAlignment(Qt::AlignRight);
   layout2->addWidget(_counterBar);
   layout->addWidget(footer);
   
   setCentralWidget(central);
   central->setAcceptDrops(false);

   setAcceptDrops(true);
}

void Frame::dragEnterEvent(QDragEnterEvent *event) {
   if (event->mimeData()->hasText()) {
      event->acceptProposedAction();
   }
}

void Frame::dropEvent(QDropEvent *event) {
   QString imageFile;
   const QMimeData *mimeData = event->mimeData();

   if (mimeData->hasUrls()) {
      // Use only the first image.
      QList<QUrl> urlList = mimeData->urls();
      imageFile = urlList.at(0).path();
   } else if (mimeData->hasText()) {
      imageFile = mimeData->text();
   }
   
   if (!imageFile.isEmpty() && QFile::exists(imageFile)) {
      QByteArray ba = imageFile.toLocal8Bit();
      char * filename = strdup(ba.data());
      // Consume events before the intensive processing.
      // Simple solution: another would be to use a thread.
      displayCounter("");
      loadImage(filename);
  }
}

void Frame::buildMenu( ) {
   _menu = menuBar();

   QMenu *file = new QMenu("File");
   _openFile = file->addAction(tr("&Open"), this, SLOT(openImage()));
   _saveAs = file->addAction(tr("&Save current image as"), this, SLOT(saveAsImage()));
   _saveCurrentSelection = file->addAction(tr("&Save current selection as"), this, SLOT(saveCurrentSelection()));
   _saveCurrentSelection->setEnabled(false);
   file->addSeparator();
   _properties = file->addAction(tr("&Properties"), this, SLOT(showPropertiesWindow()));
   file->addSeparator();
   file->addAction(tr("&Quit"), this, SLOT(closeApplication()));
   _menu->addMenu(file);

   _viewsMenu = new QMenu(tr("Views"));
   _viewsMenu->addAction(tr("&Values"), this, SLOT(showValuesWindow()));
   _viewsMenu->addAction(tr("&Histogram"), this, SLOT(showHistogramWindow()));
   QMenu *profile = new QMenu(tr("Profile"));
   profile->addAction(tr("&Row"), this, SLOT(showRowProfileWindow()));
   profile->addAction(tr("&Column"), this, SLOT(showColumnProfileWindow()));
   _viewsMenu->addMenu(profile);
   _viewsMenu->addSeparator();
   _zoomMenu = new ZoomSelector(this);
   _viewsMenu->addMenu(_zoomMenu);
   _viewsMenu->addSeparator();
   QAction *cursor = _viewsMenu->addAction(tr("&Cursor"), this, SLOT(changeCursorVisibility()));
   _menu->addMenu(_viewsMenu);
   
   _toolsMenu = new QMenu(tr("Tools"));
   _toolsMenu->addAction(tr("&LookUp table"), this, SLOT(showColorsWindow()));
   _dynamicMenu = new DynamicSelector(this); 
   QAction *dy = _toolsMenu->addMenu(_dynamicMenu);
   QAction *th = _toolsMenu->addAction(tr("&Thresholding"), this, SLOT(showThresholdingWindow()));
   _rotateMenu = new QMenu("&Rotate");
   QAction *ro = _toolsMenu->addMenu(_rotateMenu);
   _menu->addMenu(_toolsMenu);

   if (DRAWINGMODE) {
      _drawingMenu = new QMenu(tr("Drawing"));
      _drawingMenu->addAction(tr("Undo"), this, SLOT(undoDraw()));
      _drawingMenu->addAction(tr("Clear"), this, SLOT(clearDraw()));
      QAction *drawMode = _drawingMenu->addAction(tr("Erase mode"), this, SLOT(changePencilMode()));
      _menu->addMenu(_drawingMenu);
      drawMode->setCheckable(true);
      drawMode->setChecked(!_pencilMode);
   }
   cursor->setCheckable(true);
   cursor->setChecked(_cursor);
}

void Frame::openImage( ) {
   QString directory;
   if (_currentDirectory == 0) {
      directory = tr(".");
   } else {
      directory = _currentDirectory->absolutePath();
   }
   QString imageFile = QFileDialog::getOpenFileName
      (this, 
       tr("Choose an image file"), 
       directory, 
       tr("Images (*.pan *.bmp *.gif *.png *.jpg *.jpeg *.tif *.ppm *.tiff)"));

   if (!imageFile.isEmpty() && QFile::exists(imageFile)) {
      QByteArray ba = imageFile.toLocal8Bit();
      char * filename = strdup(ba.data());
      // Consume events before the intensive processing.
      // Simple solution: another would be to use a thread.
#if !defined _WIN32 && ! defined WIN32
      while (QApplication::hasPendingEvents()) {
	 QApplication::processEvents();
      }
#endif
      displayCounter("");
      loadImage(filename);
   }
}

void Frame::loadImage( const char *filename ) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   Model *OpenFile(char*, Pobject *objs, Pobject **objd);
   Pobject *objs = LoadFile(filename, false);
   if (objs == NULL) {
      QImage *data = new QImage(filename);
      if (data->width() > 0) {
	 if (data->allGray()) {
	    Img2duc *ims = new Img2duc(data->height(), data->width());
	    objs = ims;
	    for (int y = 0; y < data->height(); y++) {
	       for (int x = 0; x < data->width(); x++) {
		  QRgb pixel = data->pixel(x, y);
		  (*ims)[y][x] = qRed(pixel);
	       }
	    }
	 } else {
	    Imc2duc *ims = new Imc2duc(data->height(), data->width());
	    objs = ims;
	    for (int y = 0; y < data->height(); y++) {
	       for (int x = 0; x < data->width(); x++) {
		  QRgb pixel = data->pixel(x, y);
		  (*ims)(0, y, x) = qRed(pixel);
		  (*ims)(1, y, x) = qGreen(pixel);
		  (*ims)(2, y, x) = qBlue(pixel);
	       }
	    }
	 }
	 delete data;
      }
   }
   if (objs != NULL) {
      Imx3duc *_imageDessin;
      delete _model;
      Model *visual = OpenFile(strdup(filename), objs, (Pobject**)&_imageDessin);
      visual->setParent(this);
   }
   QApplication::restoreOverrideCursor();
}

void Frame::saveCurrentSelection( ) {
   if (_model->getRubberBand().width() > 0) {
      QString directory;
      if (_currentDirectory == 0) {
	 directory = tr(".");
      } else {
	 directory = _currentDirectory->absolutePath();
      }
      QString fileName = QFileDialog::getSaveFileName( this, //parent
	 tr("Save current selection as image"),  // Caption
	 directory, // dir
	 tr("Image Files (*.pan *.bmp *.png *.jpg *.jpeg *.tif *.tiff)"));

      if (fileName == 0) {
	 return; // cancel button
      }
      QFileInfo fi(fileName);
      QString extension = fi.completeSuffix().toLower();
      _model->saveSelection(fileName, extension);
   }
}

void Frame::saveAsImage( ) {
   QString directory;
   if (_currentDirectory == 0) {
      directory = tr(".");
   } else {
      directory = _currentDirectory->absolutePath();
   }
   QString fileName = QFileDialog::getSaveFileName(this, //parent
      tr("Save current image"),  // Caption
      directory, // dir
      tr("Image Files (*.pan *.bmp *.png *.jpg *.jpeg *.tif *.tiff)"));
   
   if (fileName == 0) {
      return; // cancel button
   }
   
   QFileInfo fi(fileName);
   QString extension = fi.completeSuffix().toLower();

   _model->save(fileName, extension);
}

void Frame::changeZoom( float zoom ) {
   if (_model) { 
      ((ImageModel*)_model)->changeZoom(zoom);
   }
}

void Frame::changeDynamic(int id) {
   if (_model) {
      ((ImageModel*)_model)->changeDynamicFunction(id);
   }
}

char tmp[256];

void Frame::openPreviousImage( ) {
   if (_currentDirectory == 0) {
      _currentDirectory = new QDir(".");
   }
   _currentDirectory->setFilter(QDir::Files);
   QStringList filters;
   filters << "*.pan" << "*.png" << "*.jpg" << "*.jpeg" << "*.JPG" << "*.bmp" << "*.tif" << "*.tiff" << "*.gif"; 
   QStringList files = _currentDirectory->entryList(filters);

   QStringListIterator iterator(files);

   QString file = 0;
   int imageCount = files.size();
   int index = 0;
   while (iterator.hasNext()) {
      QString next = iterator.next();
      index++;
      if (next == windowTitle()) {
	 if (file != 0) {
	    snprintf(tmp, sizeof(tmp), "%d/%d", index - 1, imageCount);
	    loadImage(qPrintable(_currentDirectory->absoluteFilePath(file)));
	    _planeSelector->setNextButtonEnabled(true);
	 } else {
	    snprintf(tmp, sizeof(tmp), "%d/%d", index, imageCount);
	    _planeSelector->setPreviousButtonEnabled(false);
	 }
	 displayCounter(tmp);
 	 break;
      }
      file = next;
   }
}

void Frame::openNextImage( ) {
   if (_currentDirectory == 0) {
      _currentDirectory = new QDir(".");
   }
   _currentDirectory->setFilter(QDir::Files);
   QStringList filters;
   filters << "*.pan" << "*.png" << "*.jpg" << "*.jpeg" << "*.JPG" << "*.bmp" << "*.tif" << "*.tiff" << "*.gif"; 
   QStringList files = _currentDirectory->entryList(filters);

   if (files.size() == 0) {
      _planeSelector->setPreviousButtonEnabled(false);
      return;
   }

   QStringListIterator iterator(files);
   int imageCount = files.size();
   int index = 0;
   while (iterator.hasNext()) {
      QString file = iterator.next();
      index++;
      if (file == windowTitle()) {
	 if (iterator.hasNext()) {
	    file = iterator.next();
	    loadImage(qPrintable(_currentDirectory->absoluteFilePath(file)));
	    _planeSelector->setPreviousButtonEnabled(true);
	    snprintf(tmp, sizeof(tmp), "%d/%d", index + 1, imageCount);
	 } else {
	    _planeSelector->setNextButtonEnabled(false);
	    snprintf(tmp, sizeof(tmp), "%d/%d", index, imageCount);
	 }
	 displayCounter(tmp);
 	 break;
      }
   }
}

void Frame::showHistogramWindow() {
   if (_model) {
      ((ImageModel*)_model)->showHistogramWindow();
   }
}

void Frame::showRowProfileWindow() {
   if (_model) {
      ((ImageModel*)_model)->showRowProfileWindow();
   }
}

void Frame::showColumnProfileWindow() {
   if (_model) {
      ((ImageModel*)_model)->showColumnProfileWindow();
   }
}

void Frame::showValuesWindow() {
   if (_model) {
      ((ImageModel*)_model)->showValuesWindow();
   }
}

void Frame::showPropertiesWindow() {
   if (_model) {
      ((ImageModel*)_model)->showPropertiesWindow();
   }
}

void Frame::showColorsWindow() {
   if (_model) { ((ImageModel*)_model)->showColorsWindow(); }
}

void Frame::showThresholdingWindow() {
   if (_model) {
      ((ImageModel*)_model)->showThresholdingWindow();
   }
}

void Frame::changeCursorVisibility() {
   _cursor = !_cursor;
   QSettings settings(ORGANIZATION, PROGRAM);
   settings.setValue(ATTRIBUTE, _cursor);
   if (_model) {
      _model->changeCursorVisibility();
   }
}

void Frame::closeApplication( ) const {
   QApplication::closeAllWindows();
}

void Frame::changePencilMode() {
   _pencilMode = !_pencilMode;
} 

bool Frame::isPencilMode() {
   return _pencilMode;
}

void Frame::clearDraw( ) {
   if (_model) {
      ((ImageModel*)_model)->clearDraw();
   }
}

void Frame::undoDraw( ) {
   if (_model) {
      ((ImageModel*)_model)->undoDraw();
   }
}

void Frame::displayStatus( char * message ) {
   _statusBar->setText(QString(message));
}

void Frame::displayCounter( char * message ) {
   _counterBar->setText(QString(message));
}

void Frame::rubberBandSelection( bool enabled ) {
   _saveCurrentSelection->setEnabled(enabled);
}

void Frame::setContent( TextModel *model ) {
   setWindowTitle(model->windowTitle());
   if (model->windowTitle() == QString("stdin") || DRAWINGMODE) {
      _planeSelector->setNextButtonEnabled(false);
      _planeSelector->setPreviousButtonEnabled(false);
   } else {
      _planeSelector->setNextButtonEnabled(true);
      _planeSelector->setPreviousButtonEnabled(true);
   }
   if (_currentDirectory) {
      delete _currentDirectory;
   }
   _currentDirectory = new QDir(model->getDirectory());
   setWindowIcon(QApplication::windowIcon());
   _viewsMenu->setEnabled(false);
   _toolsMenu->setEnabled(false);
   _saveAs->setEnabled(false);
   _properties->setEnabled(false);
   if (DRAWINGMODE) {
      _drawingMenu->setEnabled(false);
      _openFile->setEnabled(false);
   } else {
      _openFile->setEnabled(true);
   }
   
   _planeSelector->setModel(model);
   
   if (_canvas->layout() != 0) {
      delete _canvas->layout();
   }
   _model = model;
   
   QVBoxLayout *layout = new QVBoxLayout(_canvas);
   layout->setContentsMargins(2, 0, 2, 0);
   layout->removeItem(0);
   layout->addWidget(model);
   model->setFocus();
}

void Frame::setContent( ImageModel *model ) {
   setWindowTitle(model->windowTitle());
   if (model->windowTitle() == QString("stdin") || DRAWINGMODE) {
      _planeSelector->setNextButtonEnabled(false);
      _planeSelector->setPreviousButtonEnabled(false);
   } else {
      _planeSelector->setNextButtonEnabled(true);
      _planeSelector->setPreviousButtonEnabled(true);
   }
   if (_currentDirectory) delete _currentDirectory;
   _currentDirectory = new QDir(model->getDirectory());
   setWindowIcon(QPixmap::fromImage(model->getIcon()));
   connect(model, SIGNAL(zoom(bool)), _zoomMenu, SLOT(keyZoom(bool)));
   connect(model, SIGNAL(rubberBandSelection(bool)), this, SLOT(rubberBandSelection(bool)));
   
   ((DynamicSelector*)_dynamicMenu)->reset();
   _viewsMenu->setEnabled(true);
   _toolsMenu->setEnabled(true);
   _dynamicMenu->setEnabled(model->isDynamisable());
   if (DRAWINGMODE) {
      _openFile->setEnabled(false);
   } else {
      _openFile->setEnabled(true);
   }
   _saveAs->setEnabled(true);
   _properties->setEnabled(true);
   _rotateMenu->clear();
   if (_drawingMenu) {
      _drawingMenu->setEnabled(true);
   }
   RotateSelector(_rotateMenu, model);
   
   _planeSelector->setModel(model);
   
   if (_canvas->layout() != 0) {
      delete _canvas->layout();
   }
   _model = model;
   
   QGridLayout *layout = new QGridLayout(_canvas);
   layout->setContentsMargins(2, 0, 2, 0);
   layout->removeItem(0);
   layout->addWidget(model, 0, 0);
   model->setFocus();
   if (!isVisible()) { // Set the window size for the first time only.
      resize(model->size());
   }
}
