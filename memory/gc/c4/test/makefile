all:
	gcc -c ../heapManagement.c ../rtinfo.c ../phases.c ../rootSet.c increment.c
	g++ -std=c++11 -o test ../../../../process/thread.cpp heapManagement.o rtinfo.o phases.o rootSet.o increment.o