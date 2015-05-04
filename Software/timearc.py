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
from logging_utils import setup_logging_to_file, log_exception  

setup_logging_to_file("timearc.py.txt")

# Call backs
def alarmCallback():
	try:
		print("Alarm callback")
		global alarmTriggered
		alarmTriggered = True
		tam.clear()
		tam.add('http://icecast.omroep.nl/3fm-bb-mp3')
		tam.add('test.mp3')
		
		tam.stop()
		tam.play(65, 80, 2)
		
		taComm.setAnimation()
	except Exception as e:
		log_exception(e)
		#tam = timearcMPD.TimeArcMPD()
		#alarmTimer = Timer(2.0, alarmCallback)
		#alarmTimer.start()
		
def buttonCallback(button):
	try:
		global alarmTriggered
		global  buttonTopTime
		
		if button == "TOP":
			buttonTopTime = datetime.now()
		
		if alarmTriggered and button == "TOP":
			print("Going back to clock mode")
			alarmTriggered = False
			taComm.setClock()
			
		if tam.isPlaying():
			if button == "UP":
				tam.volumeIncrement()
			elif button == "DOWN":
				tam.volumeDecrement()
			else:
				print("Button callback: " + button)
		else:
			print("Not playing Button: " + button)
	except exceptions.Exception as e:
		log_exception(e)

def accelCallback(accel):
	try:
		global  buttonTopTime
		dt = datetime.now() - buttonTopTime
		
		if accel == "DOUBLETAP":
			print("dt: " + str(dt.total_seconds()))
			if dt.total_seconds() < 2:
				print("Toggle pause")
				tam.pause()
		else:	
			print("Accel callback: " + accel)
	except exceptions.Exception as e:
		log_exception(e)

def googleCallback(time):
	try:
		if time == None:
			print("Stopping the alarm, no event found")
			alarm.stopAlarm()
		else:
			print("Google calendar callback, found event: " + str(time))
			tz = datetime.fromtimestamp(time)
			alarm.setTime(tz.hour, tz.minute)
	except exceptions.Exception as e:
		log_exception(e)
	
# Objects
# Media player
tam = timearcMPD.TimeArcMPD()
# alarm
alarm = timearcAlarm.TimeArcAlarm(alarmCallback, 9, 5, 60)
# Serial port
taComm = timearcSerial.TimeArcSerial()	
# Google calendar interface
cal = timearcGoogleCalendar.TimeArcGoogleCalendar(FLOW, API_KEY, googleCallback)

while int(time.time()) < 1404416633:
    print("Time not set yet: " + str(int(time.time())))
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
taComm.receiveUpdate()

print("Ready for action!")
while True:
    time.sleep(0.1)

