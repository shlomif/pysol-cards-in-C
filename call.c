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
    PyObject *pValue;

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
            pValue = ((i == 0) ? (PyUnicode_FromString("black_hole"))
                               : (PyLong_FromLong(24)));
            if (!pValue)
            {
                Py_DECREF(pArgs);
                Py_DECREF(global_python->pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return 1;
            }
            /* pValue reference stolen here: */
            PyTuple_SetItem(pArgs, i, pValue);
        }
        pValue = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);
        if (!pValue)
        {
            Py_DECREF(pFunc);
            Py_DECREF(global_python->pModule);
            PyErr_Print();
            fprintf(stderr, "Call failed\n");
            return 1;
        }
        PyObject *pFunc_gen = pValue;
        for (int argvidx = 1; argvidx < argc; ++argvidx)
        {
            PyObject *pArgs_gen = PyTuple_New(1);
            for (int i = 0; i < 1; ++i)
            {
                PyObject *pValue_gen =
                    ((i == 0) ? (PyLong_FromLong(atoi(argv[argvidx]))) : NULL);
                if (!pValue_gen)
                {
                    Py_DECREF(pArgs);
                    Py_DECREF(global_python->pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs_gen, i, pValue_gen);
            }
            PyObject *const pRetString =
                PyObject_CallObject(pFunc_gen, pArgs_gen);
            const char *ret_str = PyUnicode_AsUTF8(pRetString);
            strcpy(board_string, ret_str);
            printf("%s", board_string);
            Py_DECREF(pRetString);
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
