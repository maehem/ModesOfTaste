#!/usr/bin/env python
# Listen for commands on the serial line

import datetime
import serial
from picamera import PiCamera
from time import sleep

camera = PiCamera()

def timeStamp():
	fmt='%Y%m%d%H%M%S'
	return datetime.datetime.now().strftime(fmt).format()

def takePicture():
	# take a picture with the camera
	print '<snap>'	
	# store the picture
	camera.start_preview()
	sleep(2)
	camera.capture('/home/pi/Desktop/' + timeStamp() + '-snap.jpg')
	camera.stop_preview()

def taste(val):
	# log the color sensor value from the taste command
	ser.write('<lick>')
	f=open("/home/pi/Desktop/tasteLog.txt","a+")
	f.write(timeStamp() + ':' + val + '\n')


def prompt():
	ser.write('\n\rnoodle> ')


ser = serial.Serial(
	port='/dev/ttyS0',
	baudrate = 9600,
	parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS
	#timeout=5
)
print(ser.name)
prompt()

line = ''
while 1:
	c=ser.read()
	# echo char
	ser.write(c)
	#	print x

	line += c

	if c == '\r':
	
		# split the line up
		word = line.split()	
		line = ''

		# parse the first item as a command
		print('----')
		print(word)	
		print('----')
		

		# was there even a command?
		if len(word) == 0:
			prompt()
			continue

		# if the command is 'snap' then call takePicture
		if word[0]=='snap':
			takePicture()
			ser.write('\n<tick>')
		if word[0]=='taste':
			if len(word) > 1:
				taste(word[1])
			else:
				serial.write('error: no value for tasting!')


		prompt()
	# end while section

