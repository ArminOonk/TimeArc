#!/usr/bin/python
import os
import time
import serial
import platform

if(platform.system() == "Windows"):
    print("Windows detected")

timeArcSerial = serial.Serial("COM5", 230400, timeout=1)

if not timeArcSerial.isOpen():
    print("Serial port not open, opening ...")
    timeArcSerial.open()
    print("Opened")
else:
    print("Serial opened: " + timeArcSerial.name )

timeArcSerial.write("TIME?\r\n".encode())
time.sleep(1)
response = timeArcSerial.readline()
print("Response: " + str(response).strip())

timeArcSerial.close()
