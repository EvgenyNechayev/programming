/****************************************************************************
** Meta object code from reading C++ file 'lbl_scheme.h'
**
** Created: Sat 3. Dec 21:53:08 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../lbl_scheme.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lbl_scheme.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_lblScheme[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_lblScheme[] = {
    "lblScheme\0\0closeScheme()\0"
};

const QMetaObject lblScheme::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_lblScheme,
      qt_meta_data_lblScheme, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &lblScheme::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *lblScheme::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *lblScheme::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_lblScheme))
        return static_cast<void*>(const_cast< lblScheme*>(this));
    return QLabel::qt_metacast(_clname);
}

int lblScheme::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: closeScheme(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void lblScheme::closeScheme()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
