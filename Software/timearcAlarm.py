#!/usr/bin/python3
from threading import Timer

class TimeArcAlarm:

	def update(self):
		print("TimerArcThread update")
		alarmTimer = Timer(self.interval, self.update)
		alarmTimer.start()
		
	def __init__(self, interValSec):
		self.interval = interValSec
		self.update()
		pass
		

	