/****************************************************************************
** Meta object code from reading C++ file 'imagemodel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../imagemodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imagemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ImageModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      32,   27,   11,   11, 0x05,
      51,   43,   11,   11, 0x05,
      75,   11,   11,   11, 0x05,
      90,   82,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     132,  116,   11,   11, 0x08,
     164,   11,   11,   11, 0x0a,
     186,   11,   11,   11, 0x0a,
     209,   11,   11,   11, 0x0a,
     235,   11,   11,   11, 0x0a,
     254,   11,   11,   11, 0x0a,
     277,   11,   11,   11, 0x0a,
     296,   11,   11,   11, 0x0a,
     324,  321,   11,   11, 0x0a,
     337,  321,   11,   11, 0x0a,
     352,  321,   11,   11, 0x0a,
     382,  366,   11,   11, 0x0a,
     426,  410,   11,   11, 0x0a,
     459,  454,   11,   11, 0x0a,
     479,  477,   11,   11, 0x0a,
     499,  496,   11,   11, 0x0a,
     518,  514,   11,   11, 0x0a,
     537,  514,   11,   11, 0x0a,
     554,   11,   11,   11, 0x0a,
     564,   11,   11,   11, 0x0a,
     574,   11,   11,   11, 0x0a,
     584,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ImageModel[] = {
    "ImageModel\0\0askForUpdate()\0plus\0"
    "zoom(bool)\0nbPlane\0changeNumberPlane(long)\0"
    "draw()\0enabled\0rubberBandSelection(bool)\0"
    "xb,yb,xe,ye,col\0drawLine(int,int,int,int,Uchar)\0"
    "showHistogramWindow()\0showRowProfileWindow()\0"
    "showColumnProfileWindow()\0showValuesWindow()\0"
    "showPropertiesWindow()\0showColorsWindow()\0"
    "showThresholdingWindow()\0on\0setRed(bool)\0"
    "setGreen(bool)\0setBlue(bool)\0"
    "band,thresholdL\0changeThresholdL(int,float)\0"
    "band,thresholdH\0changeThresholdH(int,float)\0"
    "zoom\0changeZoom(float)\0p\0changePlane(int)\0"
    "id\0changeLut(int)\0val\0setBrightness(int)\0"
    "setContrast(int)\0rotateX()\0rotateY()\0"
    "rotateZ()\0resetColor()\0"
};

void ImageModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ImageModel *_t = static_cast<ImageModel *>(_o);
        switch (_id) {
        case 0: _t->askForUpdate(); break;
        case 1: _t->zoom((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->changeNumberPlane((*reinterpret_cast< long(*)>(_a[1]))); break;
        case 3: _t->draw(); break;
        case 4: _t->rubberBandSelection((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->drawLine((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< Uchar(*)>(_a[5]))); break;
        case 6: _t->showHistogramWindow(); break;
        case 7: _t->showRowProfileWindow(); break;
        case 8: _t->showColumnProfileWindow(); break;
        case 9: _t->showValuesWindow(); break;
        case 10: _t->showPropertiesWindow(); break;
        case 11: _t->showColorsWindow(); break;
        case 12: _t->showThresholdingWindow(); break;
        case 13: _t->setRed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->setGreen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->setBlue((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->changeThresholdL((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 17: _t->changeThresholdH((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 18: _t->changeZoom((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 19: _t->changePlane((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->changeLut((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->setBrightness((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->setContrast((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->rotateX(); break;
        case 24: _t->rotateY(); break;
        case 25: _t->rotateZ(); break;
        case 26: _t->resetColor(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ImageModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ImageModel::staticMetaObject = {
    { &Model::staticMetaObject, qt_meta_stringdata_ImageModel,
      qt_meta_data_ImageModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ImageModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ImageModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ImageModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ImageModel))
        return static_cast<void*>(const_cast< ImageModel*>(this));
    return Model::qt_metacast(_clname);
}

int ImageModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Model::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    }
    return _id;
}

// SIGNAL 0
void ImageModel::askForUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ImageModel::zoom(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ImageModel::changeNumberPlane(long _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ImageModel::draw()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void ImageModel::rubberBandSelection(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
