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

typedef struct
{
    PyObject *create_gen;
} pysol_cards__master_instance_type;

static int pysol_cards__master_instance_init(
    pysol_cards__master_instance_type *const master_instance,
    global_python_instance_type *const global_python)
{
    PyObject *const create_gen =
        PyObject_GetAttrString(global_python->pModule, "create_gen");
    /* create_gen is a new reference */

    if (!(create_gen && PyCallable_Check(create_gen)))
    {
        if (PyErr_Occurred())
            PyErr_Print();
        fprintf(stderr, "Cannot access create_gen() \"%s\"\n", "pysol_cards_c");
        exit(PYSOL_CARDS__FAIL);
    }
    master_instance->create_gen = create_gen;
    return PYSOL_CARDS__SUCCESS;
}

static int global_python_instance__release(
    global_python_instance_type *const global_python)
{
    Py_DECREF(global_python->pModule);

    if (Py_FinalizeEx() < 0)
    {
        return 120;
    }
}

static void pysol_cards__master_instance_release(
    pysol_cards__master_instance_type *const master_instance)
{
    Py_XDECREF(master_instance->create_gen);
    master_instance->create_gen = NULL;
}

#define NUM_CARDS_DECKS 2
#define NUM_CARDS_RANKS 13
#define NUM_CARDS_SUITS 4
#define NUM_CHARS_PER_CARD 3
#define BOARD_STRING_MARGIN 512
#define BOARD_STRING_SIZE                                                      \
    (BOARD_STRING_MARGIN + NUM_CARDS_RANKS * NUM_CARDS_SUITS *                 \
                               NUM_CARDS_DECKS * NUM_CHARS_PER_CARD)

static int global_python_instance__init(
    global_python_instance_type *const global_python)
{
    Py_Initialize();
    const char *const modname = "pysol_cards_c";
    PyObject *const pName = PyUnicode_DecodeFSDefault(modname);
    /* Error checking of pName left out */

    global_python->pModule = PyImport_Import(pName);
    if (!global_python->pModule)
    {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", modname);
        exit(PYSOL_CARDS__FAIL);
    }
    Py_DECREF(pName);
    return PYSOL_CARDS__SUCCESS;
}

typedef struct
{
    PyObject *generator;
} pysol_cards__generator_type;

static void pysol_cards__create_generator(pysol_cards__generator_type *out,
    global_python_instance_type *const global_python, PyObject *const func,
    const char *const game_variant, int msdeals)
{
    PyObject *const pArgs = PyTuple_New(2);
    for (int i = 0; i < 2; ++i)
    {
        PyObject *const create_gen_param =
            ((i == 0) ? (PyUnicode_FromString(game_variant))
                      : (PyLong_FromLong(msdeals)));
        if (!create_gen_param)
        {
            Py_DECREF(pArgs);
            Py_DECREF(global_python->pModule);
            fprintf(stderr, "Cannot convert argument\n");
            exit(PYSOL_CARDS__FAIL);
        }
        /* create_gen_param reference stolen here: */
        PyTuple_SetItem(pArgs, i, create_gen_param);
    }
    PyObject *const generator_func = PyObject_CallObject(func, pArgs);
    Py_DECREF(pArgs);
    if (!generator_func)
    {
        Py_DECREF(func);
        Py_DECREF(global_python->pModule);
        PyErr_Print();
        fprintf(stderr, "Call failed\n");
        exit(PYSOL_CARDS__FAIL);
    }
    out->generator = generator_func;
}

static int pysol_cards__deal(pysol_cards__generator_type *const generator,
    char *const board_string, const long deal_idx)
{
    PyObject *const pArgs_gen = PyTuple_New(1);
    PyObject *const pValue_gen = (PyLong_FromLong(deal_idx));
    if (!pValue_gen)
    {
        return PYSOL_CARDS__FAIL;
    }
    /* pValue_gen reference stolen here: */
    PyTuple_SetItem(pArgs_gen, 0, pValue_gen);

    PyObject *const pRetString =
        PyObject_CallObject(generator->generator, pArgs_gen);
    const char *const ret_str = PyUnicode_AsUTF8(pRetString);
    strcpy(board_string, ret_str);
    Py_DECREF(pArgs_gen);
    Py_DECREF(pRetString);

    return PYSOL_CARDS__SUCCESS;
}
