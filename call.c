//
// call.c
// Copyright (C) 2025 Shlomi Fish < https://www.shlomifish.org/ >
//
// Distributed under the terms of the MIT license.
//

#define PY_SSIZE_T_CLEAN
#include <Python.h>

int main(int argc, char *argv[])
{
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: call pythonfile funcname [args]\n");
        return 1;
    }

    Py_Initialize();
    pName = PyUnicode_DecodeFSDefault("multiply");
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL)
    {
        // pFunc = PyObject_GetAttrString(pModule, argv[2]);
        pFunc = PyObject_GetAttrString(pModule, "create_gen");
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc))
        {
            pArgs = PyTuple_New(1);
            for (i = 0; i < 1; ++i)
            {
                pValue = ((i == 0) ? (PyUnicode_FromString("black_hole"))
                                   : (

                                         PyLong_FromLong(24)));
                if (!pValue)
                {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL)
            {
                PyObject *pFunc_gen = pValue;
                for (int argvidx = 1; argvidx < argc; ++argvidx)
                {
                    PyObject *pArgs_gen = PyTuple_New(1);
                    for (i = 0; i < 1; ++i)
                    {
                        PyObject *pValue_gen =
                            ((i == 0) ? (PyLong_FromLong(atoi(argv[argvidx])))
                                      : NULL);
                        if (!pValue_gen)
                        {
                            Py_DECREF(pArgs);
                            Py_DECREF(pModule);
                            fprintf(stderr, "Cannot convert argument\n");
                            return 1;
                        }
                        /* pValue reference stolen here: */
                        PyTuple_SetItem(pArgs_gen, i, pValue_gen);
                    }
                    pValue = PyObject_CallObject(pFunc_gen, pArgs_gen);
                    const char *ret_str = PyUnicode_AsUTF8(pValue);
                    printf("%s", ret_str);
                    Py_DECREF(pValue);
                }
            }
            else
            {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr, "Call failed\n");
                return 1;
            }
        }
        else
        {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else
    {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
        return 1;
    }
    if (Py_FinalizeEx() < 0)
    {
        return 120;
    }
    return 0;
}
