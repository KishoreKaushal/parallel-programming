SRCS = $(wildcard *.cpp)
PROGS = $(patsubst %.cpp,%,$(SRCS))
CXX=g++
CXXFLAGS=-Wall -g
LIBS=-lpthread

all: $(PROGS)
%: %.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@ $< 
