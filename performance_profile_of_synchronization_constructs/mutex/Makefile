SRCS = $(wildcard *.cpp)
PROGS = $(patsubst %.cpp,%,$(SRCS))
CXX=g++
CXXFLAGS=-Wall -g -std=c++17
LIBS=-lpthread
DEFFLAGS=-DVERBOSE

all: $(PROGS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@ $< $(DEFFLAGS)

clean:
		rm -rf $(PROGS)