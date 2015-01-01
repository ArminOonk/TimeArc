#!/usr/bin/python3
from threading import Timer
from datetime import datetime, timedelta

class TimeArcAlarm:

	def update(self):
		dt = self.diffSeconds(self.triggerTime)
		if dt < 0:
			print("Alarm triggered!")
			return
		
		interval = dt
		if dt > self.interval:
			interval = self.interval
			
		print("Alarm update, diff [sec]: " + str(dt) + " checking again in: " + str(interval))
				
		self.alarmTimer = Timer(interval, self.update)
		self.alarmTimer.start()
		
	def __init__(self, hour, minute, interValSec=1):
		self.interval = interValSec
		
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
