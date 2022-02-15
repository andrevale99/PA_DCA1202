/****************************************************************************
** Meta object code from reading C++ file 'mensageiro2_cliente_thread.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Mensageiro/mensageiro2_cliente_thread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mensageiro2_cliente_thread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Mensageiro2_Cliente_Thread_t {
    const uint offsetsAndSize[16];
    char stringdata0[141];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Mensageiro2_Cliente_Thread_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Mensageiro2_Cliente_Thread_t qt_meta_stringdata_Mensageiro2_Cliente_Thread = {
    {
QT_MOC_LITERAL(0, 26), // "Mensageiro2_Cliente_Thread"
QT_MOC_LITERAL(27, 24), // "signExibirThreadIniciada"
QT_MOC_LITERAL(52, 0), // ""
QT_MOC_LITERAL(53, 21), // "signExibirMsgRecebida"
QT_MOC_LITERAL(75, 14), // "signExibirErro"
QT_MOC_LITERAL(90, 25), // "signExibirThreadEncerrada"
QT_MOC_LITERAL(116, 11), // "slotIniciar"
QT_MOC_LITERAL(128, 12) // "slotEncerrar"

    },
    "Mensageiro2_Cliente_Thread\0"
    "signExibirThreadIniciada\0\0"
    "signExibirMsgRecebida\0signExibirErro\0"
    "signExibirThreadEncerrada\0slotIniciar\0"
    "slotEncerrar"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Mensageiro2_Cliente_Thread[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x06,    1 /* Public */,
       3,    2,   51,    2, 0x06,    2 /* Public */,
       4,    2,   56,    2, 0x06,    5 /* Public */,
       5,    0,   61,    2, 0x06,    8 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,   62,    2, 0x0a,    9 /* Public */,
       7,    0,   63,    2, 0x0a,   10 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void,

       0        // eod
};

void Mensageiro2_Cliente_Thread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Mensageiro2_Cliente_Thread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signExibirThreadIniciada(); break;
        case 1: _t->signExibirMsgRecebida((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->signExibirErro((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->signExibirThreadEncerrada(); break;
        case 4: { bool _r = _t->slotIniciar();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->slotEncerrar(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Mensageiro2_Cliente_Thread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Mensageiro2_Cliente_Thread::signExibirThreadIniciada)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Mensageiro2_Cliente_Thread::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Mensageiro2_Cliente_Thread::signExibirMsgRecebida)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Mensageiro2_Cliente_Thread::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Mensageiro2_Cliente_Thread::signExibirErro)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Mensageiro2_Cliente_Thread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Mensageiro2_Cliente_Thread::signExibirThreadEncerrada)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject Mensageiro2_Cliente_Thread::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Mensageiro2_Cliente_Thread.offsetsAndSize,
    qt_meta_data_Mensageiro2_Cliente_Thread,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Mensageiro2_Cliente_Thread_t
, QtPrivate::TypeAndForceComplete<Mensageiro2_Cliente_Thread, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Mensageiro2_Cliente_Thread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Mensageiro2_Cliente_Thread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Mensageiro2_Cliente_Thread.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Mensageiro2_Cliente_Thread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Mensageiro2_Cliente_Thread::signExibirThreadIniciada()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Mensageiro2_Cliente_Thread::signExibirMsgRecebida(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Mensageiro2_Cliente_Thread::signExibirErro(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Mensageiro2_Cliente_Thread::signExibirThreadEncerrada()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
