#!/usr/bin/python3
import os
import time
import serial
import platform
import calendar
import timearcSerial


taComm = timearcSerial.TimeArcSerial()
while int(time.time()) < 1404416633:
    print("Time not set yet: " + str(int(time.time())))
    time.sleep(1)
    
taComm.setTime()
time.sleep(1)
taComm.setOffset()
checkInterval = 60
nextCheckTime = int(time.time()) + 20

time.sleep(1)
while True:
    time.sleep(0.1)
    if int(time.time()) > nextCheckTime:
        taComm.sendCommand("TIME?")
        taComm.receiveData()
        nextCheckTime = int(time.time()) + checkInterval

    taComm.receiveData()

