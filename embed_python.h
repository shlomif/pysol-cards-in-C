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
