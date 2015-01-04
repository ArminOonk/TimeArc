#!/usr/bin/python3
from threading import Timer
from datetime import datetime, timedelta
import math

class TimeArcAlarm:

	def printCountDownTime(self, secRemaining):
		secRemaining = round(secRemaining)
		# Seconds remaining
		seconds = round(secRemaining%60)
		secRemaining = secRemaining/60
		# Minutes remainging
		minutes = round(secRemaining%60)
		secRemaining = secRemaining/60
		# Hours Remaining
		hours = round(secRemaining%24)
		secRemaining = secRemaining/24
		# Days remaining
		days = math.floor(secRemaining)
		
		retStr = "{:0>2d}".format(hours) + ":" + "{:0>2d}".format(minutes) + ":" + "{:0>2d}".format(seconds)
		if days > 0:
			retStr = str(days) + " days " + retStr
		
		return retStr
		
	def update(self):
		dt = self.diffSeconds(self.triggerTime)
		if dt < 0:
			self.callback()
			return
		
		interval = dt
		if dt > self.interval:
			interval = self.interval
			
		print("Alarm update, time remaining: " + self.printCountDownTime(dt) + " checking again in: " + str(interval) + " seconds")
				
		self.alarmTimer = Timer(interval, self.update)
		self.alarmTimer.start()
		
	def __init__(self, hour, minute, callback, interValSec=1):
		self.interval = interValSec
		self.callback = callback
		
		tz = datetime.now()
		newTime = tz.replace(hour=hour, minute=minute,second=0,microsecond=0)
		dt = self.diffSeconds(newTime)

		if dt <= 0:
			newTime = newTime + timedelta(days=1)
			
		print("Trigger time: " + str(newTime))
		self.triggerTime = newTime
		self.update()
		
	def diffSeconds(self, t):
		dt = t - datetime.now()
		return dt.total_seconds()
