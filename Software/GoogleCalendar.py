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

FLAGS = gflags.FLAGS

# Set up a Flow object to be used if we need to authenticate. This
# sample uses OAuth 2.0, and we set up the OAuth2WebServerFlow with
# the information it needs to authenticate. Note that it is called
# the Web Server Flow, but it can also handle the flow for native
# applications
# The client_id and client_secret can be found in Google Developers Console
FLOW = OAuth2WebServerFlow(
    client_id='910625116461-fet78vjg1docm9vq3amjqk746sgmb59t.apps.googleusercontent.com',
    client_secret='S7cLzOX-TU8GXeiozS5UQars',
    scope='https://www.googleapis.com/auth/calendar',
    user_agent='TimeArc/1.0',
	)

# To disable the local server feature, uncomment the following line:
FLAGS.auth_local_webserver = False

# If the Credentials don't exist or are invalid, run through the native client
# flow. The Storage object will ensure that if successful the good
# Credentials will get written back to a file.
storage = Storage('calendar.dat')
credentials = storage.get()
if credentials is None or credentials.invalid == True:
	credentials = run(FLOW, storage)

# Create an httplib2.Http object to handle our HTTP requests and authorize it
# with our good Credentials.
http = httplib2.Http()
http = credentials.authorize(http)

# Build a service object for interacting with the API. Visit
# the Google Developers Console
# to get a developerKey for your own application.
service = build(serviceName='calendar', version='v3', http=http, developerKey='AIzaSyBrNYKWOzxV1NFN-TjhsP05ukHK2CQCmIE')

# Get Calendar summary
calendar = service.calendars().get(calendarId='primary').execute()
print('Calendar summary: ' + calendar['summary'])

# List events
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
