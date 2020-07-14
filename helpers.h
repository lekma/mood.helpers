/*
#
# Copyright © 2020 Malek Hadj-Ali
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


#ifndef __Py_MOOD_HELPERS_H__
#define __Py_MOOD_HELPERS_H__


#ifdef __cplusplus
extern "C" {
#endif


#define PY_SSIZE_T_CLEAN
#include "Python.h"


/* misc helpers ------------------------------------------------------------- */

#define __Py_INCREF(op) (_Py_INCREF((op)), (op))


#define _PyTuple_ITEMS(op) (((PyTupleObject *)(op))->ob_item)


/* the std PyObject_HasAttr clears traceback when result is 0 */
int _PyObject_HasAttr(PyObject *obj, PyObject *name);

int __PyObject_HasAttrId(PyObject *obj, _Py_Identifier *id);


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

int _PyModule_AddTypeWithBase(PyObject *module, const char *name,
                              PyTypeObject *type, PyTypeObject *base);

int _PyModule_AddNewException(PyObject *module, const char *name,
                              const char *module_name, PyObject *base,
                              PyObject *dict, PyObject **result);


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

PyObject *__PyObject_GC_New(PyTypeObject *type);

#define __PyObject_Alloc(type, typeobj) ((type *)__PyObject_GC_New((typeobj)))


#ifdef __cplusplus
}
#endif


#endif // !__Py_MOOD_HELPERS_H__
