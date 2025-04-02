#
# Makefile
# Shlomi Fish, 2025-03-21 17:00
#

all: call.exe

PYVER = 3.13
COMMON_OPT_FLAGS := -O3 -march=native

call.exe: call.c
	# gcc ` python3-config --cflags ` $(COMMON_OPT_FLAGS) -o $@ $< ` python3-config --ldflags --libs ` -l python$(PYVER)
	clang ` python3-config --cflags ` $(COMMON_OPT_FLAGS) -o $@ $< ` python3-config --ldflags --libs ` -Weverything -Wno-declaration-after-statement -Wno-disabled-macro-expansion -Wno-extra-semi-stmt -Wno-padded -Wno-reserved-identifier -Wno-reserved-macro-identifier -Wno-unsafe-buffer-usage -l python$(PYVER)

run: all
	bash run.bash

clean:
	rm -f call.exe

rebuild: clean all

rerun: rebuild run

# vim:ft=make
#
