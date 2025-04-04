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
    PYSOL_CARDS__SUCCESS = 0,
    PYSOL_CARDS__FAIL = 1,
};

typedef struct
{
    PyObject *pModule;
} global_python_instance_type;

#define NUM_CARDS_DECKS 2
#define NUM_CARDS_RANKS 13
#define NUM_CARDS_SUITS 4
#define NUM_CHARS_PER_CARD 3
#define BOARD_STRING_MARGIN 512
#define BOARD_STRING_SIZE                                                      \
    (BOARD_STRING_MARGIN + NUM_CARDS_RANKS * NUM_CARDS_SUITS *                 \
                               NUM_CARDS_DECKS * NUM_CHARS_PER_CARD)

extern int global_python_instance__init(
    global_python_instance_type *const global_python);

static int pysol_cards__deal(
    char *const board_string, PyObject *const pFunc_gen, const long deal_idx)
{
    PyObject *const pArgs_gen = PyTuple_New(1);
    PyObject *const pValue_gen = (PyLong_FromLong(deal_idx));
    if (!pValue_gen)
    {
        return PYSOL_CARDS__FAIL;
    }
    /* pValue_gen reference stolen here: */
    PyTuple_SetItem(pArgs_gen, 0, pValue_gen);

    PyObject *const pRetString = PyObject_CallObject(pFunc_gen, pArgs_gen);
    const char *const ret_str = PyUnicode_AsUTF8(pRetString);
    strcpy(board_string, ret_str);
    Py_DECREF(pArgs_gen);
    Py_DECREF(pRetString);

    return PYSOL_CARDS__SUCCESS;
}
