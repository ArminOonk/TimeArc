#!/usr/bin/python3
import os
import time
import serial
import platform
import calendar
import sys
import timearcSerial
import timearcAlarm
import timearcMPD
import timearcGoogleCalendar
from datetime import datetime, timedelta
from timearcPrivate import *
from threading import Timer

alarmTriggered = False
buttonTopTime = datetime.now()

#import exceptions
#from logging_utils import setup_logging_to_file, log_exception  
#setup_logging_to_file("timearc.py.txt")

# Logging
import logging, logging.handlers
logger = logging.getLogger('TimeArc')
logger.setLevel(logging.DEBUG)

# create formatter
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

# create console handler and set level to debug
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
# add formatter to ch
ch.setFormatter(formatter)

# add ch to logger
logger.addHandler(ch)

# Create an email logger
emailLogger = logging.handlers.SMTPHandler(mailhost=(reportSmtpHost, reportSmtpPort), fromaddr=reportingUser, toaddrs=reportingReceipients, subject='TimeArc errors', credentials= (reportingUser, reportingPassword), secure=(), timeout=20.0)
emailLogger.setLevel(logging.WARNING)
emailLogger.setFormatter(formatter)
logger.addHandler(emailLogger)

# Call backs
def alarmCallback():
	try:
		logger.info("Alarm callback")
		global alarmTriggered
		alarmTriggered = True
		
		tam.startPlayList()
			
		taComm.setAnimation()
	except:
		logger.exception('AlarmCallback')
		
def buttonCallback(button):
	try:
		global alarmTriggered
		global buttonTopTime
		
		if button == "TOP":
			buttonTopTime = datetime.now()
		
		if alarmTriggered and button == "TOP":
			logger.debug("Going back to clock mode")
			alarmTriggered = False
			taComm.setClock()
			
		if tam.isPlaying():
			if button == "UP":
				tam.volumeIncrement()
			elif button == "DOWN":
				tam.volumeDecrement()
			elif button == "LEFT":
				tam.previous()
			elif button == "RIGHT":
				tam.next()
			else:
				logger.debug("Button callback: " + button)
		else:
			logger.debug("Not playing Button: " + button)
	except:
		logger.exception('buttonCallback')

def accelCallback(accel):
	try:
		global  buttonTopTime
		dt = datetime.now() - buttonTopTime
		
		if accel == "DOUBLETAP":
			logger.debug("dt: " + str(dt.total_seconds()))
			if dt.total_seconds() < 2:
				logger.debug("Toggle pause")
				tam.pause()
		else:	
			logger.debug("Accel callback: " + accel)
	except:
		logger.exception('accelCallback')

def poseCallback(pose):
	try:
		if pose == "FRONT" and tam.isPlaying():
			logger.debug("toggle pause --> off")
			tam.pause()
		elif pose == "BACK" and not tam.isPlaying():
			logger.debug("toggle pause --> on")
			tam.pause()
		else:
			logger.debug("Pose changed to: " + pose + " doing nothing")
			
	except:
		logger.exception('poseCallback')

def googleCallback(time):
	try:
		if time == None:
			logger.info("Stopping the alarm, no event found")
			alarm.stopAlarm()
		else:
			logger.info("Google calendar callback, found event: " + str(time))
			tz = datetime.fromtimestamp(time)
			alarm.setTime(tz.hour, tz.minute)
	except:
		logger.exception('googleCallback')
	
# Objects
# Media player
tam = timearcMPD.TimeArcMPD()
tam.addPlayList('http://pc192.pinguinradio.com:80')
tam.addPlayList('http://pr320.pinguinradio.com:80')
tam.addPlayList('http://po192.pinguinradio.com:80')
tam.addPlayList('http://icecast.omroep.nl/3fm-bb-mp3')

# alarm
alarm = timearcAlarm.TimeArcAlarm(alarmCallback, 9, 5, 60)
# Serial port
taComm = timearcSerial.TimeArcSerial()	
# Google calendar interface
cal = timearcGoogleCalendar.TimeArcGoogleCalendar(FLOW, API_KEY, googleCallback)

while int(time.time()) < 1404416633:
    logger.info("Time not set yet: " + str(int(time.time())))
    time.sleep(1)

taComm.setAuto(True)
time.sleep(1)
taComm.setTime()
time.sleep(1)
taComm.setOffset()
time.sleep(1)
taComm.setAuto(False)
time.sleep(1)
taComm.setTimeout(15)
time.sleep(1)

taComm.setButtonCallback(buttonCallback)
taComm.setAccelCallback(accelCallback)
taComm.setPoseCallback(poseCallback)
taComm.receiveUpdate()

if len(sys.argv) > 1:
	for i in sys.argv:
		if i == "testalarm":
			logger.info("Testing alarm")
			alarmCallback()
			
		if i == "testexception":
			def broken_function():
				raise Exception('This is broken')
	
			try:
				logger.info('Testing Exception')
				broken_function()
			except:
				logger.exception('Exception test')
			
logger.info("Ready for action!")
	
while True:
    time.sleep(0.1)

