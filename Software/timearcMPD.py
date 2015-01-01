#!/usr/bin/python3
import mpd
import time
from threading import Timer
	
class TimeArcMPD:
	def __init__(self):
		self.targetVolume = 90
		self.currentVolume = 65
		self.maxIncrement = 2
		
		self.volumeInterval = 2	# interval in [sec]
		
		# use_unicode will enable the utf-8 mode for python2
		# see http://pythonhosted.org/python-mpd2/topics/advanced.html#unicode-handling
		self.client = mpd.MPDClient()
		self.client.connect("localhost", 6600)
		
	def setVolume(self, vol):
		self.currentVolume = vol
		self.client.setvol(self.currentVolume)
	
	def stop(self):
		self.client.stop()
		
	def clear(self):
		self.client.clear()

	def add(self, uri):
		self.client.add(uri)
	
	def play(self, minVol=65, maxVol=90, inc=2):
		self.targetVolume = maxVol
		self.currentVolume = minVol
		self.maxIncrement = inc
		
		self.setVolume(self.currentVolume)
		self.client.play()
		
		self.volumeTimer = Timer(self.volumeInterval, self.volumeUpdate)
		self.volumeTimer.start()
	
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
			
if __name__ == "__main__":
	print("MAIN debugging/testing")
	
	tam = TimeArcMPD()
	tam.add("http://po192.pinguinradio.com/listen.pls")
	tam.add("http://icecast.omroep.nl/3fm-bb-mp3")
	tam.add('test.mp3')
	
	tam.stop()
	tam.play(65, 90, 2)