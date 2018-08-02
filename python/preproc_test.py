import collections
import cv2
import datetime
import numpy as np
import os
import platform
import sys

from parking_lot import Parking_lot
from drawAndPrint import *
from utils import *

H,W = 720, 1280

steppedPlay=False

if len(sys.argv) < 2:
	print 'Usage: "python deviation.py 2"'

videoIndex = sys.argv[1]
v="../vids/"+str(videoIndex)+".mp4"
vidcap = cv2.VideoCapture(v)

ret, im = vidcap.read()
image = np.array(im)

last_image = image
last_time = 0

r = ((522,272), (560,299), (835,307), (835,280))
asphalt = Parking_lot(r)

maskAsphalt = np.zeros((H,W), np.uint8)
rectangleA = np.array([asphalt.corner1,asphalt.corner2,asphalt.corner3,asphalt.corner4], np.int32)
cv2.fillConvexPoly(maskAsphalt, rectangleA, 255)

meanAsList = collections.deque(20*[0], 20)
meanAs = 0
cnt=0

background = cv2.imread('../bg.jpg')
background_gray = cv2.cvtColor(background, cv2.COLOR_RGB2GRAY)

while(True):
	clear_screen()

	ret, im = vidcap.read()
	if(ret == False): break

	image = np.array(im)
	imageGray = cv2.cvtColor(np.array(im), cv2.COLOR_RGB2GRAY)
	
	diff = getDiffImageInGray(image, last_image)
	last_image = image.copy()

	nonZero = calcNonZeroPixels(diff, maskAsphalt, percentage = False)

        meanAsNow = meanOfArea(imageGray, maskAsphalt)
	if nonZero < 100:
		if cnt < 20:
			meanAsList.appendleft(meanAsNow)
			meanAs *= cnt
			meanAs += meanAsNow
			meanAs /= (cnt+1)
			cnt += 1
		elif meanAs - meanAsNow < 10 and meanAsNow - meanAs < 10 :
			popped = int(meanAsList.pop())
			meanAsList.appendleft(meanAsNow)
			meanAs *= 20
			meanAs -= popped
			meanAs += meanAsNow
			meanAs /= 20

	#diffFromBlack_thresh = filterBlack(image, imageGray, meanAs/2)
	#print str(meanAs*3/4)

	diffFromBackground  = cv2.absdiff(imageGray, background_gray)
	_, diffFromBackground_thresh = cv2.threshold(diffFromBackground,140,255,cv2.THRESH_BINARY)

	'''
	constantBackground = np.zeros((H,W), np.uint8)
	constantBackground += int(meanAs/2)
	diffFromConstantBackground = cv2.absdiff(imageGray, constantBackground)
	
	_, diffFromConstantBackground_thresh = cv2.threshold(diffFromConstantBackground,70,255,cv2.THRESH_BINARY)

	
	dt = datetime.datetime.now().time()
	time_now = dt.hour
	time_now = 18
	if 20 < time_now or time_now < 5:
		thresh = thresh2
	else:
		thresh = thresh1
	thresh = thresh4

	if counter == 1: 
		calcStatistics(thresh, diff)
	calibrateSpots(image)

	dt = datetime.datetime.now().time()
	time_now = dt.minute*60*950 + dt.second*950 + dt.microsecond/950
	if time_now != last_time:
		fps = 950/(time_now-last_time)
	else:
		fps = 0
	last_time = time_now
	print "fps: "+format_string(fps)
	

	draw_rectangle_on_image(image, asphalt.getRect(), (0,0,255))
	'''
	image = cv2.resize(image,(640,360))
	cv2.imshow('original', image)
	cv2.moveWindow("original", 0,0)

	thresh2show = cv2.resize(diffFromBackground_thresh,(640,360))
	cv2.imshow('difference from background th', thresh2show)
	cv2.moveWindow("difference from background th", 700,0)

	thresh2show = cv2.resize(diffFromBackground,(640,360))
	cv2.imshow('difference from background', thresh2show)
	cv2.moveWindow("difference from background", 0,370)

	'''thresh2show = cv2.resize(diffFromBlack_thresh,(640,360))
	cv2.imshow('shadow based', thresh2show)
	cv2.moveWindow("shadow based", 700,370)'''

	'''_, imageGray_thresh = cv2.threshold(imageGray,70,255,cv2.THRESH_BINARY)
	thresh2show = cv2.resize(imageGray_thresh,(640,360))
	cv2.imshow('shadow based', thresh2show)
	cv2.moveWindow("shadow based", 700,370)'''

	k = cv2.waitKey(30)&0xff
	if k == 27:
		break;
	if k == 32 or steppedPlay:
		close = False
		while(True):
			k = cv2.waitKey(30)&0xff
			if k == 32:
				steppedPlay = False
				break;
			if k == 27:
				close = True
				break;
			if k == ord('d'):
				steppedPlay = True
				break;
			if k == ord('s'):
				cv2.imwrite('image.png',image)
		if(close):
			break;


cv2.destroyAllWindows()
