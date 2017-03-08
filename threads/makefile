all: hw5

hw5: hw5.o
		gcc hw5.o -o hw5 -lpthread

hw5.o: hw5.c
		gcc -c hw5.c

clean:
		rm *.o
		rm hw5
