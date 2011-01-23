/****************************************************************************
** Meta object code from reading C++ file 'attributes.h'
**
** Created: Mon May 3 18:40:45 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../attributes.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'attributes.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Attributes[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      21,   12,   11,   11, 0x05,
      59,   48,   11,   11, 0x05,
      98,   88,   11,   11, 0x05,
     138,  126,   11,   11, 0x05,
     181,  168,   11,   11, 0x05,
     224,  212,   11,   11, 0x05,
     267,  254,   11,   11, 0x05,
     311,  298,   11,   11, 0x05,
     354,  342,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     391,  384,   11,   11, 0x0a,
     414,   11,   11,   11, 0x0a,
     425,   11,   11,   11, 0x0a,
     438,   11,   11,   11, 0x0a,
     450,   11,   11,   11, 0x0a,
     464,   11,   11,   11, 0x0a,
     479,   11,   11,   11, 0x0a,
     493,   11,   11,   11, 0x0a,
     508,   11,   11,   11, 0x0a,
     523,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Attributes[] = {
    "Attributes\0\0id,newId\0IdChanged(QString,QString)\0"
    "id,newLeft\0LeftChanged(QString,QString)\0"
    "id,newTop\0TopChanged(QString,QString)\0"
    "id,newRight\0RightChanged(QString,QString)\0"
    "id,newBottom\0BottomChanged(QString,QString)\0"
    "id,newWidth\0WidthChanged(QString,QString)\0"
    "id,newHeight\0HeightChanged(QString,QString)\0"
    "id,newZIndex\0ZIndexChanged(QString,QString)\0"
    "id,newTitle\0TitleChanged(QString,QString)\0"
    "region\0fillFields(NCLRegion*)\0changeId()\0"
    "changeLeft()\0changeTop()\0changeRight()\0"
    "changeBottom()\0changeWidth()\0"
    "changeHeight()\0changeZIndex()\0"
    "changeTitle()\0"
};

const QMetaObject Attributes::staticMetaObject = {
    { &QGroupBox::staticMetaObject, qt_meta_stringdata_Attributes,
      qt_meta_data_Attributes, 0 }
};

const QMetaObject *Attributes::metaObject() const
{
    return &staticMetaObject;
}

void *Attributes::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Attributes))
        return static_cast<void*>(const_cast< Attributes*>(this));
    return QGroupBox::qt_metacast(_clname);
}

int Attributes::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGroupBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: IdChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: LeftChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: TopChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: RightChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: BottomChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: WidthChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: HeightChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: ZIndexChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 8: TitleChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: fillFields((*reinterpret_cast< NCLRegion*(*)>(_a[1]))); break;
        case 10: changeId(); break;
        case 11: changeLeft(); break;
        case 12: changeTop(); break;
        case 13: changeRight(); break;
        case 14: changeBottom(); break;
        case 15: changeWidth(); break;
        case 16: changeHeight(); break;
        case 17: changeZIndex(); break;
        case 18: changeTitle(); break;
        default: ;
        }
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void Attributes::IdChanged(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Attributes::LeftChanged(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Attributes::TopChanged(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Attributes::RightChanged(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Attributes::BottomChanged(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Attributes::WidthChanged(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Attributes::HeightChanged(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Attributes::ZIndexChanged(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Attributes::TitleChanged(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
