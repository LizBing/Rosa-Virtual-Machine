all:
	gcc -c ../allocator.c ../info.c ../phases.c ../rootSet.c test.c
	g++ -g -std=c++11 -o test ../../../../process/thread.cpp allocator.o info.o phases.o rootSet.o test.o
