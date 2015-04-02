#!/usr/bin/python
import gflags
import httplib2

from apiclient.discovery import build
from oauth2client.file import Storage
from oauth2client.client import OAuth2WebServerFlow
from oauth2client.tools import run

import xe #for the time comparator
from feed.date.rfc3339 import tf_from_timestamp #also for the comparator
from feed.date.rfc3339 import timestamp_from_tf #also for the comparator

from datetime import datetime #for the time on the rpi end
import time

class TimeArcGoogleCalendar:
	def __init__(self, FLOW, API_KEY):
		self.FLOW = FLOW
		self.API_KEY = API_KEY
		self.FLAGS = gflags.FLAGS
		self.FLAGS.auth_local_webserver = False
		self.credentialsReceived = False
		
		self.getCredentials() # Maybe not the best place, will do for now
		
	def getCredentials(self):
		self.storage = Storage('calendar.dat')
		self.credentials = self.storage.get()
		if self.credentials is None or self.credentials.invalid == True:
			self.credentials = run(FLOW, storage)
		self.credentialsReceived = True
	
	def getNextAlarm(self):
		# Create an httplib2.Http object to handle our HTTP requests and authorize it
		# with our good Credentials.
		http = httplib2.Http()
		http = self.credentials.authorize(http)
		service = build(serviceName='calendar', version='v3', http=http, developerKey=self.API_KEY)
		
		page_token = None
		timeNow = timestamp_from_tf(time.time())
		timeMax = timestamp_from_tf(time.time()+60*60*24)
		print("Now: " + timeNow + " max: " + timeMax)

		while True:
			events = service.events().list(calendarId='primary', pageToken=page_token, q='wake', timeMin=timeNow, timeMax=timeMax).execute()
			for event in events['items']:
				eventTime = tf_from_timestamp(event['start']['dateTime'])
				eventTimeString = time.strftime('%d-%m-%Y %H:%M',time.localtime(eventTime))
				
				delta = datetime.fromtimestamp(eventTime) - datetime.now()
				
				print(event['summary'] + " " + eventTimeString + " happening in: " + str(delta.total_seconds()) + " seconds")
				print(timestamp_from_tf(eventTime))
				
			page_token = events.get('nextPageToken')
			if not page_token:
				print("Done")
				break

if __name__ == "__main__":
	from timearcPrivate import *
	print("Google calendar testing and debugging")
	cal = TimeArcGoogleCalendar(FLOW, API_KEY)
	cal.getNextAlarm()