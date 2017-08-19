CC = g++ -std=c++11
#zmq_path = /home/utp/zmq
#zmq_inc = $(zmq_path)/include
#zmq_lib = $(zmq_path)/lib

all: hwclient hwserver

hwclient:hwclient.cpp
	$(CC) hwclient.cpp -o hwclient -lzmq -lzmqpp

hwserver:hwserver.cpp
	$(CC) hwserver.cpp -o hwserver -lzmq -lzmqpp
