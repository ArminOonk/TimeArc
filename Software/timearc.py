#!/usr/bin/python
import os
import time
import serial
import platform
import calendar

if(platform.system() == "Windows"):
    print("Windows detected")

timeArcSerial = serial.Serial("COM5", 230400, timeout=1)

def sendCommand(command):
    if timeArcSerial.isOpen():
        print("Sending: " + command)
        command += "\r\n"
        timeArcSerial.write(command.encode())

def receiveData():
    try:
        response = timeArcSerial.readline()
    except serial.serialutil.SerialException:
        print("Serial exception, handle it!")

    txt = str(response.strip(), "ascii")
    if txt == "":
        #print("Empty response")
        return
    else:
        print("Response: " + txt)
        vals = txt.split("=")

        if len(vals) == 2:
            print("First: " + vals[0] + " Second: " + vals[1])
            if vals[0] == "TIME":
                print("Received time from electronics")
                timeArcTime = int(vals[1])
                diffTime = int(time.time()) - timeArcTime
                timeSinceStart = int(time.time())-startTime
                print("timeArcTime: " + str(timeArcTime) + " computerTime: " + str(int(time.time())) + " diff: " + str(diffTime))
                print("StartTime: " + str(startTime) + " seconds since start: " + str(timeSinceStart) + " Offset promil: " + str((diffTime/timeSinceStart)*1000.0) )

        else:
            print("Wrong length")

def setTime():
    sendCommand("TIME="+str(int(time.time())))

def setOffset():
    offset = 0
    if time.daylight != 0:
        offset =  -time.altzone
    else:
        offset = -time.timezone

    print("Offset: " + str(offset))
    sendCommand("OFFSET="+str(offset))

if not timeArcSerial.isOpen():
    print("Serial port not open, opening ...")
    timeArcSerial.open()
    print("Opened")
else:
    print("Serial opened: " + timeArcSerial.name )

setTime()
setOffset()
startTime = int(time.time())
checkInterval = 60
nextCheckTime = int(time.time()) + 20

time.sleep(1)
while True:
    time.sleep(0.1)
    if int(time.time()) > nextCheckTime:
        sendCommand("TIME?")
        receiveData()
        nextCheckTime = int(time.time()) + checkInterval

    if timeArcSerial.inWaiting() > 0:
        receiveData()

timeArcSerial.close()
