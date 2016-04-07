from pcbnew import *
b = GetBoard()

for module in b.GetModules():
    print(module.GetReference())
	
