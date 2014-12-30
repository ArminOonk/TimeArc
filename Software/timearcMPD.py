#!/usr/bin/python3
import mpd

# use_unicode will enable the utf-8 mode for python2
# see http://pythonhosted.org/python-mpd2/topics/advanced.html#unicode-handling
client = mpd.MPDClient()
client.connect("localhost", 6600)

client.clear()
client.add('test.mp3')
client.add("http://icecast.omroep.nl/3fm-bb-mp3")

for item in client.playlist():
	print("Item: " + item)
	
client.play()
print(client.currentsong())