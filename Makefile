all: interface.o main.o
	gcc -o trabalho interface.o main.o

interface.o: interface.c
	gcc -o interface.o -c interface.c

main.o: main.c interface.c
	gcc -o main.o -c main.c 

clean:
	rm -rf *.o

mrproper: clean
	rm -rf teste