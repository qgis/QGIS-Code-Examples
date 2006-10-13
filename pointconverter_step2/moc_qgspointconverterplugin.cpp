/****************************************************************************
** Meta object code from reading C++ file 'qgspointconverterplugin.h'
**
** Created: Don Sep 7 10:11:34 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qgspointconverterplugin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qgspointconverterplugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QgsPointConverterPlugin[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QgsPointConverterPlugin[] = {
    "QgsPointConverterPlugin\0\0convertToPoint()\0"
};

const QMetaObject QgsPointConverterPlugin::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QgsPointConverterPlugin,
      qt_meta_data_QgsPointConverterPlugin, 0 }
};

const QMetaObject *QgsPointConverterPlugin::metaObject() const
{
    return &staticMetaObject;
}

void *QgsPointConverterPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QgsPointConverterPlugin))
	return static_cast<void*>(const_cast<QgsPointConverterPlugin*>(this));
    if (!strcmp(_clname, "QgisPlugin"))
	return static_cast<QgisPlugin*>(const_cast<QgsPointConverterPlugin*>(this));
    return QObject::qt_metacast(_clname);
}

int QgsPointConverterPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: convertToPoint(); break;
        }
        _id -= 1;
    }
    return _id;
}
