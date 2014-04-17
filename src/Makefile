# g++ (GCC) 4.8.2 20131219 (prerelease)
#CXX ?= g++
#CXXFLAGS ?= -Wall -DNDEBUG -O3 -march=native -std=c++11 -ffast-math

# icpc (ICC) 14.0.1 20131008
CXX = icpc
CXXFLAGS = -Wall -DNDEBUG -fast -std=c++11

# clang version 3.4 (tags/RELEASE_34/final)
#CXX = clang++
#CXXFLAGS = -Wall -DNDEBUG -Ofast -march=native -std=c++11

HEADERS = rexp.h xorshift64.h rgamma.h rnormal.h rbeta.h
OBJ = rexp.o
TARGETS = wrms-ht wrms-htu wrms-hw wrms-vt wrms-htm \
          wrms-htw wrms-vto wrms-vt-a wrms-vt-b std

.PHONY: clean
.PHONY: all

all: $(TARGETS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGETS): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.o *~ core

std: std.o
wrms-hw: wrms-hw.o
wrms-ht: wrms-ht.o
wrms-htm: wrms-htm.o
wrms-htw: wrms-htw.o
wrms-htu: wrms-htu.o
wrms-vt: wrms-vt.o rnormal.o
wrms-vt-a: wrms-vt-a.o rnormal.o
wrms-vt-b: wrms-vt-b.o rnormal.o
wrms-vto: wrms-vto.o rnormal.o

# Notes
# g++ -g -O3 -c -fverbose-asm -Wa,-adhln -march=native -std=c++11 -ffast-math wrms-vtu.cpp > test.txt


