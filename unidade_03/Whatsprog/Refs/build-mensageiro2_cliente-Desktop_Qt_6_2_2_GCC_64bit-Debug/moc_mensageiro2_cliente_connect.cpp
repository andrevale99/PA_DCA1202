/****************************************************************************
** Meta object code from reading C++ file 'mensageiro2_cliente_connect.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Mensageiro/mensageiro2_cliente_connect.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mensageiro2_cliente_connect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Mensageiro2_Cliente_Connect_t {
    const uint offsetsAndSize[12];
    char stringdata0[79];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Mensageiro2_Cliente_Connect_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Mensageiro2_Cliente_Connect_t qt_meta_stringdata_Mensageiro2_Cliente_Connect = {
    {
QT_MOC_LITERAL(0, 27), // "Mensageiro2_Cliente_Connect"
QT_MOC_LITERAL(28, 12), // "signConectar"
QT_MOC_LITERAL(41, 0), // ""
QT_MOC_LITERAL(42, 6), // "server"
QT_MOC_LITERAL(49, 7), // "usuario"
QT_MOC_LITERAL(57, 21) // "on_buttonBox_accepted"

    },
    "Mensageiro2_Cliente_Connect\0signConectar\0"
    "\0server\0usuario\0on_buttonBox_accepted"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Mensageiro2_Cliente_Connect[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   31,    2, 0x08,    4 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void Mensageiro2_Cliente_Connect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Mensageiro2_Cliente_Connect *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signConectar((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->on_buttonBox_accepted(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Mensageiro2_Cliente_Connect::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Mensageiro2_Cliente_Connect::signConectar)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Mensageiro2_Cliente_Connect::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_Mensageiro2_Cliente_Connect.offsetsAndSize,
    qt_meta_data_Mensageiro2_Cliente_Connect,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Mensageiro2_Cliente_Connect_t
, QtPrivate::TypeAndForceComplete<Mensageiro2_Cliente_Connect, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Mensageiro2_Cliente_Connect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Mensageiro2_Cliente_Connect::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Mensageiro2_Cliente_Connect.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Mensageiro2_Cliente_Connect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Mensageiro2_Cliente_Connect::signConectar(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
