import subprocess
import time

try:
	# for i in range(1, 36):
	# 	subprocess.call(["./main", "bioshock.jpg", "color", str(i)])
	# 	print("bioshock: " + str(i) + "/" + "35")
	# 	time.sleep(3)
	for i in range(8, 36):
		subprocess.call(["./main", "castle.jpg", "pb", str(i)])
		print("castle: " + str(i) + "/" + "35")
		time.sleep(3)
except:
	pass