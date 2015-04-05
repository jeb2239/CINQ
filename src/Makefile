CC  = gcc
CXX = g++

INCLUDES = -I../origin/

FLAGS    = -Wall -pedantic -O2 $(INCLUDES)
CFLAGS   = $(FLAGS)
CXXFLAGS = $(FLAGS) -std=c++1z

LDFLAGS = -lstdc++
LDLIBS  =

EXE = cinq_test
OBJ = cinq_test.o

.PHONY: default
default: $(EXE)

$(EXE): $(OBJ)

$(OBJ): cinq_enumerable.hpp cinq_test.hpp all_concepts.hpp test_code.cpp

.PHONY: clean
clean:
	rm -f *~ a.out core $(OBJ) $(EXE)

.PHONY: all
all: clean default
