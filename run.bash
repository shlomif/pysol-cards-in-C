#! /usr/bin/env bash
#
# run.bash
# Copyright (C) 2025 Shlomi Fish < https://www.shlomifish.org/ >
#
# Distributed under the terms of the MIT license.
#

set -e -x

export PYTHONPATH="`pwd`"
# make
./call.exe multiply multiply 3 8
./call.exe multiply multiply 200 3
