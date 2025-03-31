#
# Makefile
# Shlomi Fish, 2025-03-21 17:00
#

all: call.exe

call.exe: call.c
	gcc ` python3-config --cflags ` -o $@ $< ` python3-config --ldflags --libs ` -l python3.14

run: all
	bash run.bash

# vim:ft=make
#
