#!/usr/bin/env python
# Listen for commands on the serial line

import socket
import requests
import logging
import datetime
import serial
from picamera import PiCamera
from time import sleep
#from geolocation.main import GoogleMaps
#from geolocation.distance_matrix.client import DistanceMatrixApiClient
import urllib2
#import urllib
import json
#import sys
from twython import Twython
import ConfigParser
from os.path import expanduser
import colormessages


#debugTwitPhoto = True
debugTwitPhoto = False

#home = expanduser("~")
home = expanduser("/home/pi")
logger = logging.getLogger('Feet Party')
loggerFileName   = home + '/feetParty.log'

tasteLogFileName = home + '/Desktop/tasteLog.txt'
snapFilesDir     = home + '/Desktop/'
thereAreColors = False
thereArePictures = False
lastHour = 0

# import the 'key' variable from the home directory.
execfile(home + "/ip-key.py")

#tweetHashTags = "#arselectronica17 #noodlefeet #tweetfeet"
tweetHashTags = "#tweetfeet"

Config = ConfigParser.ConfigParser()

def twitterInit():
    global twitter
    Config.read("/home/pi/twitter-config.ini")
    tc = ConfigSectionMap("Twitter")
    twitter = Twython(tc['apikey'],tc['apisecret'],
                  tc['accesstoken'],tc['accesstokensecret'] )
    logger.info( "Twitter Init" )


def twitterPostPic(tweetStr, imageFile):
    global twitter
    photo = open(imageFile, 'rb')
    #    response = twitter.upload_media(media=photo)
    #   twitter.update_status_with_media(status=tweetStr,
    #                          media_ids=[response['media_id']])
    twitter.update_status_with_media(status=tweetStr, media=photo)
    logger.info( "Tweeted with image: " + tweetStr )

def twitterPostText(tweetStr):
    global twitter
    twitter.update_status(status=tweetStr)
    logger.info( "Tweeted: " + tweetStr )

def ConfigSectionMap(section):
    dict1 = {}
    options = Config.options(section)
    for option in options:
        try:
            dict1[option] = Config.get(section, option)
            if dict1[option] == -1:
                DebugPrint("skip: %s" % option)
        except:
            print("exception on %s!" % option)
            dict1[option] = None
    return dict1


def timeStamp():
    fmt='%Y%m%d%H%M%S'
    return datetime.datetime.now().strftime(fmt).format()

def initLogger():
    hdlr = logging.FileHandler(loggerFileName)
    formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
    hdlr.setFormatter(formatter)
    logger.addHandler(hdlr)
    logger.setLevel(logging.INFO)
    logger.info("----- Logger running ---------------------")

def takePicture():
    global lastImageFile
    global thereArePictures
    # take a picture with the camera
    # store the picture
    camera.start_preview()
    sleep(2)
    lastImageFile = snapFilesDir + timeStamp() + '-snap.jpg'
    camera.capture(lastImageFile)
    camera.stop_preview()
    thereArePictures = bool(1)
    ser.write('<tick>')
    logger.info('photo snapped: ' + lastImageFile )

def taste(vr, vg, vb, vc):
    global lastRed, lastGreen, lastBlue, lastClear
    global thereAreColors
    # log the color sensor value from the taste command
    ser.write('<lick>')
    f=open(tasteLogFileName,"a+")
    f.write(timeStamp() + ':' + vr + ' '+ vg + ' ' + vb + ' ' + vc + '\n')
    thereAreColors = True
    logger.info('color tasted: %d %d %d %d', vr,vg,vb,vc )
    lastRed = vr
    lastGreen = vg
    lastBlue = vb
    lastClear = vc

