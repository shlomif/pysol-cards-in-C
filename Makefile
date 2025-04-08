#
# Makefile
# Shlomi Fish, 2025-03-21 17:00
#

WRAPPEREXE = pysol_cards_wrap

all: $(WRAPPEREXE)

PYVER = 3.13
COMMON_OPT_FLAGS := -O3 -march=native
CFLAGS = ` python3-config --cflags ` $(COMMON_OPT_FLAGS)
LDFLAGS = ` python3-config --ldflags --libs ` -l python$(PYVER)

$(WRAPPEREXE) : call.c
	# gcc $(CFLAGS) -Wall -Wextra -o $@ $< $(LDFLAGS)
	clang $(CFLAGS) -Weverything -Wno-declaration-after-statement -Wno-disabled-macro-expansion -Wno-extra-semi-stmt -Wno-padded -Wno-reserved-identifier -Wno-reserved-macro-identifier -Wno-unsafe-buffer-usage -o $@ $< $(LDFLAGS)

run: all
	bash run.bash

test: run

clean:
	rm -f $(WRAPPEREXE)

rebuild: clean all

rerun: rebuild run

retest: rerun

# vim:ft=make
#
