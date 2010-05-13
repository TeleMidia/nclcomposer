/****************************************************************************
** Meta object code from reading C++ file 'CoreManager.h'
**
** Created: Thu May 6 17:29:55 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/manager/CoreManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CoreManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CoreManager[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   13,   12,   12, 0x05,
      47,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      64,   13,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CoreManager[] = {
    "CoreManager\0\0,\0projectCreated(QString,QString)\0"
    "onError(QString)\0createProject(QString,QString)\0"
};

const QMetaObject CoreManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CoreManager,
      qt_meta_data_CoreManager, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CoreManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CoreManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CoreManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CoreManager))
        return static_cast<void*>(const_cast< CoreManager*>(this));
    return QObject::qt_metacast(_clname);
}

int CoreManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: projectCreated((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: onError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: createProject((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CoreManager::projectCreated(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CoreManager::onError(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
