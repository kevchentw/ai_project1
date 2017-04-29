all: q1 q2 q3 run_q1 run_q2 run_q3

q1: q1.cpp
	g++ q1.cpp -std=c++14  -O3 -o q1.o

q2: q2.cpp
	g++ q2.cpp -std=c++14  -O3 -o q2.o

q3: q3.cpp
	g++ q3.cpp -std=c++14  -O3 -o q3.o

run_q1:
	./q1.o > ./output/q1.output

run_q2:
	./q2.o > ./output/q2.output

run_q3:
	./q3.o > ./output/q3.output

clean:
	rm -f q1.o q2.o q3.o ./output/q1.output ./output/q2.output ./output/q3.output
