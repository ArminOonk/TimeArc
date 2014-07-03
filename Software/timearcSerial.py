import serial
import time
import platform

class TimeArcSerial:
    serialPortName = "COM5"
    if(platform.system() == "Windows"):
        print("Windows detected")
        serialPortName = "COM5"
    elif (platform.system() == "Linux"):
        print("Linux detected")
        serialPortName = "/dev/ttyUSB0"

    startTime = int(time.time())

    try:
        serialPort = serial.Serial(serialPortName, 230400, timeout=1)
    except serial.SerialException:
        print("Error opening serial port")

    def __init__(self):
        pass

    def sendCommand(self, command):
        if self.serialPort.isOpen():
            print("Sending: " + command)
            command += "\r\n"
            self.serialPort.write(command.encode())


    def setTime(self):
        self.sendCommand("TIME="+str(int(time.time())))
        self.receiveData()

    def setOffset(self):
        offset = 0
        if time.daylight != 0:
            offset =  -time.altzone
        else:
            offset = -time.timezone

        print("Offset: " + str(offset))
        self.sendCommand("OFFSET="+str(offset))
        self.receiveData()

    def receiveData(self):
        if self.serialPort.inWaiting() > 0:
            try:
                response = self.serialPort.readline()
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
                        print("Computertime: " + str(int(time.time())) + " timearctime: " + str(timeArcTime))
                else:
                    print("Wrong length")