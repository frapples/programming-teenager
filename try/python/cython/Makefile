all:
	cython3 -3 -o hello.c hello.pyx
	gcc -g -O2 -fpic -c hello.c -o hello.o `python3-config --cflags`
	gcc -g -O2 -shared -o hello.so hello.o `python3-config --libs`
