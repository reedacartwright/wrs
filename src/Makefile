CXX = c++
CXXFLAGS = -g -O2 -std=c++11
HEADERS = rbeta.h  rexp.h  rgamma.h  rnormal.h  xorshift64.h
OBJ = rnormal.o rexp.o
TARGETS = wrms-ht wrms-htu wrms-hw wrms-vtw wrms-htm  wrms-htw  wrms-vtu

.PHONY: clean
.PHONY: all

all: $(TARGETS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGETS): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.o *~ core

wrms-ht: wrms-ht.o
wrms-hw: wrms-hw.o
wrms-htu: wrms-htu.o
wrms-vtw: wrms-vtw.o
wrms-htm: wrms-htm.o
wrms-htw: wrms-htw.o
wrms-vtu: wrms-vtu.o

