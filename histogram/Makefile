all:
	make clean
	make run
clean:
	rm -f *.out
local:
	g++ -Wall histogram_local.cpp -lpthread -o histogram_local.out
	time ./histogram_local.out
global:
	g++ -Wall histogram_global.cpp -lpthread -o histogram_global.out
	time ./histogram_global.out
run:
	g++ -Wall histogram_global.cpp -lpthread -o histogram_global.out
	g++ -Wall histogram_local.cpp -lpthread -o histogram_local.out
	time ./histogram_global.out
	time ./histogram_local.out