#!/usr/bin/python
from threading import Timer
from datetime import datetime, timedelta
import math

import logging, logging.handlers
logger = logging.getLogger('TimeArc.Alarm')
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
	
	def setTime(self, timestamp):
		tz = datetime.fromtimestamp(timestamp)
		dt = self.diffSeconds(tz)
		if dt > 0:
			self.triggerTime = tz
			self.alarmSet = True
			self.update()

	def stopAlarm(self):
		self.alarmSet = False
		
	def update(self):		
		dt = self.diffSeconds(self.triggerTime)
		if self.alarmSet and dt < 0:
			self.stopAlarm()
			self.callback()
		
		timeRemaining = dt
		if dt > self.interval or dt <= 0:
			dt = self.interval
			
		logger.debug("Alarm update, time remaining: " + self.printCountDownTime(timeRemaining) + " checking again in: " + str(dt) + " seconds")

		self.alarmTimer = Timer(dt, self.update)
		self.alarmTimer.start()
		
	def __init__(self, callback, interValSec=60):
		self.interval = interValSec
		self.callback = callback
		self.alarmSet = False
		self.triggerTime = datetime.now()
		self.update()
		
	def diffSeconds(self, t):
		dt = t - datetime.now()
		return dt.total_seconds()
