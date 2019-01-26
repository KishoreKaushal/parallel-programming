all:
	make clean
	make run
clean:
	rm -f *.out
run:
	g++ -Wall harmonic_sum.cpp -lpthread -o harmonic_sum.out
	g++ -Wall harmonic_sum_without_mutex.cpp -lpthread -o harmonic_sum_without_mutex.out
	g++ -Wall harmonic_sum_with_n.cpp -lpthread -o harmonic_sum_with_n.out
	g++ -Wall harmonic_sum_with_n.cpp -lpthread -o harmonic_sum_with_n_with_mutex.out
	time ./harmonic_sum.out
	time ./harmonic_sum_without_mutex.out
	time ./harmonic_sum_with_n.out
	time ./harmonic_sum_with_n_with_mutex.out