_Py_INLINE(int)
__PyByteArray_Grow(PyByteArrayObject *self, Py_ssize_t size, const char *bytes)
{
    Py_ssize_t osize, nsize, nalloc, alloc;
    char *tmp = NULL;

    if (size <= 0) {
        PyErr_BadInternalCall();
        return -1;
    }
    if ((nalloc = ((nsize = ((osize = Py_SIZE(self)) + size)) + 1)) < 0) {
        PyErr_NoMemory();
        return -1;
    }
    if (self->ob_alloc < nalloc) {
        alloc = (self->ob_alloc) ? (self->ob_alloc << 1) : _Py_MIN_ALLOC;
        while (alloc < nalloc) {
            alloc <<= 1;
            if (alloc < 0) {
                alloc = nalloc;
                break;
            }
        }
        if (!(tmp = (char *)PyObject_Realloc(self->ob_bytes, alloc))) {
            PyErr_NoMemory();
            return -1;
        }
        self->ob_bytes = self->ob_start = tmp;
        self->ob_alloc = alloc;
    }
    memcpy((self->ob_bytes + osize), bytes, size);
    Py_SIZE(self) = nsize;
    self->ob_bytes[nsize] = '\0';
    return 0;
}


_Py_INLINE(int)
__PyByteArray_Shrink(PyByteArrayObject *self, Py_ssize_t size)
{
    Py_ssize_t nsize = Py_SIZE(self) - size;
    char *buf = PyByteArray_AS_STRING(self);

    if (nsize < 0) {
        PyErr_BadInternalCall();
        return -1;
    }
    if (nsize > 0) {
        memmove(buf, buf + size, nsize);
    }
    // XXX: very bad shortcut ¯\_(ツ)_/¯
    Py_SIZE(self) = nsize;
    PyByteArray_AS_STRING(self)[nsize] = '\0';
    return 0;
}

