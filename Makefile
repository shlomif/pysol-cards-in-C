#
# Makefile
# Shlomi Fish, 2025-03-21 17:00
#

all: call.exe

call.exe: call.c
	# gcc ` python3-config --cflags ` -O3 -march=native -o $@ $< ` python3-config --ldflags --libs ` -l python3.13
	clang ` python3-config --cflags ` -O3 -march=native -o $@ $< ` python3-config --ldflags --libs ` -Weverything -Wno-declaration-after-statement -Wno-disabled-macro-expansion -Wno-extra-semi-stmt -Wno-padded -Wno-reserved-identifier -Wno-reserved-macro-identifier -l python3.13

run: all
	bash run.bash

clean:
	rm -f call.exe

rebuild: clean all

# vim:ft=make
#
