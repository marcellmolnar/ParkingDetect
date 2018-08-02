import cv2
import numpy as np

from drawAndPrint import draw_rectangle_on_image
from parking_lot import Parking_lot
from utils import calcNonZeroPixels

H,W = 720, 1280

parking_zone_up = [(80,272), (147,257), (185,258), (251,249), (309,249), (378,245), (465,241), (560,241), (653,238), (769,233), (867,240), (948,249), (1016,247)]

'''[(95,241), (154,240), (266,237), (357,228), (457,226), (564,226), (651,221), (736,223), (847,223), (956,229), (1012,229), (1019,229)]
'''

parking_zone_down = [(66,284), (90,287), (126,283), (152,282), (182,281), (226,282), (263,279), (301,281), (358,281), (412,277), (479,277), (556,276), (609,272), (662,271), (735,273), (802,273), (887,278), (971,282), (1021,280)]

'''[(49,291), (91,293), (138,296), (193,296), (255,291), (331,291), (405,292), (494,291), (614,288), (736,288), (836,295), (924,295), (1019,296), (1027,296)]
'''

def getRect(x, width):
	scale = 1-0.8*(1030-x)/(1030-60)

	slide = (-30*(1030-x)/(1030-60))/2

	index = 1
	while index < len(parking_zone_down)-1 and parking_zone_down[index][0] < x-width/2+slide: index += 1
	rate2 = float((parking_zone_down[index][0] - x+width/2-slide)) / (parking_zone_down[index][0] - parking_zone_down[index-1][0])
	corner2 = (x-width/2+slide, int(parking_zone_down[index][1] + (parking_zone_down[index-1][1] - parking_zone_down[index][1])*rate2))

	while index < len(parking_zone_down)-1 and parking_zone_down[index][0] < x+width/2+slide: index += 1
	rate3 = float((parking_zone_down[index][0] - x-width/2-slide)) / (parking_zone_down[index][0] - parking_zone_down[index-1][0])
	corner3 = (x+width/2+slide, int(parking_zone_down[index][1] + (parking_zone_down[index-1][1] - parking_zone_down[index][1])*rate3))


	index = 1
	while index < len(parking_zone_up)-1 and parking_zone_up[index][0] < x-width/2-slide: index += 1
	rate1 = float((parking_zone_up[index][0] - x+width/2+slide)) / (parking_zone_up[index][0] - parking_zone_up[index-1][0])
	corner1 = (x-width/2-slide, int(parking_zone_up[index][1] + (parking_zone_up[index-1][1] - parking_zone_up[index][1])*rate1))

	while index < len(parking_zone_up)-1 and parking_zone_up[index][0] < x+width/2-slide: index += 1
	rate4 = float((parking_zone_up[index][0] - x-width/2+slide)) / (parking_zone_up[index][0] - parking_zone_up[index-1][0])
	corner4 = (x+width/2-slide, int(parking_zone_up[index][1] + (parking_zone_up[index-1][1] - parking_zone_up[index][1])*rate4))
	rect = (corner1, corner2, corner3, corner4)
	return rect



maskRectangles = []
canRefresh = []

currX = 950
while currX > 60:
	rect = getRect(currX, 50)
	mask = np.zeros((H,W), np.uint8)
	rectangle = np.array(rect, np.int32)
	cv2.fillConvexPoly(mask, rectangle, 255)
	maskRectangles.append(mask)
	canRefresh.append(False)
	currX -= 10

percentages=((950-60)/10)*[0]
percentagesToDraw=((950-60)/10)*[10]

