/****************************************************************************
** Meta object code from reading C++ file 'download_words.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../download_words.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'download_words.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DownloadWordList_t {
    QByteArrayData data[6];
    char stringdata0[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DownloadWordList_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DownloadWordList_t qt_meta_stringdata_DownloadWordList = {
    {
QT_MOC_LITERAL(0, 0, 16), // "DownloadWordList"
QT_MOC_LITERAL(1, 17, 12), // "kr_def_found"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 5), // "index"
QT_MOC_LITERAL(4, 37, 12), // "en_def_found"
QT_MOC_LITERAL(5, 50, 9) // "def_found"

    },
    "DownloadWordList\0kr_def_found\0\0index\0"
    "en_def_found\0def_found"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DownloadWordList[] = {

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
       4,    1,   32,    2, 0x06 /* Public */,
       5,    1,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void DownloadWordList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DownloadWordList *_t = static_cast<DownloadWordList *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->kr_def_found((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->en_def_found((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->def_found((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DownloadWordList::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DownloadWordList::kr_def_found)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DownloadWordList::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DownloadWordList::en_def_found)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (DownloadWordList::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DownloadWordList::def_found)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject DownloadWordList::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DownloadWordList.data,
      qt_meta_data_DownloadWordList,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DownloadWordList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DownloadWordList::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DownloadWordList.stringdata0))
        return static_cast<void*>(const_cast< DownloadWordList*>(this));
    return QObject::qt_metacast(_clname);
}

int DownloadWordList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void DownloadWordList::kr_def_found(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DownloadWordList::en_def_found(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DownloadWordList::def_found(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