#def whereAreMyFeet():
#	f=open("~/google-map-api-key.txt", "r")
#	api_key=f.readline()
#	google_maps = GoogleMaps(api_key)
#
#	#location = google_maps.search(location=address) # sends search to Google Maps.
#	location = google_maps.search() # sends search to Google Maps.
#
#	print(location.all()) # returns all locations.
#
#	my_location = location.first() # returns only first location.
#
#	print(my_location.city)
#	print(my_location.route)
#	print(my_location.street_number)
#	print(my_location.postal_code)

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
    logger.info( 'location detected: ' + location_city )

def publishPhoto():
    # Get a unique message from a list
    # add location with: whereAreMyFeet2()
    message = "I'm testing my tweet-feet camera code! " + tweetHashTags
    twitterPostPic( message, lastImageFile )
    logger.info( 'twitter publish: ' + lastImageFile )

def publishColor():
    message = "I tasted a color!  R:" + lastRed + "  G:" + lastGreen + "  B:" + lastBlue + " " + tweetHashTags
    # Generate a 4x4x4 cube with all the color names?
    # or a list of known color range values and other colors are just unknown.
    
    twitterPostText( message )
    logger.info( 'twitter publish: %s', message )

def checkInternetConnection():
    global myIP
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        socket.setdefaulttimeout(5)
        #socket.create_connection(('8.8.8.8',53))
        s.connect(('10.255.255.255', 1))
        haveNetwork = True
        logger.info( 'Network: connected to google' )
        #myIP = socket.getsockname()[0]
        myIP = s.getsockname()[0]
        recordIPAddress()
    except socket.error as msg:
        haveNetwork = False
        logger.critical( 'Network: ' + msg )
        IP = '127.0.0.1'
    finally:
        s.close()

def recordIPAddress():
    if ( haveNetwork ):
        # push our IP address to the website

        payload = {'key': ipkey, 'ip': myIP, 'mode': mode}

        r = requests.post("http://maehem.com/arsFoo", data=payload)
        #print(r.text)
        logger.info("IP recorder: %s %s", response.status_code, response.reason) #HTTP

#def get_ip():
#    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#    try:
#        # doesn't even have to be reachable
#        s.connect(('10.255.255.255', 1))
#        IP = s.getsockname()[0]
#    except:
#        IP = '127.0.0.1'
#    finally:
#        s.close()
#    return IP

def cameraInit():
    try:
        camera = PiCamera()
    except:
        logger.warning("No camera plugged in.")

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
logger.info("Pi serial port is: " + ser.name)

cameraInit()

checkInternetConnection()

twitterInit()

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
        logger.debug(word)	


        # was there even a command?
        if len(word) == 0:
            prompt()
            continue

        # if the command is 'snap' then call takePicture
        if word[0]=='mode':
            if len(word < 2):
                mode = "Unknown Mode"
                logger.warning("Unknown Mode registered!")
            else:
                mode = word[1]
                logger.info( "Mode is: %s", mode)
                recordIPAddress()
        if word[0]=='snap':
           takePicture()
        if word[0]=='taste':
            if len(word) > 1:
                taste(word[1], word[2], word[3], word[4])
            else:
                serial.write('error: no value for tasting!')
        if word[0]=='locate':
            whereAreMyFeet2()
        if word[0]=='log':
       	    # Write the rest of the line to the log file.
            logger.info("croissant {}".format(" ".join(word[-len(word)+1:])))

        if thereArePictures:
            thisHour = datetime.datetime.now().strftime('%H').format()
            if ( debugTwitPhoto or lastHour != thisHour ):
                lastHour = thisHour
                logger.info('twitter: attempt to publish...')
                publishPhoto()
                thereArePictures = bool(0)
        else:
            logger.debug( "No pics to tweet var={0}".format(thereArePictures) )
                
        if thereAreColors:
            thisHour = int(datetime.datetime.now().strftime('%H').format())
            thisMinute = int(datetime.datetime.now().strftime('%M').format())
            if ( lastHour != thisHour and thisMinute > 28 and thisMinute < 33 ):
                lastHour = thisHour
            logger.info('twitter: attempt to publish...')
            publishColor()
            thereAreColors = False

    prompt()
    # end while section

