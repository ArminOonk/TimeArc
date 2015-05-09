#!/usr/bin/python
from threading import Timer
from datetime import datetime, timedelta
import math

import logging, logging.handlers
logger = logging.getLogger('TimeArc')
logger.setLevel(logging.DEBUG)

class TimeArcAlarm:

	def printCountDownTime(self, secRemaining):
		secRemaining = round(secRemaining)
		# Seconds remaining
		seconds = math.floor(secRemaining%60)
		secRemaining = secRemaining/60
		# Minutes remainging
		minutes = math.floor(secRemaining%60)
		secRemaining = secRemaining/60
		# Hours Remaining
		hours = math.floor(secRemaining%24)
		secRemaining = secRemaining/24
		# Days remaining
		days = math.floor(secRemaining)
		
		retStr = "{:0>2d}".format(int(hours)) + ":" + "{:0>2d}".format(int(minutes)) + ":" + "{:0>2d}".format(int(seconds))
		if days > 0:
			retStr = str(days) + " days " + retStr
		
		return retStr
	
	def setTime(self, hour, minute):
		tz = datetime.now()
		newTime = tz.replace(hour=hour, minute=minute,second=0,microsecond=0)
		dt = self.diffSeconds(newTime)

		if dt <= 0:
			newTime = newTime + timedelta(days=1)
		self.triggerTime = newTime
		self.alarmSet = True
		self.update()
	
	def stopAlarm(self):
		self.alarmSet = False
		
	def update(self):
		if not self.alarmSet:
			logger.debug("Alarm not set")
			return # Alarm not set
			
		dt = self.diffSeconds(self.triggerTime)
		if dt < 0:
			self.callback()
			return
		
		interval = dt
		if dt > self.interval:
			interval = self.interval
			
		logger.debug("Alarm update, time remaining: " + self.printCountDownTime(dt) + " checking again in: " + str(interval) + " seconds")
				
		self.alarmTimer = Timer(interval, self.update)
		self.alarmTimer.start()
		
	def __init__(self, callback, hour, minute, interValSec=60):
		self.interval = interValSec
		self.callback = callback
		
		self.setTime(hour, minute)
		logger.debug("Trigger time: " + str(self.triggerTime))
		
	def diffSeconds(self, t):
		dt = t - datetime.now()
		return dt.total_seconds()
