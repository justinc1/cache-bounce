CXX = g++
CXXFLAGS = -std=c++11
LDFLAGS = -pthread

CXXFLAGS += -DCACHELINE_SIZE=$(shell getconf LEVEL1_DCACHE_LINESIZE)

SOURCES = cache-bounce.cc

OBJ = $(SOURCES:%.cc=%.o)

all: cache-bounce

%.o: %.cc Makefile
	$(CXX) $(CXXFLAGS) -c -o $@ $<

cache-bounce: $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o $@
	
