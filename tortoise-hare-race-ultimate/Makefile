all:
	make clean
	make run
clean:
	rm -f *.out
run:
	g++ -Wall hare_turtle_race.cpp -lpthread 
	./a.out
test:
	g++ -Wall -DTEST hare_turtle_race.cpp -lpthread 
	./a.out