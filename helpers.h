#ifndef Py_MOOD_HELPERS_H
#define Py_MOOD_HELPERS_H


#define PY_SSIZE_T_CLEAN
#include "Python.h"


#ifdef __cplusplus
extern "C" {
#endif


/* sys/lib call helpers ----------------------------------------------------- */

#define __sys_debug__(fmt, ...) \
    printf("[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)


#define __sys_wrap__(t, fn, ...) (t)fn(__VA_ARGS__)

#define __sys_gil_wrap__(t, fn, ...) \
    ( \
        { \
            PyThreadState *_save_ = PyEval_SaveThread(); \
            t _res_ = __sys_wrap__(t, fn, __VA_ARGS__); \
            PyEval_RestoreThread(_save_); \
            (_res_); \
        } \
    )


/* pyport.h ----------------------------------------------------------------- */

#if !defined(_Py_CAST)
#define _Py_CAST(type, expr) ((type)expr)
#endif /* _Py_CAST */

#if !defined(_Py_FUNC_CAST)
#define _Py_FUNC_CAST(type, func) _Py_CAST(type, _Py_CAST(void(*)(void), func))
#endif /* _Py_FUNC_CAST */

#if !defined(_PyCFunction_CAST)
#define _PyCFunction_CAST(func) _Py_FUNC_CAST(PyCFunction, func)
#endif /* _PyCFunction_CAST */


/* misc helpers ------------------------------------------------------------- */

#define _PyTuple_ITEMS(op) (((PyTupleObject *)(op))->ob_item)
#define _PyList_ITEMS(op) (((PyListObject *)(op))->ob_item)


#define _Py_PROTECTED_ATTRIBUTE(v, r) \
    do { \
        if ((v) == NULL) { \
            PyErr_SetString(PyExc_AttributeError, "cannot delete attribute"); \
            return (r); \
        } \
    } while (0)

int _Py_READONLY_ATTRIBUTE(PyObject *self, PyObject *value, void *closure);


#define _Py_SET_MEMBER(m, op) \
    do { \
        PyObject *_py_tmp = (PyObject *)(m); \
        Py_INCREF((op)); \
        (m) = (op); \
        Py_XDECREF(_py_tmp); \
    } while (0)


/* module init helpers ------------------------------------------------------ */

#define _PyModule_AddIntConstant(m, n, v) \
    PyModule_AddIntConstant((m), (n), (int)(v))

#define _PyModule_AddUnsignedIntConstant(m, n, v) \
    PyModule_AddIntConstant((m), (n), (unsigned int)(v))

#define _PyModule_AddIntMacro(m, c) \
    _PyModule_AddIntConstant((m), #c, (c))

#define _PyModule_AddUnsignedIntMacro(m, c) \
    _PyModule_AddUnsignedIntConstant((m), #c, (c))


int _PyType_ReadyWithBase(PyTypeObject *type, PyTypeObject *base);

int _PyModule_AddTypeWithBase(
    PyObject *module, PyTypeObject *type, PyTypeObject *base
);

int _PyModule_AddNewException(
    PyObject *module,
    const char *name,
    const char *module_name,
    PyObject *base,
    PyObject *dict,
    PyObject **result
);

int _PyModule_AddTypeFromSpec(
    PyObject *module, PyType_Spec *spec, PyObject *bases, PyObject **result
);


/* module state helpers ----------------------------------------------------- */

void *__PyModule_GetState__(PyObject *module);

void *__PyModuleDef_GetState__(PyModuleDef *def);

void *__PyObject_GetState__(PyObject *self);


/* err helpers -------------------------------------------------------------- */

PyObject *_PyErr_SetFromErrnoWithFilename(const char *filename);

PyObject *_PyErr_SetFromErrnoWithFilenameAndChain(const char *filename);

#define _PyErr_SetFromErrno() \
    _PyErr_SetFromErrnoWithFilename(NULL)

#define _PyErr_SetFromErrnoAndChain() \
    _PyErr_SetFromErrnoWithFilenameAndChain(NULL)


/* alloc helpers ------------------------------------------------------------ */

PyObject *_PyObject_GC_NEW(PyTypeObject *type);

#define PyObject_GC_NEW(TYPE, type) ((TYPE *)_PyObject_GC_NEW(type))


/* unicode helpers ---------------------------------------------------------- */

#define _PyUnicode_DecodeFSDefault(bytes) \
    PyUnicode_DecodeFSDefault(PyBytes_AS_STRING((bytes)))


#ifdef __cplusplus
}
#endif


#endif // !Py_MOOD_HELPERS_H
