CC=g++ -std=c++11

ZMQ_PATH=/usr/local

ZMQ_INC=$(ZMQ_PATH)/include
ZMQ_LIB=$(ZMQ_PATH)/lib

all:Node Node1

Node:node.cpp
	$(CC) -L$(ZMQ_LIB) -I$(ZMQ_INC) node.cpp -o node.out -lzmq -lzmqpp -lpthread

Node1:node2.cpp
	$(CC) -L$(ZMQ_LIB) -I$(ZMQ_INC) node2.cpp -o node2.out -lzmq -lzmqpp -lpthread
