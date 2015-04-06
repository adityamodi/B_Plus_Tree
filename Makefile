CPP=g++ -std=c++11
DEBUG=-g

#build

all: main

main: bptree.cpp
	$(CPP) $(DEBUG) bptree.cpp -o bpt.o

clean: fresh
	rm bpt

fresh:
	rm -rf ./data/*
	echo $(M) > bplustree.config
	echo 1 >> bplustree.config
