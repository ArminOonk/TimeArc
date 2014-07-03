#!/usr/bin/python3
import os
import time
import serial
import platform
import calendar
import timearcSerial


taComm = timearcSerial.TimeArcSerial()

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

