all:
	make clean
	make run
clean:
	rm -f *.out
local:
	g++ -Wall busy_wait.cpp -lpthread -o busy_wait.out
	time ./busy_wait.out
global:
	g++ -Wall mutex.cpp -lpthread -o mutex.out
	time ./mutex.out
run:
	g++ -Wall busy_wait.cpp -lpthread -o busy_wait.out
	g++ -Wall mutex.cpp -lpthread -o mutex.out
	time ./busy_wait.out
	time ./mutex.out