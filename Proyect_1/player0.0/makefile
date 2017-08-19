CC = g++ -std=c++11

all: Client serverMusic

hwclient:Client.cpp
	$(CC) Client.cpp -o client -lzmq -lzmqpp -lsfml-audio

hwserver:serverMusic.cpp
	$(CC) serverMusic.cpp -o server -lzmq -lzmqpp
