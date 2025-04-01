#
# Makefile
# Shlomi Fish, 2025-03-21 17:00
#

all: call.exe

call.exe: call.c
	gcc ` python3-config --cflags ` -O3 -march=native -o $@ $< ` python3-config --ldflags --libs ` -l python3.13

run: all
	bash run.bash

# vim:ft=make
#
