/****************************************************************************
** Meta object code from reading C++ file 'mensageiro2_cliente_main.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Mensageiro/mensageiro2_cliente_main.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mensageiro2_cliente_main.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Mensageiro2_Cliente_Main_t {
    const uint offsetsAndSize[38];
    char stringdata0[347];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Mensageiro2_Cliente_Main_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Mensageiro2_Cliente_Main_t qt_meta_stringdata_Mensageiro2_Cliente_Main = {
    {
QT_MOC_LITERAL(0, 24), // "Mensageiro2_Cliente_Main"
QT_MOC_LITERAL(25, 18), // "signExecutarThread"
QT_MOC_LITERAL(44, 0), // ""
QT_MOC_LITERAL(45, 18), // "signEncerrarThread"
QT_MOC_LITERAL(64, 20), // "slotConectarServidor"
QT_MOC_LITERAL(85, 6), // "server"
QT_MOC_LITERAL(92, 7), // "usuario"
QT_MOC_LITERAL(100, 28), // "slotExibirInterfaceConectada"
QT_MOC_LITERAL(129, 31), // "slotExibirInterfaceDesconectada"
QT_MOC_LITERAL(161, 21), // "slotExibirMsgRecebida"
QT_MOC_LITERAL(183, 5), // "remet"
QT_MOC_LITERAL(189, 3), // "msg"
QT_MOC_LITERAL(193, 14), // "slotExibirErro"
QT_MOC_LITERAL(208, 5), // "title"
QT_MOC_LITERAL(214, 23), // "on_actionSair_triggered"
QT_MOC_LITERAL(238, 27), // "on_actionConectar_triggered"
QT_MOC_LITERAL(266, 25), // "on_actionApagar_triggered"
QT_MOC_LITERAL(292, 30), // "on_actionDesconectar_triggered"
QT_MOC_LITERAL(323, 23) // "on_enviarButton_clicked"

    },
    "Mensageiro2_Cliente_Main\0signExecutarThread\0"
    "\0signEncerrarThread\0slotConectarServidor\0"
    "server\0usuario\0slotExibirInterfaceConectada\0"
    "slotExibirInterfaceDesconectada\0"
    "slotExibirMsgRecebida\0remet\0msg\0"
    "slotExibirErro\0title\0on_actionSair_triggered\0"
    "on_actionConectar_triggered\0"
    "on_actionApagar_triggered\0"
    "on_actionDesconectar_triggered\0"
    "on_enviarButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Mensageiro2_Cliente_Main[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   86,    2, 0x06,    1 /* Public */,
       3,    0,   87,    2, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    2,   88,    2, 0x0a,    3 /* Public */,
       7,    0,   93,    2, 0x0a,    6 /* Public */,
       8,    0,   94,    2, 0x0a,    7 /* Public */,
       9,    2,   95,    2, 0x0a,    8 /* Public */,
      12,    2,  100,    2, 0x0a,   11 /* Public */,
      14,    0,  105,    2, 0x08,   14 /* Private */,
      15,    0,  106,    2, 0x08,   15 /* Private */,
      16,    0,  107,    2, 0x08,   16 /* Private */,
      17,    0,  108,    2, 0x08,   17 /* Private */,
      18,    0,  109,    2, 0x08,   18 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   10,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   13,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Mensageiro2_Cliente_Main::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Mensageiro2_Cliente_Main *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signExecutarThread(); break;
        case 1: _t->signEncerrarThread(); break;
        case 2: _t->slotConectarServidor((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->slotExibirInterfaceConectada(); break;
        case 4: _t->slotExibirInterfaceDesconectada(); break;
        case 5: _t->slotExibirMsgRecebida((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->slotExibirErro((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: _t->on_actionSair_triggered(); break;
        case 8: _t->on_actionConectar_triggered(); break;
        case 9: _t->on_actionApagar_triggered(); break;
        case 10: _t->on_actionDesconectar_triggered(); break;
        case 11: _t->on_enviarButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Mensageiro2_Cliente_Main::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Mensageiro2_Cliente_Main::signExecutarThread)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Mensageiro2_Cliente_Main::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Mensageiro2_Cliente_Main::signEncerrarThread)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject Mensageiro2_Cliente_Main::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_Mensageiro2_Cliente_Main.offsetsAndSize,
    qt_meta_data_Mensageiro2_Cliente_Main,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Mensageiro2_Cliente_Main_t
, QtPrivate::TypeAndForceComplete<Mensageiro2_Cliente_Main, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Mensageiro2_Cliente_Main::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Mensageiro2_Cliente_Main::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Mensageiro2_Cliente_Main.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Mensageiro2_Cliente_Main::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Mensageiro2_Cliente_Main::signExecutarThread()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Mensageiro2_Cliente_Main::signEncerrarThread()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
