# sys.path.append('C:/Users/zerok/Documents/timearc/Hardware/RPi')
from pcbnew import *
import math

scale = 1000000
angular_scale = 10

def show_reference():
	b = GetBoard()

	for module in b.GetModules():
		print(module.GetReference())

def circular_place(refs=None, x_center=55, y_center=54, radius=40):

	if refs == None:
		refs = ['U7', 'U8', 'U9', 'U10', 'U11', 'U12', 'U14', 'U15', 'U16', 'U17', 'U18', 'U19']
	
	b = GetBoard()
	
	mods = []
	
	for r in refs:
		m = b.FindModuleByReference(r)
		if m == None:
			print('Could not find reference: ' + r)
			return
		else:
			mods.append(m)
			print(m.GetReference() + ' Position: ' + str(m.GetPosition()) + ' orientation: ' + str(m.GetOrientation()))
	
	if len(mods) == 0:
		print('No modules found!')
	else:
		print('Loaded: ' + str(len(mods)) + ' modules')
	
	pos = mods[0].GetPosition()
	angle_increment = 360.0 / len(mods)
	angle = 0.0
	
	mods.append(mods[0])
	
	for m in mods:
		x = scale*(x_center + radius*math.cos(math.radians(angle)))
		y = scale*(y_center + radius*math.sin(math.radians(angle)))
		
		pos.x = int(x)
		pos.y = int(y)
	
		m.SetPosition(pos)
		m.SetOrientation(angular_scale*(-angle))
		
		print(m.GetReference() + " New (x,y): " + str(m.GetPosition()) + " angle: " + str(m.GetOrientation()))
		
		angle += angle_increment

print('arc loaded')