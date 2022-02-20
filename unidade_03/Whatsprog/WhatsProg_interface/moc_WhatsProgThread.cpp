/****************************************************************************
** Meta object code from reading C++ file 'WhatsProgThread.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "WhatsProgThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WhatsProgThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WhatsProgThread_t {
    const uint offsetsAndSize[20];
    char stringdata0[114];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_WhatsProgThread_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_WhatsProgThread_t qt_meta_stringdata_WhatsProgThread = {
    {
QT_MOC_LITERAL(0, 15), // "WhatsProgThread"
QT_MOC_LITERAL(16, 8), // "nova_msg"
QT_MOC_LITERAL(25, 0), // ""
QT_MOC_LITERAL(26, 12), // "IterConversa"
QT_MOC_LITERAL(39, 3), // "itr"
QT_MOC_LITERAL(43, 12), // "conf_leitura"
QT_MOC_LITERAL(56, 12), // "matou_thread"
QT_MOC_LITERAL(69, 12), // "desconectado"
QT_MOC_LITERAL(82, 18), // "conf_lancar_thread"
QT_MOC_LITERAL(101, 12) // "matar_thread"

    },
    "WhatsProgThread\0nova_msg\0\0IterConversa\0"
    "itr\0conf_leitura\0matou_thread\0"
    "desconectado\0conf_lancar_thread\0"
    "matar_thread"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WhatsProgThread[] = {

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
       1,    1,   50,    2, 0x06,    1 /* Public */,
       5,    1,   53,    2, 0x06,    3 /* Public */,
       6,    0,   56,    2, 0x06,    5 /* Public */,
       7,    0,   57,    2, 0x06,    6 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   58,    2, 0x08,    7 /* Private */,
       9,    0,   59,    2, 0x08,    8 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void WhatsProgThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WhatsProgThread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->nova_msg((*reinterpret_cast< IterConversa(*)>(_a[1]))); break;
        case 1: _t->conf_leitura((*reinterpret_cast< IterConversa(*)>(_a[1]))); break;
        case 2: _t->matou_thread(); break;
        case 3: _t->desconectado(); break;
        case 4: _t->conf_lancar_thread(); break;
        case 5: _t->matar_thread(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WhatsProgThread::*)(IterConversa );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WhatsProgThread::nova_msg)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WhatsProgThread::*)(IterConversa );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WhatsProgThread::conf_leitura)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (WhatsProgThread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WhatsProgThread::matou_thread)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (WhatsProgThread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WhatsProgThread::desconectado)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject WhatsProgThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_WhatsProgThread.offsetsAndSize,
    qt_meta_data_WhatsProgThread,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_WhatsProgThread_t
, QtPrivate::TypeAndForceComplete<WhatsProgThread, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<IterConversa, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<IterConversa, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *WhatsProgThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WhatsProgThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WhatsProgThread.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WhatsProgThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void WhatsProgThread::nova_msg(IterConversa _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WhatsProgThread::conf_leitura(IterConversa _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void WhatsProgThread::matou_thread()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void WhatsProgThread::desconectado()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
