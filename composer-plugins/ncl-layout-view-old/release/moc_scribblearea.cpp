/****************************************************************************
** Meta object code from reading C++ file 'scribblearea.h'
**
** Created: Mon May 3 18:40:44 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../scribblearea.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scribblearea.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ScribbleArea[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      21,   14,   13,   13, 0x05,
      48,   14,   13,   13, 0x05,
      72,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      98,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ScribbleArea[] = {
    "ScribbleArea\0\0region\0regionSelected(NCLRegion*)\0"
    "regionAdded(NCLRegion*)\0"
    "regionChanged(NCLRegion*)\0clear()\0"
};

const QMetaObject ScribbleArea::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ScribbleArea,
      qt_meta_data_ScribbleArea, 0 }
};

const QMetaObject *ScribbleArea::metaObject() const
{
    return &staticMetaObject;
}

void *ScribbleArea::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ScribbleArea))
        return static_cast<void*>(const_cast< ScribbleArea*>(this));
    return QWidget::qt_metacast(_clname);
}

int ScribbleArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: regionSelected((*reinterpret_cast< NCLRegion*(*)>(_a[1]))); break;
        case 1: regionAdded((*reinterpret_cast< NCLRegion*(*)>(_a[1]))); break;
        case 2: regionChanged((*reinterpret_cast< NCLRegion*(*)>(_a[1]))); break;
        case 3: clear(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ScribbleArea::regionSelected(NCLRegion * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ScribbleArea::regionAdded(NCLRegion * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ScribbleArea::regionChanged(NCLRegion * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
