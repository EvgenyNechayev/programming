/****************************************************************************
** Meta object code from reading C++ file 'tsmfb.h'
**
** Created: Fri 2. Dec 01:44:00 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../tsmfb.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tsmfb.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_tsmfb[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x0a,
      27,    6,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_tsmfb[] = {
    "tsmfb\0\0slotDecButClicked()\0"
    "slotRecButClicked()\0"
};

const QMetaObject tsmfb::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_tsmfb,
      qt_meta_data_tsmfb, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &tsmfb::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *tsmfb::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *tsmfb::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_tsmfb))
        return static_cast<void*>(const_cast< tsmfb*>(this));
    return QWidget::qt_metacast(_clname);
}

int tsmfb::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slotDecButClicked(); break;
        case 1: slotRecButClicked(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
