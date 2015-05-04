/****************************************************************************
** Meta object code from reading C++ file 'my_glwidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../hellogl/my_glwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'my_glwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_My_GLWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   13,   12,   12, 0x05,
      41,   13,   12,   12, 0x05,
      63,   13,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      85,   13,   12,   12, 0x0a,
     103,   13,   12,   12, 0x0a,
     121,   13,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_My_GLWidget[] = {
    "My_GLWidget\0\0angle\0xRotationChanged(int)\0"
    "yRotationChanged(int)\0zRotationChanged(int)\0"
    "setXRotation(int)\0setYRotation(int)\0"
    "setZRotation(int)\0"
};

void My_GLWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        My_GLWidget *_t = static_cast<My_GLWidget *>(_o);
        switch (_id) {
        case 0: _t->xRotationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->yRotationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->zRotationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setXRotation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setYRotation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setZRotation((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData My_GLWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject My_GLWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_My_GLWidget,
      qt_meta_data_My_GLWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &My_GLWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *My_GLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *My_GLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_My_GLWidget))
        return static_cast<void*>(const_cast< My_GLWidget*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int My_GLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void My_GLWidget::xRotationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void My_GLWidget::yRotationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void My_GLWidget::zRotationChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
