CC  = g++
CXX = g++

INCLUDES = -I../origin/

FLAGS    = -Wall -pedantic -O2 $(INCLUDES)
CFLAGS   = $(FLAGS)
CXXFLAGS = $(FLAGS) -std=c++1z -fconcepts

LDFLAGS = -lstdc++
LDLIBS  =

EXE = cinq_test
OBJ = cinq_test.o test_performance.o

.PHONY: default
default: $(EXE)

$(EXE): $(OBJ)

$(OBJ): cinq_enumerable.hpp cinq_test.hpp test_performance.hpp test_shared.hpp all_concepts.hpp

.PHONY: clean
clean:
	rm -f *~ a.out core $(OBJ) $(EXE)

.PHONY: all
all: clean default