import zmq
import random
import sys
import time
#import hashlib
import json
import pygame
import math
from pygame.locals import *

#constantes
WIDTH = 1000
HIGH = 700

# set up the colors
BLACK = (  0,   0,   0)
WHITE = (255, 255, 255)
RED   = (255,   0,   0)
GREEN = (  0, 255,   0)
BLUE  = (  0,   0, 255)
COLOR_TEXT = (0, 128, 128)
RADIO = 300

def Plotter():

	#se crea  la pantalla 
	pygame.init()
	screen = pygame.display.set_mode([WIDTH,HIGH])

	#screen.fill(BLACK)
	font = pygame.font.SysFont("comicsansms", 50)
	text = font.render(" *** Pirate Torrent ***", True, COLOR_TEXT)


	#load images
	background = pygame.image.load('background.jpg').convert_alpha()
	screen.blit(background, [0,0])

	screen.blit(text,(WIDTH/2+100,15))
	pygame.draw.circle(screen, WHITE, (WIDTH/3, HIGH/2+20), 300, 3)


	#draw nodes
	for i in xrange(0, 36):
		x = RADIO*math.cos(10*i)
		y = RADIO*math.sin(10*i)
		pygame.draw.circle(screen, (0,150,150), (int(x)+WIDTH/3, int(y)+HIGH/2+20), 15, 0)


	####################
	pygame.display.flip()

	# while True:
	# 	for event in pygame.event.get():
	# 		if event.type == pygame.QUIT:
	# 			sys.exit(0)


def main():

	Plotter()


	if len(sys.argv) != 2:
		print 'Enter server port'
		exit()
	context = zmq.Context()
	socket = context.socket(zmq.PULL)
	socket.bind('tcp://127.0.0.1:{}'.format(sys.argv[1]))

	while True:
		#req = json.loads(socket.recv())
		req = socket.recv_json()

		if req['op']=='add_node':
			print req['my_id']
			print req['my_ip']
		if req['op'] =='delete_node':
			pass



if __name__ == '__main__':
	main()