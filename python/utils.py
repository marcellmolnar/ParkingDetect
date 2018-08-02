import collections
import cv2
import numpy as np


def getDiffImageInGray(image, lastImage):
	diffImage1 = cv2.addWeighted(image, 1, lastImage, -1, 0)
	diffImage2 = cv2.addWeighted(lastImage, 1, image, -1, 0)
	diffImage  = cv2.add(diffImage1, diffImage2)
	diffGray = cv2.cvtColor(diffImage, cv2.COLOR_BGR2GRAY)
	_, thresh = cv2.threshold(diffGray,10,255,cv2.THRESH_BINARY)
	return thresh

def calcNonZeroPixels(blackAndWhite, mask, percentage = True):
	maskSize = cv2.countNonZero(mask)

	blackAndWhite_masked = cv2.bitwise_and(blackAndWhite,blackAndWhite,mask = mask)
	nonZeroPixels = cv2.countNonZero(blackAndWhite_masked)
	if percentage:
		return 100*nonZeroPixels/maskSize
	else:
		return nonZeroPixels

def filterBlack(image, imageGray, grayValue):
	hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
	imageGrayNew = imageGray.copy()
	mask = np.where( hsv[:,:,2] > grayValue , 0, 255)

	hsv[:,:,2] = mask
	imageGrayNew[:] = np.where(mask > 120, 255, 0)
	return imageGrayNew


#####################################################################################################################


def meanOfArea(imageGray, maskAsphalt):
	image_masked = cv2.bitwise_and(imageGray,imageGray,mask = maskAsphalt)
	mean, stddev = cv2.meanStdDev(image_masked)
	imageSize = 1280*720
	areaSize = cv2.countNonZero(maskAsphalt)
	if(areaSize != 0):
		sumSquare = (stddev**2 + mean**2)*imageSize/areaSize
		areaMean = mean*imageSize/areaSize
		return areaMean[0][0]
	else:
		return 140

def updateAsphaltColor(diff, imageGray, maskAsphalt):
        nonZero = calcNonZeroPixels(diff, maskAsphalt, percentage = False)
	meanAsNow = meanOfArea(imageGray, maskAsphalt)
	if nonZero < 100:
		if updateAsphaltColor.cnt < 20:
			updateAsphaltColor.meanAsList.appendleft(meanAsNow)
			updateAsphaltColor.meanAs *= updateAsphaltColor.cnt
			updateAsphaltColor.meanAs += int(meanAsNow)
			updateAsphaltColor.meanAs /= (updateAsphaltColor.cnt+1)
			updateAsphaltColor.cnt += 1
		elif updateAsphaltColor.meanAs - meanAsNow < 10 and meanAsNow - updateAsphaltColor.meanAs < 10 :
			popped = updateAsphaltColor.meanAsList.pop()
			updateAsphaltColor.meanAsList.appendleft(int(meanAsNow))
			updateAsphaltColor.meanAs *= 20
			updateAsphaltColor.meanAs -= popped
			updateAsphaltColor.meanAs += int(meanAsNow)
			updateAsphaltColor.meanAs /= 20
        return updateAsphaltColor.meanAs

updateAsphaltColor.meanAsList = collections.deque(20*[0], 20)
updateAsphaltColor.meanAs = 0
updateAsphaltColor.cnt=0
