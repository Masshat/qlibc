/******************************************************************************
 * qLibc
 *
 * Copyright (c) 2010-2015 Seungyoung Kim.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

/**
 * List container with key/value pair in doubly linked data structure.
 *
 * @file qlisttbl.h
 */

#ifndef QLISTTBL_H
#define QLISTTBL_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* types */
typedef struct qlisttbl_s qlisttbl_t;
typedef struct qlisttbl_obj_s qlisttbl_obj_t;
typedef struct qlisttbl_data_s qlisttbl_data_t;

/* public functions */
enum {
    QLISTTBL_THREADSAFE      = (0x01),      /*!< make it thread-safe */
    QLISTTBL_UNIQUE          = (0x01 << 1), /*!< keys are unique */
    QLISTTBL_CASEINSENSITIVE = (0x01 << 2), /*!< keys are case insensitive */
    QLISTTBL_INSERTTOP       = (0x01 << 3), /*!< insert new key at the top */
    QLISTTBL_LOOKUPFORWARD   = (0x01 << 4), /*!< find key from the top (default: backward) */
};

extern qlisttbl_t *qlisttbl(int options);  /*!< qlisttbl constructor */

/**
 *  qlisttbl container structure
 */
struct qlisttbl_s {
    /* capsulated member functions */
    bool (*put) (qlisttbl_t *tbl, const char *name, const void *data, size_t size);
    bool (*putstr) (qlisttbl_t *tbl, const char *name, const char *str);
    bool (*putstrf) (qlisttbl_t *tbl, const char *name, const char *format, ...);
    bool (*putint) (qlisttbl_t *tbl, const char *name, int64_t num);

    void *(*get) (qlisttbl_t *tbl, const char *name, size_t *size, bool newmem);
    char *(*getstr) (qlisttbl_t *tbl, const char *name, bool newmem);
    int64_t (*getint) (qlisttbl_t *tbl, const char *name);

    qlisttbl_data_t *(*getmulti) (qlisttbl_t *tbl, const char *name, bool newmem, size_t *numobjs);
    void (*freemulti) (qlisttbl_data_t *objs);

    bool (*getnext) (qlisttbl_t *tbl, qlisttbl_obj_t *obj, const char *name, bool newmem);

    size_t (*remove) (qlisttbl_t *tbl, const char *name);
    bool (*removeobj) (qlisttbl_t *tbl, const qlisttbl_obj_t *obj);

    size_t (*size) (qlisttbl_t *tbl);
    void (*sort) (qlisttbl_t *tbl);
    void (*clear) (qlisttbl_t *tbl);

    bool (*save) (qlisttbl_t *tbl, const char *filepath, char sepchar,
                  bool encode);
    ssize_t (*load) (qlisttbl_t *tbl, const char *filepath, char sepchar,
                     bool decode);
    bool (*debug) (qlisttbl_t *tbl, FILE *out);

    void (*lock) (qlisttbl_t *tbl);
    void (*unlock) (qlisttbl_t *tbl);

    void (*free) (qlisttbl_t *tbl);

    /* private methods */
    bool (*namematch) (qlisttbl_obj_t *obj, const char *name, uint32_t hash);
    int (*namecmp) (const char *s1, const char *s2);

    /* private variables - do not access directly */
    bool unique;           /*!< keys are unique */
    bool caseinsensitive;  /*!< case insensitive key comparison */
    bool keepsorted;       /*!< keep table in sorted (default: insertion order) */
    bool inserttop;        /*!< add new key at the top. (default: bottom) */
    bool lookupforward;    /*!< find keys from the top. (default: backward) */

    void *qmutex;          /*!< initialized when QLISTTBL_OPT_THREADSAFE is given */
    size_t num;            /*!< number of elements */
    qlisttbl_obj_t *first; /*!< first object pointer */
    qlisttbl_obj_t *last;  /*!< last object pointer */
};

/**
 * qlisttbl node object data structure
 */
struct qlisttbl_obj_s {
    uint32_t hash;      /*!< 32bit-hash value of object name */
    char *name;         /*!< object name */
    void *data;         /*!< data */
    size_t size;        /*!< data size */

    qlisttbl_obj_t *prev;    /*!< previous link */
    qlisttbl_obj_t *next;    /*!< next link */
};

/**
 * qlisttbl value data structure for user return
 */
struct qlisttbl_data_s {
    void *data;         /*!< data */
    size_t size;        /*!< data size */
    uint8_t type;       /*!< data type, internal use */
};

#ifdef __cplusplus
}
#endif

#endif /* QLISTTBL_H */
