CC = g++ -std=c++11
zmq_path = /usr/local
zmq_inc = $(zmq_path)/include
zmq_lib = $(zmq_path)/lib

all: Client serverMusic

hwclient:Client.cpp
	$(CC) -l$(zmq_lib) -I$(zmq_inc) Client.cpp -o client -lzmq -lzmqpp

hwserver:serverMusic.cpp
	$(CC) -L$(zmq_lib) -I$(zmq_inc) serverMusic.cpp -o server -lzmq -lzmqpp