def calcPercentages(thresh, diffImage):
	currX = 950
	i = 0
	newPercentages = ((950-60)/10)*[0]
	while currX > 60:
		percentage = calcNonZeroPixels(thresh, mask = maskRectangles[i], percentage = True)
		percentage_of_moving_objects = calcNonZeroPixels(diffImage, mask = maskRectangles[i], percentage = True)
		newPercentages[i] = percentage
		if percentage_of_moving_objects < 5:
			canRefresh[i] = True
		else:
			canRefresh[i] = False
		currX -= 10
		i += 1
	for i in range(0,(950-60)/10):
		refresh = True
		if (i>0 and canRefresh[i-1] == False): refresh = False
		if (i>1 and canRefresh[i-2] == False): refresh = False
		if (i>2 and canRefresh[i-3] == False): refresh = False
		if (i>3 and canRefresh[i-4] == False): refresh = False

		if (i<len(canRefresh)-1 and canRefresh[i+1] == False): refresh = False
		if (i<len(canRefresh)-2 and canRefresh[i+2] == False): refresh = False
		if (i<len(canRefresh)-3 and canRefresh[i+3] == False) : refresh = False
		if (i<len(canRefresh)-4 and canRefresh[i+4] == False): refresh = False

		if refresh:
			percentages[i] = newPercentages[i]
	pass

######################################################################################################################################

def calcParkinglotsStatus(image):
	p = percentages[:]
	i=1
	# averaging
	while i < len(p)-1:
		percentagesToDraw[i] = (p[i-1] + p[i] + p[i+1]) / 3
		i += 1
		percentagesToDraw[i] = p[i]
	#print str(percentagesToDraw)
	parkingCarsNow = []
	parkingCarsX = []
	last_percentage = 100
	ascending = False
	currX = 950
	i = 0
	last_rect_position = 100
	position = 1
	while currX > 60:
		scale = 1-0.80*(950-currX)/(950-60)
		percentage = percentagesToDraw[i]
		if percentage < last_percentage and ascending:
			ascending = False
			if last_percentage > 15 and last_rect_position > 70*scale:
				width = int(80*scale)
				rect = getRect(currX, width)
				position += 1
				last_rect_position = 0
				parkingCarsNow.append(Parking_lot(rect))
				parkingCarsX.append(currX)
				cv2.circle(image,(currX+10,500-last_percentage*3), 4, (0,255,255),4)

		if percentage >= last_percentage:
			ascending = True
		last_percentage = percentage
		currX -= 10
		i += 1
		last_rect_position += 10
	return [parkingCarsNow, parkingCarsX]

def drawStatisticsOnImage(image):
	minY = 500
	maxY = 200
	scale = (minY-maxY)/100
	gridHeight = (minY-maxY)/5
	currX = 940
	last_percentage = percentagesToDraw[0]
	avg = 0
	for i in range(len(percentagesToDraw)):
                if i > 0:
                        percentage = percentagesToDraw[i]
                        percentage2 = percentages[i]
                        avg += percentage
                        if last_percentage < percentage:
                                color = (255,0,0)
                        elif last_percentage > percentage:
                                color = (0,0,255)
                        else:
                                color = (0,255,0)
                        if (currX%20)==0:
                                color = (0,255,0)
                        else:
                                color = (0,0,255)
                        cv2.line(image,(currX+10,minY-last_percentage*scale),(currX,minY-percentage*scale),color,2)
                        last_percentage = percentage
                        currX -= 10
	for i in range(1,10):
		cv2.line(image,(950-i*100,maxY),(950-i*100,minY),(255,255,255), 1)
		if (950-i*100 < 100): space = ' '
                else: space = ''
                cv2.putText(image, space+str(950-i*100), (950-i*100-30,minY+40), 1, 2, (0,255,255), 3)
		
	# vertical axis
	cv2.line(image,(50,maxY),(50,minY),(255,0,0),2)
	cv2.line(image,(40,maxY+20),(50,maxY),(255,0,0),2)
	cv2.line(image,(60,maxY+20),(50,maxY),(255,0,0),2)
	cv2.putText(image, 'Percentage [%]', (40,maxY-20), 1, 2, (0,255,255), 3)
	for i in range(0,5):
		cv2.line(image,(50,minY-i*gridHeight),(950,minY-i*gridHeight),(255,255,255),1)
                if (i==0): space = ' '
                else: space = ''
                cv2.putText(image, space+str(i*20), (5,minY-i*gridHeight+10), 1, 2, (0,255,255), 3)
		
	# horizontal axis
	cv2.line(image,(50,minY),(950,minY),(255,0,0),2)
	cv2.line(image,(940,minY-10),(950,minY),(255,0,0),2)
	cv2.line(image,(940,minY+10),(950,minY),(255,0,0),2)
	cv2.putText(image, 'X coord.', (930,minY+40), 1, 2, (0,255,255), 3)
	pass


