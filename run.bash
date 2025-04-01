#! /usr/bin/env bash
#
# run.bash
# Copyright (C) 2025 Shlomi Fish < https://www.shlomifish.org/ >
#
# Distributed under the terms of the MIT license.
#

set -e -x

pyp="${HOME}/.local/lib/python3.13/site-packages"
export PYTHONPATH="`pwd`"
PYTHONPATH+=":${pyp}"
printf "%s\\n" "${PYTHONPATH}"
# make
./call.exe 3 8
./call.exe 200 3

expected()
{
    local deal="$1"
    make_pysol_freecell_board.py -t "$deal" black_hole
}

got()
{
    local deal="$1"
    ./call.exe "$deal"
}

(
    set +x

    for (( i = 1; i <= 50; i++ ))
    do
        diff -u <(expected "${i}") <(got "${i}")
    done
)
