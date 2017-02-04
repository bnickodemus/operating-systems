all: my_tar

my_tar: my_tar.o
		gcc my_tar.o -o my_tar

my_tar.o: my_tar.c
		gcc -c my_tar.c

clean:
		rm *.o
		rm my_tar
