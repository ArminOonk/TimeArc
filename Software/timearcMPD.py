#!/usr/bin/python3
import musicpd

import time
from threading import Timer
	
class TimeArcMPD:
	def __init__(self):
		self.targetVolume = 90
		self.currentVolume = 65
		self.maxIncrement = 2
		
		self.volumeInterval = 2	# interval in [sec]
		
		self.client = musicpd.MPDClient();
		self.update()
	
	# Keep the MPD connection alive
	def update(self):
		print("MPD keep the connection alive")
		self.CheckConnection()
		
		self.alarmTimer = Timer(30, self.update)
		self.alarmTimer.start()
		
	#decorator to check the connection
	# http://hangar.runway7.net/python/decorators-and-wrappers
	# http://simeonfranklin.com/blog/2012/jul/1/python-decorators-in-12-steps/

	def CheckConnection(self):
		# Check the connection
		try:
			self.client.ping()
		except musicpd.ConnectionError:
			print(time.strftime("%H:%M:%S") + ": MPD reconnecting")
			self.client.connect("localhost", 6600)
			self.client.ping()

	#@CheckConnection	
	def setVolume(self, vol):
		if vol > 100:
			vol = 100
		if vol < 0:
			vol = 0
			
		self.currentVolume = vol
		self.client.setvol(self.currentVolume)
		print("Volume: " + str(vol))
	
	#@CheckConnection
	def stop(self):
		self.client.stop()
	
	#@CheckConnection
	def clear(self):
		#self.isConnected()
		self.client.clear()
		
	#@CheckConnection
	def add(self, uri):
		try:
			self.client.add(uri)
		except musicpd.CommandError as e:
			print("Exception: " + str(e))
	
	#@CheckConnection
	def play(self, minVol=65, maxVol=90, inc=2):
		self.targetVolume = maxVol
		self.currentVolume = minVol
		self.maxIncrement = inc
		
		self.setVolume(self.currentVolume)
		self.client.play()
		
		self.volumeTimer = Timer(self.volumeInterval, self.volumeUpdate)
		self.volumeTimer.start()
	
	#@CheckConnection
	def pause(self):
		self.client.pause()
		
	def volumeIncrement(self):
		self.targetVolume = self.currentVolume + self.maxIncrement
		self.setVolume(self.targetVolume)
		
	def volumeDecrement(self):
		self.targetVolume = self.currentVolume - self.maxIncrement
		self.setVolume(self.targetVolume)
		
	def volumeUpdate(self):
		if self.targetVolume != self.currentVolume:
			inc = 0
			if self.targetVolume > self.currentVolume:
				inc = self.targetVolume - self.currentVolume
			else:
				inc = -(self.currentVolume - self.targetVolume)
			
			if inc > self.maxIncrement:
				inc = self.maxIncrement
			if inc < -self.maxIncrement:
				inc = -self.maxIncrement
			
			self.setVolume(self.currentVolume + inc)
			print("Updated volume to: " + str(self.currentVolume) + " going to: " + str(self.targetVolume))

			self.volumeTimer = Timer(self.volumeInterval, self.volumeUpdate)
			self.volumeTimer.start()
		else:
			print("Target volume reached")
	
	#@CheckConnection
	def status(self):
		return self.client.status()
	
	#@CheckConnection
	def isPlaying(self):
		status = self.status()
		return status['state'] == 'play'
		
if __name__ == "__main__":
	print("MAIN debugging/testing")
	
	tam = TimeArcMPD()
	tam.clear()
	#tam.add('http://po192.pinguinradio.com/listen.pls')
	tam.add('http://icecast.omroep.nl/3fm-bb-mp3')
	tam.add('test.mp3')
	
	tam.stop()
	tam.play(65, 90, 2)