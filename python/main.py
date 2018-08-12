import cv2
import datetime
import numpy as np
import os
import platform
import sys

from carDetect import *
from drawAndPrint import *
from parking_lot import Parking_lot
from utils import *

H,W = 720, 1280

#####################################################################################################################

steppedPlay=False

if len(sys.argv) < 2:
	print 'Usage: "python deviation.py 2"'

videoIndex = sys.argv[1]
v="C:/Users/Marci/Desktop/Smart City/Park Detect//vids/"+str(videoIndex)+".mp4"
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

background = cv2.imread('C:/Users/Marci/Desktop/Smart City/Park Detect/bg.jpg')
background_gray = cv2.cvtColor(background, cv2.COLOR_RGB2GRAY)

# create video output
#fourcc = cv2.VideoWriter_fourcc(*'XVID')
#out = cv2.VideoWriter('outpy.avi', fourcc, 10, (1280,720))


while(True):
	clear_screen()

	ret, im = vidcap.read()
	if(ret == False): break

	image = np.array(im)
	imageGray = cv2.cvtColor(np.array(im), cv2.COLOR_RGB2GRAY)
	image2 = image.copy()
	diff = getDiffImageInGray(image, last_image)
	last_image = image.copy()

        # mean value of the asphalt
	meanAs = updateAsphaltColor(diff, imageGray, maskAsphalt)
	if (datetime.datetime.now().time()).hour > 20 or (datetime.datetime.now().time()).hour < 4 :
		value = meanAs*1/2
	else:
		value = meanAs*3/4
	# value for differentiation
	value = meanAs/2

	diffFromBlack_thresh = filterBlack(image, imageGray, value)
	print format_string(meanAs)
        image = cv2.resize(diffFromBlack_thresh,(640,360))
	cv2.imshow('original', diffFromBlack_thresh)
	cv2.moveWindow("original", 0,0)

	# calc graph
	#calcPercentages(diffFromBlack_thresh, diff)
        # calc occupied spots
	#calibrateSpots(image)

        # show every 5th rectangle
        #diffFromBlack_thresh = cv2.cvtColor(diffFromBlack_thresh, cv2.COLOR_GRAY2RGB)
	#drawOnRectangles(diffFromBlack_thresh,0,5)

        #out.write(image)
	#image = cv2.resize(image,(640,360))
	#cv2.imshow('original', image)
	#cv2.moveWindow("original", 0,0)

	#thresh2show = cv2.resize(diffFromBlack_thresh,(640,360))
	#cv2.imshow('shadow based', thresh2show)
	#cv2.moveWindow("shadow based", 705,0)


        # some fps calculating
	dt = datetime.datetime.now().time()
	time_now = dt.minute*60*1000 + dt.second*1000 + dt.microsecond/1000
	if time_now != last_time:
		fps = 1000/(time_now-last_time)
	else:
		fps = 0
	last_time = time_now
	print "fps: "+format_string(fps)
	

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
				cv2.imwrite('stat.png',image)
		if(close):
                        #out.release()
			break;


cv2.destroyAllWindows()
