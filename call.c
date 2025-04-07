//
// call.c
// Copyright (C) 2025 Shlomi Fish < https://www.shlomifish.org/ >
//
// Distributed under the terms of the MIT license.
//
// Based on https://docs.python.org/3/extending/embedding.html . Thanks!

#include "embed_python.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: call ./call.exe [game] [deals]\n");
        return PYSOL_CARDS__FAIL;
    }
    global_python_instance_type global_python_struct;
    global_python_instance_type *const global_python = &global_python_struct;
    global_python_instance__init(global_python);
    pysol_cards__master_instance_type master_instance_struct;
    pysol_cards__master_instance_type *const master_instance =
        &master_instance_struct;
    pysol_cards__master_instance_init(master_instance, global_python);
    char board_string[BOARD_STRING_SIZE];
    pysol_cards__generator_type generator;
    pysol_cards__create_generator(
        &generator, global_python, master_instance->create_gen, argv[1], 0);
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
                pysol_cards__deal(&generator, board_string, deal_idx);
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
    pysol_cards__master_instance_release(master_instance);
    global_python_instance__release(global_python);
    return PYSOL_CARDS__SUCCESS;
}
