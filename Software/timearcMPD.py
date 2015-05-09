#!/usr/bin/python3
import musicpd

import time
from threading import Timer

import logging, logging.handlers
logger = logging.getLogger('TimeArc')
logger.setLevel(logging.DEBUG)

class TimeArcMPD:
	def __init__(self):
		self.targetVolume = 90
		self.currentVolume = 65
		self.maxIncrement = 2
		
		self.volumeInterval = 2	# interval in [sec]
		
		self.playList = []
		self.client = musicpd.MPDClient();
		self.update()
	
	# Keep the MPD connection alive
	def update(self):
		logger.debug("MPD keep the connection alive")
		self.CheckConnection()
		
		self.alarmTimer = Timer(30, self.update)
		self.alarmTimer.start()

	def CheckConnection(self):
		# Check the connection
		try:
			self.client.ping()
		except:
			logger.info(time.strftime("%H:%M:%S") + ": MPD reconnecting")
			self.client.connect("localhost", 6600)
			self.client.ping()
	
	def setVolume(self, vol):
		if vol > 100:
			vol = 100
		if vol < 0:
			vol = 0
			
		self.currentVolume = vol
		self.client.setvol(self.currentVolume)
		logger.debug("Volume: " + str(vol))
	
	def stop(self):
		self.client.stop()
	
	def clear(self):
		self.client.clear()
	
	def addPlayList(self, uri):
		self.playList.append(uri)
		
	def add(self, uri):
		try:
			self.client.add(uri)
		except musicpd.CommandError as e:
			logger.exception("timearcMPD add exception")
	
	def startPlayList(self, minVol=65, maxVol=80, inc=2):
		self.clear()
		for uri in self.playList:
			self.add(uri)
		
		self.stop()
		self.play(minVol, maxVol, inc)
		
	def play(self, minVol=65, maxVol=90, inc=2):
		self.targetVolume = maxVol
		self.currentVolume = minVol
		self.maxIncrement = inc
		
		self.setVolume(self.currentVolume)
		self.client.play()
		
		self.volumeTimer = Timer(self.volumeInterval, self.volumeUpdate)
		self.volumeTimer.start()
	
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
			logger.debug("Updated volume to: " + str(self.currentVolume) + " going to: " + str(self.targetVolume))

			self.volumeTimer = Timer(self.volumeInterval, self.volumeUpdate)
			self.volumeTimer.start()
		else:
			logger.debug("Target volume reached")
	
	def status(self):
		return self.client.status()
	
	def isPlaying(self):
		status = self.status()
		return status['state'] == 'play'
		
	def next(self):
		self.client.next()
		
	def previous(self):
		self.client.previous()
		
if __name__ == "__main__":
	print("MAIN debugging/testing")
	
	tam = TimeArcMPD()
	
	tam.addPlayList('http://pc192.pinguinradio.com:80')
	tam.addPlayList('http://pr320.pinguinradio.com:80')
	tam.addPlayList('http://po192.pinguinradio.com:80')
	tam.addPlayList('http://icecast.omroep.nl/3fm-bb-mp3')
	
	tam.startPlayList()