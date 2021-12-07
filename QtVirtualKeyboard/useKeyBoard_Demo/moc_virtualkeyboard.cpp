/****************************************************************************
** Meta object code from reading C++ file 'virtualkeyboard.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "virtualkeyboard_hasQSQLITE/virtualkeyboard.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'virtualkeyboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VirtualKeyBoard_t {
    QByteArrayData data[18];
    char stringdata0[208];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VirtualKeyBoard_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VirtualKeyBoard_t qt_meta_stringdata_VirtualKeyBoard = {
    {
QT_MOC_LITERAL(0, 0, 15), // "VirtualKeyBoard"
QT_MOC_LITERAL(1, 16, 24), // "changedOutSideTextSignal"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 4), // "text"
QT_MOC_LITERAL(4, 47, 7), // "objName"
QT_MOC_LITERAL(5, 55, 21), // "setDefaultColorSignal"
QT_MOC_LITERAL(6, 77, 11), // "showChinese"
QT_MOC_LITERAL(7, 89, 3), // "key"
QT_MOC_LITERAL(8, 93, 21), // "QMap<int,QStringList>"
QT_MOC_LITERAL(9, 115, 16), // "chinese_list_map"
QT_MOC_LITERAL(10, 132, 17), // "recKeyClickedSlot"
QT_MOC_LITERAL(11, 150, 13), // "selectChinese"
QT_MOC_LITERAL(12, 164, 16), // "focusChangedSlot"
QT_MOC_LITERAL(13, 181, 8), // "QWidget*"
QT_MOC_LITERAL(14, 190, 3), // "old"
QT_MOC_LITERAL(15, 194, 3), // "now"
QT_MOC_LITERAL(16, 198, 4), // "show"
QT_MOC_LITERAL(17, 203, 4) // "hide"

    },
    "VirtualKeyBoard\0changedOutSideTextSignal\0"
    "\0text\0objName\0setDefaultColorSignal\0"
    "showChinese\0key\0QMap<int,QStringList>\0"
    "chinese_list_map\0recKeyClickedSlot\0"
    "selectChinese\0focusChangedSlot\0QWidget*\0"
    "old\0now\0show\0hide"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VirtualKeyBoard[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x06 /* Public */,
       5,    0,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   60,    2, 0x08 /* Private */,
      10,    0,   65,    2, 0x08 /* Private */,
      11,    0,   66,    2, 0x08 /* Private */,
      12,    2,   67,    2, 0x08 /* Private */,
      16,    0,   72,    2, 0x0a /* Public */,
      17,    0,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8,    7,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13, 0x80000000 | 13,   14,   15,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VirtualKeyBoard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VirtualKeyBoard *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changedOutSideTextSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->setDefaultColorSignal(); break;
        case 2: _t->showChinese((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QMap<int,QStringList>(*)>(_a[2]))); break;
        case 3: _t->recKeyClickedSlot(); break;
        case 4: _t->selectChinese(); break;
        case 5: _t->focusChangedSlot((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 6: _t->show(); break;
        case 7: _t->hide(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VirtualKeyBoard::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VirtualKeyBoard::changedOutSideTextSignal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VirtualKeyBoard::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VirtualKeyBoard::setDefaultColorSignal)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VirtualKeyBoard::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_VirtualKeyBoard.data,
    qt_meta_data_VirtualKeyBoard,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VirtualKeyBoard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VirtualKeyBoard::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VirtualKeyBoard.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int VirtualKeyBoard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void VirtualKeyBoard::changedOutSideTextSignal(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VirtualKeyBoard::setDefaultColorSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
