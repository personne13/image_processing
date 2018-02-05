/****************************************************************************
** Meta object code from reading C++ file 'thresholdselector.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../thresholdselector.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'thresholdselector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ThresholdSelector[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   19,   18,   18, 0x05,
      45,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      80,   70,   18,   18, 0x08,
      97,   70,   18,   18, 0x08,
     114,   70,   18,   18, 0x08,
     131,   70,   18,   18, 0x08,
     148,   70,   18,   18, 0x08,
     165,   70,   18,   18, 0x08,
     197,  182,   18,   18, 0x08,
     217,  182,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ThresholdSelector[] = {
    "ThresholdSelector\0\0,\0lowThreshold(int,float)\0"
    "highThreshold(int,float)\0threshold\0"
    "thresholdL0(int)\0thresholdL1(int)\0"
    "thresholdL2(int)\0thresholdH0(int)\0"
    "thresholdH1(int)\0thresholdH2(int)\0"
    "band,threshold\0thresholdL(int,int)\0"
    "thresholdH(int,int)\0"
};

void ThresholdSelector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ThresholdSelector *_t = static_cast<ThresholdSelector *>(_o);
        switch (_id) {
        case 0: _t->lowThreshold((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 1: _t->highThreshold((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 2: _t->thresholdL0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->thresholdL1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->thresholdL2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->thresholdH0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->thresholdH1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->thresholdH2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->thresholdL((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->thresholdH((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ThresholdSelector::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ThresholdSelector::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ThresholdSelector,
      qt_meta_data_ThresholdSelector, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ThresholdSelector::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ThresholdSelector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ThresholdSelector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ThresholdSelector))
        return static_cast<void*>(const_cast< ThresholdSelector*>(this));
    if (!strcmp(_clname, "Controller"))
        return static_cast< Controller*>(const_cast< ThresholdSelector*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ThresholdSelector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void ThresholdSelector::lowThreshold(int _t1, float _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ThresholdSelector::highThreshold(int _t1, float _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
