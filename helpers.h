#ifndef Py_MOOD_HELPERS_H
#define Py_MOOD_HELPERS_H


#define PY_SSIZE_T_CLEAN
#include "Python.h"


#ifdef __cplusplus
extern "C" {
#endif


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

int _PyModule_AddType(PyObject *module, const char *name, PyTypeObject *type);

int _PyModule_AddTypeWithBase(
    PyObject *module, const char *name, PyTypeObject *type, PyTypeObject *base
);

int _PyModule_AddNewException(
    PyObject *module, const char *name, const char *module_name, PyObject *base,
    PyObject *dict, PyObject **result
);


/* module state helpers ----------------------------------------------------- */

void *_PyModule_GetState(PyObject *module);

void *_PyModuleDef_GetState(PyModuleDef *def);


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

#define _PyUnicode_DecodeFSDefault(op) \
    PyUnicode_DecodeFSDefault(PyBytes_AS_STRING((op)))


#ifdef __cplusplus
}
#endif


#endif // !Py_MOOD_HELPERS_H
