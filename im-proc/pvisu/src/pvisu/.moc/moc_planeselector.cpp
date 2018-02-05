/****************************************************************************
** Meta object code from reading C++ file 'planeselector.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../planeselector.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'planeselector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PlaneSelector[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      34,   26,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PlaneSelector[] = {
    "PlaneSelector\0\0addFocus()\0nbPlane\0"
    "updateNbPlane(long)\0"
};

void PlaneSelector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PlaneSelector *_t = static_cast<PlaneSelector *>(_o);
        switch (_id) {
        case 0: _t->addFocus(); break;
        case 1: _t->updateNbPlane((*reinterpret_cast< long(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PlaneSelector::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PlaneSelector::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PlaneSelector,
      qt_meta_data_PlaneSelector, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PlaneSelector::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PlaneSelector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PlaneSelector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PlaneSelector))
        return static_cast<void*>(const_cast< PlaneSelector*>(this));
    if (!strcmp(_clname, "Controller"))
        return static_cast< Controller*>(const_cast< PlaneSelector*>(this));
    return QWidget::qt_metacast(_clname);
}

int PlaneSelector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
