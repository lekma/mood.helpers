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


#ifndef _Py_MSGPACK_CAPI_H
#define _Py_MSGPACK_CAPI_H

#ifdef __cplusplus
extern "C" {
#endif


/* these must be kept in sync with their msgpack equivalent */

#define MSGPACK_INVALID 0xc1        // _MSGPACK_INVALID
#define MSGPACK_LEN8    0xcc        // _MSGPACK_UINT8
#define MSGPACK_LEN16   0xcd        // _MSGPACK_UINT16
#define MSGPACK_LEN32   0xce        // _MSGPACK_UINT32
#define MSGPACK_LEN_MAX (1LL << 32) // _MSGPACK_UINT32_MAX


/* CAPI */

#define _MSGPACK_MODULE_NAME  "mood.msgpack"
#define _MSGPACK_CAPI_NAME    "CAPI"
#define _MSGPACK_CAPSULE_NAME _MSGPACK_MODULE_NAME "." _MSGPACK_CAPI_NAME

typedef struct {
    int (*pack_obj)(PyObject *, PyObject *);
    int (*pack_len)(PyObject *, Py_ssize_t, uint8_t *);
    Py_ssize_t (*type_size)(uint8_t);
} _Py_msgpack_CAPI;


#ifdef __cplusplus
}
#endif

#endif /* !_Py_MSGPACK_CAPI_H */
