all: TSPACO

TSPACO: TSPACO.o main.o 
	g++ -ggdb -O3 -o TSPACO TSPACO.o main.o

TSPACO.o: TSPACO.cpp
	g++ -ggdb -O3 -c TSPACO.cpp

main.o: main.cpp
	g++ -ggdb -O3 -c main.cpp

plot: 
	gnuplot pic.gp

dep:
	echo "Do nothing"

Clean:
	rm -f TSPACO TSPACO *.o *.png ~/Desktop/runtime.png  *.dat

clean:
	rm -f TSPACO TSPACO *.o *.dat