parkingCarsLast = []
lastSeen = []

def calibrateSpots(image):
	global parkingCarsLast
	[parkingCarsNew, parkingCarsX] = calcParkinglotsStatus(image)
	if len(parkingCarsLast) == 0:
		for i in range(len(parkingCarsNew)):
			parkingCarsLast.append(parkingCarsNew[i])
			lastSeen.append(0)
	else:
		# list new cars
		isNewCar = []
		for i in range(len(parkingCarsNew)):
			isNewCar.append(True)

		# match new cars with existing ones
		for i in range(len(parkingCarsLast)):
			match = False
			for j in range(len(parkingCarsNew)):
				if parkingCarsLast[i].distanceFrom(parkingCarsNew[j]) < 40:
					match = True
					lastSeen[i] = 0
					isNewCar[j] = False
			if match == False:
				lastSeen[i] += 1
		
		# Put in the new cars
		for i in range(len(parkingCarsNew)):
			if isNewCar[i] == True:
				parkingCarsLast.append(parkingCarsNew[i])
				lastSeen.append(0)
		
		# Throw out cars that is not parking anymore
		indexesToDel=[]
		for i in range(len(parkingCarsLast)):
			if lastSeen[i] > 1:
				indexesToDel.append(i)

		for i in range(len(indexesToDel)):
			index = indexesToDel[i]
			parkingCarsLast[index].throwOut(index)
			del parkingCarsLast[index]
			del lastSeen[index]

	currX = 950
	parkingLotIndex = 0
	for i in range(len(parkingCarsX)):
		scale = 1-0.80*(950-currX)/(950-60)
		width = int(80*scale)
		distBetweenCarAndSpot = 50*( 1-0.70*(950-currX)/(950-60) )
		while parkingCarsX[i] < currX-width/2-distBetweenCarAndSpot:
			scale = 1-0.80*(950-currX)/(950-60)
			position = int(currX)
			width = int(80*scale)
			rect = getRect(position, width)
			draw_rectangle_on_image(image,rect,(0,255,0))
			center = (Parking_lot(rect)).center()
			center = (center[0]-10, center[1]+10)
			#cv2.putText(image, ''+str(parkingLotIndex), center, 1, 2, (0,0,255),2)
			parkingLotIndex += 1
			currX -= int(120*scale)
			distBetweenCarAndSpot = 50*( 1-0.70*(950-currX)/(950-60) )

		center = (parkingCarsNew[i]).center()
		center = (center[0]-10, center[1]+10)
		#cv2.putText(image, ''+str(parkingLotIndex), center, 1, 2, (0,0,255),2)
		parkingLotIndex += 1

		currX = parkingCarsX[i]-width*3/5-distBetweenCarAndSpot

	scale = 1-0.80*(950-currX)/(950-60)
	width = int(80*scale)
	distBetweenCarAndSpot = 50*( 1-0.70*(950-currX)/(950-60) )
	while 50 < currX-width/2-distBetweenCarAndSpot:
			scale = 1-0.80*(950-currX)/(950-60)
			position = int(currX)
			width = int(80*scale)
			rect = getRect(position, width)
			draw_rectangle_on_image(image,rect,(0,255,0))
			center = (Parking_lot(rect)).center()
			center = (center[0]-10, center[1]+10)
			#cv2.putText(image, ''+str(parkingLotIndex), center, 1, 2, (0,0,255),2)
			parkingLotIndex += 1
			currX -= int(120*scale)
			distBetweenCarAndSpot = 50*( 1-0.70*(950-currX)/(950-60) )

	for i in range(len(parkingCarsLast)):
		draw_rectangle_on_image(image, (parkingCarsLast[i]).getRect(), (0,0,255))

	drawStatisticsOnImage(image)
	pass


def drawOnRectangles(image, start, diff):
        currX = 950-start*10
	i = start
	while currX > 60:
                rect = getRect(currX, 50)
                currX -= 10*diff
		draw_rectangle_on_image(image,rect,(0,0,255))
                i += diff

