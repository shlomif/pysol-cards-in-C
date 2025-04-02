/*
 * embed_python.h
 * Copyright (C) 2025 Shlomi Fish <shlomif@cpan.org>
 *
 * Distributed under terms of the MIT license.
 */
#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>

enum
{
    SUCCESS = 0,
    FAIL = 1,
};

typedef struct
{
    PyObject *pModule;
} global_python_instance_type;

extern int global_python_instance__init(
    global_python_instance_type *const global_python);

static int pysol_cards__deal(
    char *const board_string, PyObject *const pFunc_gen, const long deal_idx)
{
    PyObject *const pArgs_gen = PyTuple_New(1);
    for (int i = 0; i < 1; ++i)
    {
        PyObject *const pValue_gen =
            ((i == 0) ? (PyLong_FromLong(deal_idx)) : NULL);
        if (!pValue_gen)
        {
            return FAIL;
        }
        /* pValue_gen reference stolen here: */
        PyTuple_SetItem(pArgs_gen, i, pValue_gen);
    }

    PyObject *const pRetString = PyObject_CallObject(pFunc_gen, pArgs_gen);
    const char *const ret_str = PyUnicode_AsUTF8(pRetString);
    strcpy(board_string, ret_str);
    Py_DECREF(pRetString);

    return SUCCESS;
}
