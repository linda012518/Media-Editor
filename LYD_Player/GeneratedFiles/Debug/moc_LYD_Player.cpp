/****************************************************************************
** Meta object code from reading C++ file 'LYD_Player.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../LYD_Player.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LYD_Player.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LYD_Player_t {
    QByteArrayData data[13];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LYD_Player_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LYD_Player_t qt_meta_stringdata_LYD_Player = {
    {
QT_MOC_LITERAL(0, 0, 10), // "LYD_Player"
QT_MOC_LITERAL(1, 11, 8), // "setPause"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 11), // "sliderPress"
QT_MOC_LITERAL(4, 33, 13), // "sliderRelease"
QT_MOC_LITERAL(5, 47, 12), // "volumeChange"
QT_MOC_LITERAL(6, 60, 5), // "value"
QT_MOC_LITERAL(7, 66, 8), // "setTempo"
QT_MOC_LITERAL(8, 75, 5), // "speed"
QT_MOC_LITERAL(9, 81, 7), // "setRate"
QT_MOC_LITERAL(10, 89, 8), // "setPitch"
QT_MOC_LITERAL(11, 98, 9), // "openVideo"
QT_MOC_LITERAL(12, 108, 11) // "exportVideo"

    },
    "LYD_Player\0setPause\0\0sliderPress\0"
    "sliderRelease\0volumeChange\0value\0"
    "setTempo\0speed\0setRate\0setPitch\0"
    "openVideo\0exportVideo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LYD_Player[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x0a /* Public */,
       3,    0,   60,    2, 0x0a /* Public */,
       4,    0,   61,    2, 0x0a /* Public */,
       5,    1,   62,    2, 0x0a /* Public */,
       7,    1,   65,    2, 0x0a /* Public */,
       9,    1,   68,    2, 0x0a /* Public */,
      10,    1,   71,    2, 0x0a /* Public */,
      11,    0,   74,    2, 0x0a /* Public */,
      12,    0,   75,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Float,    8,
    QMetaType::Void, QMetaType::Float,    8,
    QMetaType::Void, QMetaType::Float,    8,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void LYD_Player::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LYD_Player *_t = static_cast<LYD_Player *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setPause(); break;
        case 1: _t->sliderPress(); break;
        case 2: _t->sliderRelease(); break;
        case 3: _t->volumeChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setTempo((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 5: _t->setRate((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 6: _t->setPitch((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 7: _t->openVideo(); break;
        case 8: _t->exportVideo(); break;
        default: ;
        }
    }
}

const QMetaObject LYD_Player::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_LYD_Player.data,
      qt_meta_data_LYD_Player,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LYD_Player::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LYD_Player::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LYD_Player.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int LYD_Player::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
