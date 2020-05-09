# comment

include Makefile.in
-include Makefile.doesntexist

.PHONY: all

all: target

foo = bar $(var) \
    $(baz) quux

ifeq ($(CC),gcc)
    bla=$(call func,param1,param2)
else ifeq "x" "y"
    $(error this seems wrong)
else
    libs=$(normal_libs)
endif

target1:
        error

target2: bla.cpp $@
	$(CC) bla.c -o bla.o
	$(CC) bla.o $< \
	    -o bla.exe
	@echo "hello"

# String Substitution
VAR := $(ANOTHER_VAR:%=%+1)

rempl := x
foo := a b c
bar := foo
hello := $($(bar):a=$(rempl))

# Conditionals
bar =
foo = $(bar)
ifdef bar
frobozz = yes
else ifdef foo
frobozz = no
endif

error_type := 0
check:
ifeq ($(error_type),0)
else ifeq ($(error_type),1)
else ifeq ($(error_type),2)
else ifeq ($(error_type),3)
endif
