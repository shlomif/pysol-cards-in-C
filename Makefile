#
# Makefile
# Shlomi Fish, 2025-03-21 17:00
#

WRAPPEREXE = pysol_cards_wrap
# $(WRAPPEREXE)

all: $(WRAPPEREXE)

PYVER = 3.13
COMMON_OPT_FLAGS := -O3 -march=native

$(WRAPPEREXE) : call.c
	# gcc ` python3-config --cflags ` $(COMMON_OPT_FLAGS) -Wall -Wextra -o $@ $< ` python3-config --ldflags --libs ` -l python$(PYVER)
	clang ` python3-config --cflags ` $(COMMON_OPT_FLAGS) -Weverything -Wno-declaration-after-statement -Wno-disabled-macro-expansion -Wno-extra-semi-stmt -Wno-padded -Wno-reserved-identifier -Wno-reserved-macro-identifier -Wno-unsafe-buffer-usage -o $@ $< ` python3-config --ldflags --libs ` -l python$(PYVER)

run: all
	bash run.bash

clean:
	rm -f $(WRAPPEREXE)

rebuild: clean all

rerun: rebuild run

# vim:ft=make
#
