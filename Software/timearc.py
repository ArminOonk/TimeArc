#!/usr/bin/python
import os
import time
import serial
import platform

if(platform.system() == "Windows"):
    print("Windows detected")

timeArcSerial = serial.Serial("COM5", 230400, timeout=1)

def sendCommand(command):
    if timeArcSerial.isOpen():
        command += "\r\n"
        timeArcSerial.write(command.encode())

def receiveData():
    try:
        response = timeArcSerial.readline()
    except serial.serialutil.SerialException:
        print("Serial exception, handle it!")

    txt = str(response.strip())
    if txt == "":
        print("Empty response")
        return
    else:
        print("Response: " + txt)
        vals = txt.split("=")

        if len(vals) == 2:
            print("First: " + vals[0] + " Second: " + vals[1])
            if vals[0] == "TIME":
                print("Received time from electronics")
        else:
            print("Wrong length")

if not timeArcSerial.isOpen():
    print("Serial port not open, opening ...")
    timeArcSerial.open()
    print("Opened")
else:
    print("Serial opened: " + timeArcSerial.name )

sendCommand("TIME?")
receiveData()

timeArcSerial.close()
