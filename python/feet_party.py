#!/usr/bin/env python
# Listen for commands on the serial line

import logging
import datetime
import serial
from picamera import PiCamera
from time import sleep
from geolocation.main import GoogleMaps
from geolocation.distance_matrix.client import DistanceMatrixApiClient
import urllib2
import json


camera = PiCamera()
logger = logging.getLogger('Feet Party')

def timeStamp():
	fmt='%Y%m%d%H%M%S'
	return datetime.datetime.now().strftime(fmt).format()

def initLogger():
    hdlr = logging.FileHandler('~/feetParty.log')
    formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
    hdlr.setFormatter(formatter)
    logger.addHandler(hdlr)
    logger.setLevel(logging.WARNING)

def takePicture():
	# take a picture with the camera
	print '<snap>'	
	# store the picture
	camera.start_preview()
	sleep(2)
	camera.capture('/home/pi/Desktop/' + timeStamp() + '-snap.jpg')
	camera.stop_preview()
	ser.write('<tick>')

def taste(val):
	# log the color sensor value from the taste command
	ser.write('<lick>')
	f=open("/home/pi/Desktop/tasteLog.txt","a+")
	f.write(timeStamp() + ':' + val + '\n')

def whereAreMyFeet():
	f=open("~/google-map-api-key.txt", "r")
	api_key=f.readline()
	google_maps = GoogleMaps(api_key)

	#location = google_maps.search(location=address) # sends search to Google Maps.
	location = google_maps.search() # sends search to Google Maps.

	print(location.all()) # returns all locations.

	my_location = location.first() # returns only first location.

	print(my_location.city)
	print(my_location.route)
	print(my_location.street_number)
	print(my_location.postal_code)

def whereAreMyFeet2():
	f=urllib2.urlopen("https://api.ipify.org/?format=json")
	json_string = f.read()
	f.close()
	stuff = json.loads(json_string)
	ip = stuff['ip']
	# Automatically geolocate the connecting IP
	f = urllib2.urlopen('http://freegeoip.net/json/'+ip)
	json_string = f.read()
	f.close()
	location = json.loads(json_string)
	#print(location)
	location_city = location['city']
	#location_state = location['region_name']
	#location_country = location['country_name']
	#location_zip = location['zip_code']
	ser.write('\n\r' + location['city'])


def prompt():
	ser.write('\n\rnoodle> ')


initLogger()

logger.info("It's a Feet Party!!!!")

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
		if word[0]=='taste':
			if len(word) > 1:
				taste(word[1])
			else:
				serial.write('error: no value for tasting!')
		if word[0]=='locate':
			whereAreMyFeet2()
        if word[0]=='log':
            # Write the rest of the line to the log file.


		prompt()
	# end while section

