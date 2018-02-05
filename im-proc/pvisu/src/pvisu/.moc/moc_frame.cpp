/****************************************************************************
** Meta object code from reading C++ file 'frame.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../frame.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'frame.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Frame[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x08,
      29,    6,    6,    6, 0x08,
      52,    6,    6,    6, 0x08,
      78,    6,    6,    6, 0x08,
      97,    6,    6,    6, 0x08,
     120,    6,    6,    6, 0x08,
     139,    6,    6,    6, 0x08,
     164,    6,    6,    6, 0x08,
     189,    6,    6,    6, 0x08,
     208,    6,    6,    6, 0x08,
     228,    6,    6,    6, 0x08,
     244,    6,    6,    6, 0x08,
     255,    6,    6,    6, 0x08,
     267,    6,    6,    6, 0x0a,
     279,    6,    6,    6, 0x0a,
     293,    6,    6,    6, 0x0a,
     316,    6,    6,    6, 0x0a,
     340,  335,    6,    6, 0x0a,
     361,  358,    6,    6, 0x0a,
     388,  380,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Frame[] = {
    "Frame\0\0showHistogramWindow()\0"
    "showRowProfileWindow()\0showColumnProfileWindow()\0"
    "showValuesWindow()\0showPropertiesWindow()\0"
    "showColorsWindow()\0showThresholdingWindow()\0"
    "changeCursorVisibility()\0changePencilMode()\0"
    "openPreviousImage()\0openNextImage()\0"
    "undoDraw()\0clearDraw()\0openImage()\0"
    "saveAsImage()\0saveCurrentSelection()\0"
    "closeApplication()\0zoom\0changeZoom(float)\0"
    "id\0changeDynamic(int)\0enabled\0"
    "rubberBandSelection(bool)\0"
};

void Frame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Frame *_t = static_cast<Frame *>(_o);
        switch (_id) {
        case 0: _t->showHistogramWindow(); break;
        case 1: _t->showRowProfileWindow(); break;
        case 2: _t->showColumnProfileWindow(); break;
        case 3: _t->showValuesWindow(); break;
        case 4: _t->showPropertiesWindow(); break;
        case 5: _t->showColorsWindow(); break;
        case 6: _t->showThresholdingWindow(); break;
        case 7: _t->changeCursorVisibility(); break;
        case 8: _t->changePencilMode(); break;
        case 9: _t->openPreviousImage(); break;
        case 10: _t->openNextImage(); break;
        case 11: _t->undoDraw(); break;
        case 12: _t->clearDraw(); break;
        case 13: _t->openImage(); break;
        case 14: _t->saveAsImage(); break;
        case 15: _t->saveCurrentSelection(); break;
        case 16: _t->closeApplication(); break;
        case 17: _t->changeZoom((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 18: _t->changeDynamic((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->rubberBandSelection((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Frame::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Frame::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Frame,
      qt_meta_data_Frame, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Frame::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Frame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Frame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Frame))
        return static_cast<void*>(const_cast< Frame*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Frame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
