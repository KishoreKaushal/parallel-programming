all: god.out hare.out reporter.out turtle.out
god.out: udef.hpp god.cpp
	g++ -Wall god.cpp -o god.out
hare.out: udef.hpp hare.cpp
	g++ -Wall hare.cpp -o hare.out
turtle.out: udef.hpp turtle.cpp
	g++ -Wall turtle.cpp -o turtle.out
reporter.out: udef.hpp reporter.cpp 
	g++ -Wall reporter.cpp -o reporter.out
clean:
	rm -f *.out
	rm -f /tmp/hare2turtle 
	rm -f /tmp/turtle2hare
	rm -f /tmp/hare2reporter
	rm -f /tmp/turtle2reporter
	rm -f /tmp/god2hare
	rm -f /tmp/god2turtle
	rm -f /tmp/hare2god
	rm -f /tmp/turtle2god

