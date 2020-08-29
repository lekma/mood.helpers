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


#include "helpers.h"


/* module init helpers ------------------------------------------------------ */

int
_PyType_ReadyWithBase(PyTypeObject *type, PyTypeObject *base)
{
    type->tp_base = base;
    return PyType_Ready(type);
}


static inline int
_PyModule_AddObject(PyObject *module, const char *name, PyObject *object)
{
    Py_INCREF(object);
    if (PyModule_AddObject(module, name, object)) {
        Py_DECREF(object);
        return -1;
    }
    return 0;
}


int
_PyModule_AddType(PyObject *module, const char *name, PyTypeObject *type)
{
    if (PyType_Ready(type)) {
        return -1;
    }
    return _PyModule_AddObject(module, name, _PyObject_CAST(type));
}


int
_PyModule_AddTypeWithBase(PyObject *module, const char *name,
                          PyTypeObject *type, PyTypeObject *base)
{
    type->tp_base = base;
    return _PyModule_AddType(module, name, type);
}


int
_PyModule_AddNewException(PyObject *module, const char *name,
                          const char *module_name, PyObject *base,
                          PyObject *dict, PyObject **result)
{
    const char *mod_name = NULL;
    char *full_name = NULL;
    PyObject *exception = NULL;
    size_t name_size = 1, full_size = 1; // name_size: dot, full_size: terminator
    int name_res = -1;

    if (!(mod_name = (module_name) ? module_name : PyModule_GetName(module))) {
        return -1;
    }
    name_size += (strlen(mod_name) + strlen(name));
    full_size += name_size;
    if (!(full_name = PyObject_Malloc(full_size))) {
        PyErr_NoMemory();
        return -1;
    }
    if (((name_res = PyOS_snprintf(full_name, full_size,
                                   "%s.%s", mod_name, name)) < 0) ||
        ((size_t)name_res != name_size)) {
        PyObject_Free(full_name);
        if (errno) {
            PyErr_SetFromErrno(PyExc_OSError);
        }
        else {
            PyErr_SetString(PyExc_OSError, "PyOS_snprintf failed");
        }
        return -1;
    }
    exception = PyErr_NewException(full_name, base, dict);
    PyObject_Free(full_name);
    if (!exception || _PyModule_AddObject(module, name, exception)) {
        Py_XDECREF(exception);
        return -1;
    }
    if (result) {
        *result = exception;
    }
    else {
        Py_DECREF(exception);
    }
    return 0;
}


/* module state helpers ----------------------------------------------------- */

void *
_PyModule_GetState(PyObject *module)
{
    void *state = NULL;

    if (!(state = PyModule_GetState(module)) && !PyErr_Occurred()) {
        PyErr_Format(PyExc_SystemError, "no state attached to %R", module);
    }
    return state;
}


void *
_PyModuleDef_GetState(PyModuleDef *def)
{
    PyObject *module = NULL;

    if (!(module = PyState_FindModule(def))) { // borrowed
        PyErr_Format(PyExc_SystemError,
                     "<module '%s'> not found in interpreter state",
                     def->m_name ? def->m_name : "unknown");
        return NULL;
    }
    return _PyModule_GetState(module);
}


/* err helpers -------------------------------------------------------------- */

PyObject *
_PyErr_SetFromErrnoWithFilename(const char *filename)
{
    switch (errno) {
        case ENOMEM:
            return PyErr_NoMemory();
        default:
            return PyErr_SetFromErrnoWithFilename(PyExc_OSError, filename);
    }
}


PyObject *
_PyErr_SetFromErrnoWithFilenameAndChain(const char *filename)
{
    PyObject *exc_type, *exc_value, *exc_traceback;

    PyErr_Fetch(&exc_type, &exc_value, &exc_traceback);
    _PyErr_SetFromErrnoWithFilename(filename);
    _PyErr_ChainExceptions(exc_type, exc_value, exc_traceback);
    return NULL;
}
