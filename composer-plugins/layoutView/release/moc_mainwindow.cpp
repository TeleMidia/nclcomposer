/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Mon May 3 18:40:43 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      21,   11,   11,   11, 0x0a,
      36,   29,   11,   11, 0x0a,
      59,   29,   11,   11, 0x0a,
      87,   11,   11,   11, 0x0a,
     102,   29,   11,   11, 0x0a,
     131,  129,   11,   11, 0x0a,
     165,  129,   11,   11, 0x0a,
     200,  129,   11,   11, 0x0a,
     232,  129,   11,   11, 0x0a,
     265,  129,   11,   11, 0x0a,
     301,  129,   11,   11, 0x0a,
     336,  129,   11,   11, 0x0a,
     372,  129,   11,   11, 0x0a,
     408,  129,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0accept()\0clear()\0region\0"
    "fillFields(NCLRegion*)\0"
    "addNodeTreeView(NCLRegion*)\0selectRegion()\0"
    "performChanges(NCLRegion*)\0,\0"
    "changeLeftRegion(QString,QString)\0"
    "changeRightRegion(QString,QString)\0"
    "changeIdRegion(QString,QString)\0"
    "changeTopRegion(QString,QString)\0"
    "changeBottomRegion(QString,QString)\0"
    "changeWidthRegion(QString,QString)\0"
    "changeHeightRegion(QString,QString)\0"
    "changeZIndexRegion(QString,QString)\0"
    "changeTitleRegion(QString,QString)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

const QMetaObject *MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: accept(); break;
        case 1: clear(); break;
        case 2: fillFields((*reinterpret_cast< NCLRegion*(*)>(_a[1]))); break;
        case 3: addNodeTreeView((*reinterpret_cast< NCLRegion*(*)>(_a[1]))); break;
        case 4: selectRegion(); break;
        case 5: performChanges((*reinterpret_cast< NCLRegion*(*)>(_a[1]))); break;
        case 6: changeLeftRegion((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: changeRightRegion((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 8: changeIdRegion((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: changeTopRegion((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 10: changeBottomRegion((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 11: changeWidthRegion((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 12: changeHeightRegion((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 13: changeZIndexRegion((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 14: changeTitleRegion((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
