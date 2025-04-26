/*
 * libpysol_cards/python_embed.h
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
    PyObject *py_module;
} global_python_instance_type;

typedef struct
{
    PyObject *create_gen;
} pysol_cards__master_instance_type;

static int global_python_instance__init(
    global_python_instance_type *const global_python)
{
    Py_Initialize();
    const char *const modname = "pysol_cards_c";
    PyObject *const py_modname = PyUnicode_DecodeFSDefault(modname);
    /* Error checking of py_modname left out */

    global_python->py_module = PyImport_Import(py_modname);
    if (!global_python->py_module)
    {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", modname);
        exit(PYSOL_CARDS__FAIL);
    }
    Py_DECREF(py_modname);
    return PYSOL_CARDS__SUCCESS;
}

static int global_python_instance__release(
    global_python_instance_type *const global_python)
{
    Py_DECREF(global_python->py_module);

    if (Py_FinalizeEx() < 0)
    {
        return 120;
    }
}

static int pysol_cards__master_instance_init(
    pysol_cards__master_instance_type *const master_instance,
    global_python_instance_type *const global_python)
{
    PyObject *const create_gen =
        PyObject_GetAttrString(global_python->py_module, "create_gen");
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

typedef struct
{
    PyObject *generator;
} pysol_cards__generator_type;

static void pysol_cards__create_generator(
    pysol_cards__generator_type *const out,
    global_python_instance_type *const global_python, PyObject *const func,
    const char *const game_variant, int msdeals)
{
    PyObject *const py_args = PyTuple_New(2);
    for (int i = 0; i < 2; ++i)
    {
        PyObject *const create_gen_param =
            ((i == 0) ? (PyUnicode_FromString(game_variant))
                      : (PyLong_FromLong(msdeals)));
        if (!create_gen_param)
        {
            Py_DECREF(py_args);
            Py_DECREF(global_python->py_module);
            fprintf(stderr, "Cannot convert argument\n");
            exit(PYSOL_CARDS__FAIL);
        }
        /* create_gen_param reference stolen here: */
        PyTuple_SetItem(py_args, i, create_gen_param);
    }
    PyObject *const generator_func = PyObject_CallObject(func, py_args);
    Py_DECREF(py_args);
    if (!generator_func)
    {
        Py_DECREF(func);
        Py_DECREF(global_python->py_module);
        PyErr_Print();
        fprintf(stderr, "Call failed\n");
        exit(PYSOL_CARDS__FAIL);
    }
    out->generator = generator_func;
}

static int pysol_cards__deal(pysol_cards__generator_type *const generator,
    char *const board_string, const long deal_idx)
{
    PyObject *const generator_args = PyTuple_New(1);
    PyObject *const py__deal_idx = (PyLong_FromLong(deal_idx));
    if (!py__deal_idx)
    {
        return PYSOL_CARDS__FAIL;
    }
    /* py__deal_idx reference stolen here: */
    PyTuple_SetItem(generator_args, 0, py__deal_idx);

    PyObject *const py__ret_string =
        PyObject_CallObject(generator->generator, generator_args);
    const char *const ret_str = PyUnicode_AsUTF8(py__ret_string);
    strcpy(board_string, ret_str);
    Py_DECREF(generator_args);
    Py_DECREF(py__ret_string);

    return PYSOL_CARDS__SUCCESS;
}
