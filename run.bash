#! /usr/bin/env bash
#
# run.bash
# Copyright (C) 2025 Shlomi Fish < https://www.shlomifish.org/ >
#
# Distributed under the terms of the MIT license.
#

set -e -x
wrapperexe=./pysol_cards_wrap

pyp="${HOME}/.local/lib/python3.13/site-packages"
export PYTHONPATH="`pwd`"
PYTHONPATH+=":${pyp}"
printf "%s\\n" "${PYTHONPATH}"
# make

"${wrapperexe}" black_hole 3 8
"${wrapperexe}" black_hole 200 3

_start()
{
    local deal="$1"
    shift
    printf "%d" "$(expr "$deal" '*' 5 + 1)"
}

_end()
{
    local deal="$1"
    shift
    printf "%d" "$(expr "$deal" '*' 5 + 4)"
}

expected()
{
    local deal="$1"
    shift
    local s="$(_start "$deal")"
    local e="$(_end "$deal")"
    for (( i = s; i <= e; i++ ))
    do
        make_pysol_freecell_board.py -t "$i" black_hole
    done
}

got()
{
    local deal="$1"
    shift
    local s="$(_start "$deal")"
    local e="$(_end "$deal")"
    eval "\"${wrapperexe}\" black_hole {${s}..${e}}"
}

run_test()
{
    (
        set +x

        for (( i = 0; i <= 10; i++ ))
        do
            diff -u <(expected "${i}") <(got "${i}")
        done
    )
}

run_test

srcdir="`pwd`"
bindir="${srcdir}/b"
(
    set +x

    rm -fr "${bindir}"
    mkdir -p "${bindir}"
    cd "${bindir}"
    cmake "${srcdir}"
    gmake VERBOSE=1
)
wrapperexe="${bindir}/pysol_cards_wrap"
run_test
