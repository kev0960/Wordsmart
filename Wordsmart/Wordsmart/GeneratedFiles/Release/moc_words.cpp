/****************************************************************************
** Meta object code from reading C++ file 'words.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../words.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'words.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_WordInfo_t {
    QByteArrayData data[7];
    char stringdata0[58];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WordInfo_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WordInfo_t qt_meta_stringdata_WordInfo = {
    {
QT_MOC_LITERAL(0, 0, 8), // "WordInfo"
QT_MOC_LITERAL(1, 9, 9), // "def_found"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 6), // "string"
QT_MOC_LITERAL(4, 27, 4), // "word"
QT_MOC_LITERAL(5, 32, 12), // "kr_def_found"
QT_MOC_LITERAL(6, 45, 12) // "en_def_found"

    },
    "WordInfo\0def_found\0\0string\0word\0"
    "kr_def_found\0en_def_found"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WordInfo[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       5,    0,   32,    2, 0x06 /* Public */,
       6,    0,   33,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void WordInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WordInfo *_t = static_cast<WordInfo *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->def_found((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 1: _t->kr_def_found(); break;
        case 2: _t->en_def_found(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (WordInfo::*_t)(string );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WordInfo::def_found)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (WordInfo::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WordInfo::kr_def_found)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (WordInfo::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WordInfo::en_def_found)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject WordInfo::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WordInfo.data,
      qt_meta_data_WordInfo,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *WordInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WordInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_WordInfo.stringdata0))
        return static_cast<void*>(const_cast< WordInfo*>(this));
    return QObject::qt_metacast(_clname);
}

int WordInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void WordInfo::def_found(string _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WordInfo::kr_def_found()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void WordInfo::en_def_found()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
struct qt_meta_stringdata_Words_t {
    QByteArrayData data[5];
    char stringdata0[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Words_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Words_t qt_meta_stringdata_Words = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Words"
QT_MOC_LITERAL(1, 6, 8), // "defFound"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 8), // "WordInfo"
QT_MOC_LITERAL(4, 25, 4) // "word"

    },
    "Words\0defFound\0\0WordInfo\0word"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Words[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void Words::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Words *_t = static_cast<Words *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->defFound((*reinterpret_cast< const WordInfo(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Words::*_t)(const WordInfo & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Words::defFound)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Words::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Words.data,
      qt_meta_data_Words,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Words::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Words::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Words.stringdata0))
        return static_cast<void*>(const_cast< Words*>(this));
    return QObject::qt_metacast(_clname);
}

int Words::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void Words::defFound(const WordInfo & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
