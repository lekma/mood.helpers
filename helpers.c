#include "helpers.h"


/* misc helpers ------------------------------------------------------------- */

int
_Py_READONLY_ATTRIBUTE(PyObject *self, PyObject *value, void *closure)
{
    _Py_PROTECTED_ATTRIBUTE(value, -1);
    PyErr_SetString(PyExc_AttributeError, "readonly attribute");
    return -1;
}


/* module init helpers ------------------------------------------------------ */

int
_PyType_ReadyWithBase(PyTypeObject *type, PyTypeObject *base)
{
    type->tp_base = base;
    return PyType_Ready(type);
}


int
_PyModule_AddTypeWithBase(
    PyObject *module, PyTypeObject *type, PyTypeObject *base
)
{
    type->tp_base = base;
    return PyModule_AddType(module, type);
}


int
_PyModule_AddNewException(
    PyObject *module,
    const char *name,
    const char *module_name,
    PyObject *base,
    PyObject *dict,
    PyObject **result
)
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
    if (
        ((name_res = PyOS_snprintf(full_name, full_size, "%s.%s", mod_name, name)) < 0) ||
        ((size_t)name_res != name_size)
    ) {
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
    if (!exception || PyModule_AddObjectRef(module, name, exception)) {
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


int
_PyModule_AddTypeFromSpec(
    PyObject *module, PyType_Spec *spec, PyObject *bases, PyObject **result
)
{
    PyObject *type = PyType_FromModuleAndSpec(module, spec, bases);

    if (!type || PyModule_AddType(module, (PyTypeObject*)type)) {
        Py_XDECREF(type);
        return -1;
    }
    if (result) {
        *result = type;
    }
    else {
        Py_DECREF(type);
    }
    return 0;
}


/* module state helpers ----------------------------------------------------- */

void *
__PyModule_GetState__(PyObject *module)
{
    void *state = NULL;

    if (!(state = PyModule_GetState(module)) && !PyErr_Occurred()) {
        PyErr_Format(PyExc_SystemError, "no state attached to %R", module);
    }
    return state;
}


void *
__PyModuleDef_GetState__(PyModuleDef *def)
{
    PyObject *module = NULL;

    if (!(module = PyState_FindModule(def))) { // borrowed
        if (!PyErr_Occurred()) {
            PyErr_Format(
                PyExc_SystemError,
                "<module '%s'> not found in interpreter state",
                def->m_name ? def->m_name : "unknown"
            );
        }
        return NULL;
    }
    return __PyModule_GetState__(module);
}


void *
__PyObject_GetState__(PyObject *self)
{
    PyObject *module = NULL;

    if (!(module = PyType_GetModule(Py_TYPE(self)))) {
        return NULL;
    }
    return __PyModule_GetState__(module);
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


/* alloc helpers ------------------------------------------------------------ */

PyObject *
_PyObject_GC_NEW(PyTypeObject *type)
{
    PyObject *obj = NULL;

    if ((obj = _PyObject_GC_Calloc(_PyObject_SIZE(type)))) {
        obj = PyObject_INIT(obj, type);
    }
    return obj;
}
