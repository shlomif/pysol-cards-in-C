//
// call.c
// Copyright (C) 2025 Shlomi Fish < https://www.shlomifish.org/ >
//
// Distributed under the terms of the MIT license.
//
//
#include "embed_python.h"

int global_python_instance__init(
    global_python_instance_type *const global_python)
{
    Py_Initialize();
    const char *const modname = "multiply";
    PyObject *pName = PyUnicode_DecodeFSDefault(modname);
    /* Error checking of pName left out */

    global_python->pModule = PyImport_Import(pName);
    if (!global_python->pModule)
    {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", modname);
        exit(FAIL);
    }
    Py_DECREF(pName);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: call ./call.exe [deals]\n");
        return 1;
    }
    global_python_instance_type global_python_struct;
    global_python_instance_type *global_python = &global_python_struct;
    global_python_instance__init(global_python);

    // pFunc = PyObject_GetAttrString(pModule, argv[2]);
    PyObject *const pFunc =
        PyObject_GetAttrString(global_python->pModule, "create_gen");
    /* pFunc is a new reference */
    char board_string[3 * 4 * 13 * 2 * 2];

    if (pFunc && PyCallable_Check(pFunc))
    {
        PyObject *const pArgs = PyTuple_New(1);
        for (int i = 0; i < 1; ++i)
        {
            PyObject *const create_gen_param =
                ((i == 0) ? (PyUnicode_FromString("black_hole")) : NULL);
            if (!create_gen_param)
            {
                Py_DECREF(pArgs);
                Py_DECREF(global_python->pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return 1;
            }
            /* create_gen_param reference stolen here: */
            PyTuple_SetItem(pArgs, i, create_gen_param);
        }
        PyObject *const pFunc_gen = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);
        if (!pFunc_gen)
        {
            Py_DECREF(pFunc);
            Py_DECREF(global_python->pModule);
            PyErr_Print();
            fprintf(stderr, "Call failed\n");
            return 1;
        }
        for (int argvidx = 1; argvidx < argc; ++argvidx)
        {
            const long deal_idx = atoi(argv[argvidx]);
            const int ret_code =
                pysol_cards__deal(board_string, pFunc_gen, deal_idx);
            if (ret_code)
            {
                Py_DECREF(pArgs);
                Py_DECREF(global_python->pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return FAIL;
            }
            printf("%s", board_string);
        }
    }
    else
    {
        if (PyErr_Occurred())
            PyErr_Print();
        fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
    }
    Py_XDECREF(pFunc);
    Py_DECREF(global_python->pModule);

    if (Py_FinalizeEx() < 0)
    {
        return 120;
    }
    return 0;
}
