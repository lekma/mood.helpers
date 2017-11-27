/*
#
# Copyright © 2017 Malek Hadj-Ali
# All rights reserved.
#
# This file is part of mood.
#
# mood is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 3
# as published by the Free Software Foundation.
#
# mood is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with mood.  If not, see <http://www.gnu.org/licenses/>.
#
*/


#ifndef _Py_HELPERS_H
#define _Py_HELPERS_H

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _PY_INLINE_HELPERS
#define _Py_INLINE(type) static inline type
#else
#define _Py_INLINE(type) type
#endif


/* module init helpers */

int
_PyType_ReadyWithBase(PyTypeObject *type, PyTypeObject *base);

int
_PyModule_AddType(PyObject *module, const char *name, PyTypeObject *type);

int
_PyModule_AddTypeWithBase(PyObject *module, const char *name,
                          PyTypeObject *type, PyTypeObject *base);

int
_PyModule_AddNewException(PyObject *module, const char *name,
                          const char *module_name, PyObject *base,
                          PyObject *dict, PyObject **result);

#define _PyModule_AddIntConstant(m, n, v) \
    PyModule_AddIntConstant((m), (n), (int)(v))

#define _PyModule_AddUnsignedIntConstant(m, n, v) \
    PyModule_AddIntConstant((m), (n), (unsigned int)(v))

#define _PyModule_AddIntMacro(m, c) \
    _PyModule_AddIntConstant((m), #c, (c))

#define _PyModule_AddUnsignedIntMacro(m, c) \
    _PyModule_AddUnsignedIntConstant((m), #c, (c))


/* module state helpers */

void *
_PyModule_GetState(PyObject *module);

void *
_PyModuleDef_GetState(PyModuleDef *def);


/* err helpers */

PyObject *
_PyErr_SetFromErrnoWithFilename(const char *filename);

PyObject *
_PyErr_SetFromErrnoWithFilenameAndChain(const char *filename);

#define _PyErr_SetFromErrno() \
    _PyErr_SetFromErrnoWithFilename(NULL)

#define _PyErr_SetFromErrnoAndChain() \
    _PyErr_SetFromErrnoWithFilenameAndChain(NULL)


/* misc helpers */

#define _Py_SET_MEMBER(m, op) \
    do { \
        PyObject *_py_tmp = (PyObject *)(m); \
        Py_INCREF((op)); \
        (m) = (op); \
        Py_XDECREF(_py_tmp); \
    } while (0)

#define _Py_RETURN_OBJECT(op) \
    do { \
        PyObject *_py_res = (PyObject *)(op); \
        return Py_INCREF(_py_res), _py_res; \
    } while (0)

#define _PyDict_GET_SIZE(op) (((PyDictObject *)(op))->ma_used)

#define _PyTuple_ITEMS(op) (((PyTupleObject *)(op))->ob_item)


/* alloc helpers */

PyObject *
__PyObject_GC_New(PyTypeObject *type);

#define __PyObject_Alloc(type, typeobj) ((type *)__PyObject_GC_New((typeobj)))


/* capsule helpers */

void *
_PyCapsule_Import(const char *name);


/* bytearray helpers */

#ifndef _Py_MIN_ALLOC
#define _Py_MIN_ALLOC 8
#endif

#ifdef _PY_INLINE_HELPERS
#include "bytearray.inl"
#else
int
__PyByteArray_Grow(PyByteArrayObject *self, Py_ssize_t size, const char *bytes,
                   Py_ssize_t initsize);

int
__PyByteArray_Shrink(PyByteArrayObject *self, Py_ssize_t size);
#endif


#ifdef __cplusplus
}
#endif

#endif /* !_Py_HELPERS_H */
