#!/usr/bin/python3
import os
import time
import serial
import platform
import calendar
import timearcSerial
import timearcAlarm
import timearcMPD

# Call backs
def alarmCallback():
	print("Alarm callback")
	
	tam.clear()
	tam.add('http://po192.pinguinradio.com/listen.pls')
	tam.add('http://icecast.omroep.nl/3fm-bb-mp3')
	tam.add('test.mp3')
	
	tam.stop()
	tam.play(65, 90, 2)

# Objects
tam = timearcMPD.TimeArcMPD()
alarm = timearcAlarm.TimeArcAlarm(9, 00, alarmCallback, 60)
taComm = timearcSerial.TimeArcSerial()	

while int(time.time()) < 1404416633:
    print("Time not set yet: " + str(int(time.time())))
    time.sleep(1)

taComm.setAuto(True)
time.sleep(1)
taComm.setTime()
time.sleep(1)
taComm.setOffset()
time.sleep(1)
taComm.setTimeout(10)
time.sleep(1)
taComm.setAuto(False)
time.sleep(1)
taComm.setTimeout(10)
time.sleep(1)

checkInterval = 60
nextCheckTime = int(time.time()) + 20

time.sleep(1)

taComm.receiveUpdate()

while True:
    time.sleep(0.1)
    #if int(time.time()) > nextCheckTime:
    #    taComm.sendCommand("TIME?")
    #    taComm.receiveData()
    #    nextCheckTime = int(time.time()) + checkInterval

    #taComm.receiveData()

