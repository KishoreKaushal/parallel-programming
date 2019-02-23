SRCS = $(wildcard *.cpp)
PROGS = $(patsubst %.cpp,%,$(SRCS))
CXX=g++
CXXFLAGS=-Wall -g
LIBS=-lpthread
DEFFLAGS=-DVERBOSE

all: $(PROGS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) -o $@ $< $(DEFFLAGS)

clean:
		rm -rf $(PROGS)