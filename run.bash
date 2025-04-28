#! /usr/bin/env bash
#
# run.bash
# Copyright (C) 2025 Shlomi Fish < https://www.shlomifish.org/ >
#
# Distributed under the terms of the MIT license.
#

set -e -x

wrapperexe_basename="pysol_cards_wrap"
wrapperexe_dot="./${wrapperexe_basename}"
wrapperexe="${wrapperexe_dot}"

pyp="${HOME}/.local/lib/python3.13/site-packages"
export PYTHONPATH="`pwd`"
PYTHONPATH+=":${pyp}"
printf "%s\\n" "${PYTHONPATH}"
# make

"${wrapperexe}" black_hole 3 8
"${wrapperexe}" black_hole 200 3

_start()
{
    local deals_slice_index="$1"
    shift
    printf "%d" "$(expr "$deals_slice_index" '*' 5 + 1)"
}

_end()
{
    local deals_slice_index="$1"
    shift
    printf "%d" "$(expr "$deals_slice_index" '*' 5 + 4)"
}

expected()
{
    local deals_slice_index="$1"
    shift
    local s="$(_start "$deals_slice_index")"
    local e="$(_end "$deals_slice_index")"
    for (( i = s; i <= e; i++ ))
    do
        make_pysol_freecell_board.py -t "$i" black_hole
    done
}

got()
{
    local deals_slice_index="$1"
    shift
    local s="$(_start "$deals_slice_index")"
    local e="$(_end "$deals_slice_index")"
    eval "\"${wrapperexe}\" black_hole {${s}..${e}}"
}

run_test()
{
    (
        set +x

        for (( i = 0; i <= 10; i++ ))
        do
            > /dev/null diff -u <(expected "${i}") <(got "${i}")
        done
    )
}

run_test

srcdir="`pwd`"
bindir="${srcdir}/../pysol-cards-in-C-build-dir-to-del"
(
    set +x

    rm -fr "${bindir}"
    mkdir -p "${bindir}"
    cd "${bindir}"
    cmake "${srcdir}"
    gmake VERBOSE=1
)
wrapperexe_cmake="${bindir}/${wrapperexe_basename}"
wrapperexe="${wrapperexe_cmake}"
run_test
