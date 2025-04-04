//
// call.c
// Copyright (C) 2025 Shlomi Fish < https://www.shlomifish.org/ >
//
// Distributed under the terms of the MIT license.
//
// Based on https://docs.python.org/3/extending/embedding.html . Thanks!

#include "embed_python.h"

int global_python_instance__init(
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

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: call ./call.exe [game] [deals]\n");
        return 1;
    }
    global_python_instance_type global_python_struct;
    global_python_instance_type *const global_python = &global_python_struct;
    global_python_instance__init(global_python);

    PyObject *const pFunc =
        PyObject_GetAttrString(global_python->pModule, "create_gen");
    /* pFunc is a new reference */
    char board_string[BOARD_STRING_SIZE];

    if (!(pFunc && PyCallable_Check(pFunc)))
    {
        if (PyErr_Occurred())
            PyErr_Print();
        fprintf(stderr, "Cannot find game_variant \"%s\"\n", argv[2]);
        goto cleanup_module;
    }
    PyObject *const pFunc_gen =
        pysol_cards__create_generator(global_python, pFunc, argv[1], 0);
    for (int argvidx = 2; argvidx < argc; ++argvidx)
    {
        char *const arg = argv[argvidx];
        long startidx, endidx;
        if (!strcmp(arg, "seq"))
        {
            if (argvidx + 2 + 0 >= argc)
            {
                exit(1);
            }
            startidx = atol(argv[++argvidx]);
            endidx = atol(argv[++argvidx]);
        }
        else
        {
            endidx = startidx = atol(arg);
        }
        for (long deal_idx = startidx; deal_idx <= endidx; ++deal_idx)
        {
            const int ret_code =
                pysol_cards__deal(board_string, pFunc_gen, deal_idx);
            if (ret_code)
            {
                Py_DECREF(global_python->pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return PYSOL_CARDS__FAIL;
            }
            printf("%s", board_string);
            if ((deal_idx & ((1 << 12) - 1)) == 0)
            {
                fprintf(stderr, "Reached %ld\n", deal_idx);
                fflush(stderr);
            }
        }
    }
    Py_XDECREF(pFunc);

cleanup_module:
    Py_DECREF(global_python->pModule);

    if (Py_FinalizeEx() < 0)
    {
        return 120;
    }
    return PYSOL_CARDS__SUCCESS;
}
