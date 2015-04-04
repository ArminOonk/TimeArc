import serial
import time
import sys
import platform
from threading import Timer

class TimeArcSerial:
	def __init__(self):
		serialPortName = "COM5"
		if platform.system() == "Windows":
			print("Windows detected")
			serialPortName = "COM5"
		elif (platform.system() == "Linux"):
			print("Linux detected")
			serialPortName = "/dev/ttyUSB0"

		startTime = int(time.time())
		
		try:
			self.serialPort = serial.Serial(serialPortName, 230400, timeout=1)
		except serial.SerialException:
			print("Error opening serial port")

	def receiveUpdate(self):		
		self.receiveData()
		
		self.recvTimer = Timer(0.1, self.receiveUpdate)
		self.recvTimer.start()
		
	def sendCommand(self, command):
		if self.serialPort.isOpen():
			print("Sending: " + command)
			command += "\r\n"
			self.serialPort.write(command.encode())

	def setTime(self):
		self.sendCommand("TIME="+str(int(time.time())))
		self.receiveData()

	def setOffset(self):
		if time.daylight == 1:
			offset = -(time.timezone-3600)
		else:
			offset = -time.timezone

		print("Offset: " + str(offset))
		self.sendCommand("OFFSET="+str(offset))
		self.receiveData()

	def setTimeout(self, timeout):
		command = "TIMEOUT="+str(timeout)
		print(command)
		self.sendCommand(command)
		self.receiveData()

	def setAuto(self, auto):
		if auto:
			command = "AUTO=TRUE"
		else:
			command = "AUTO=FALSE"

		print("Auto: " + command)
		self.sendCommand(command)
		self.receiveData()

	def setAnimation(self):
		self.sendCommand("MODE=ANIMATION")
		
	def setClock(self):
		self.sendCommand("MODE=CLOCK")
		
	def setButtonCallback(self, callback):
		self.buttonCallback = callback
	
	def setAccelCallback(self, callback):
		self.accelCallback = callback
		
	def receiveData(self):
		if self.serialPort.inWaiting() > 0:
			try:
				response = self.serialPort.readline()
			except serial.serialutil.SerialException:
				print("Serial exception, handle it!")

			if sys.hexversion >= 50463728:
				txt = str(response.strip(), "ascii")
			else:
				txt = str(response.strip())
				
			if txt == "":
				#print("Empty response")
				return
			else:
				self.decode(txt)
	
	def decode(self, txt):
		vals = txt.split("=")

		if len(vals) == 2:
			
			if vals[0] == "TIME":
				print("Received time from electronics")
				timeArcTime = int(vals[1])
				print("Computertime: " + str(int(time.time())) + " timearctime: " + str(timeArcTime))
			elif vals[0] == "BUTTON":
				if "buttonCallback" in dir(self):
					self.buttonCallback(vals[1])
				else:
					print("No callback found Button " + vals[1])
			elif vals[0] == "ACCEL":
				if "accelCallback" in dir(self):
					self.accelCallback(vals[1])
				else:
					print("No callback found Accel " + vals[1])
			else:
				print("First: " + vals[0] + " Second: " + vals[1])
		else:
			print("Response: " + txt)