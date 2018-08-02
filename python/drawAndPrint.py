import cv2
import os
import platform

##############################################################################################################
## Drawers:


def draw_parkinglot_on_image(image, index, color, width=2):
	px = parkinglots[index]
	cv2.line(image,px.corner1,px.corner2,color,width)
	cv2.line(image,px.corner2,px.corner3,color,width)
	cv2.line(image,px.corner3,px.corner4,color,width)
	cv2.line(image,px.corner4,px.corner1,color,width)

def draw_rectangle_on_image(image, rect, color, width=2):
	cv2.line(image,rect[0],rect[1],color,width)
	cv2.line(image,rect[1],rect[2],color,width)
	cv2.line(image,rect[2],rect[3],color,width)
	cv2.line(image,rect[3],rect[0],color,width)

def draw_asphalt_on_image(image, asphalt, color, width=2):
	px = asphalt
	cv2.line(image,px.corner1,px.corner2,color,width)
	cv2.line(image,px.corner2,px.corner3,color,width)
	cv2.line(image,px.corner3,px.corner4,color,width)
	cv2.line(image,px.corner4,px.corner1,color,width)

##############################################################################################################
## Printer:

	
def clear_screen():
        if platform.system() == 'Linux':
                os.system('clear')	# clear for logging the deviations
        if platform.system() == 'Windows':
                clear = lambda: os.system('cls')
                clear()

def format_string(value):
	return ""+"{:8.2f}".format(value)+""


def print_values(meanB, meanG, meanR, devB, devG, devR,space, index='  a', percentage = 0):
	print "  "+space+str(index)+":"+format_string(devB)+" "+format_string(devG)+" "+format_string(devR)+" "+format_string(meanB)+" "+format_string(meanG)+" "+format_string(meanR)+" "+format_string(percentage)

def printText(text):
	print str(text)

#####################################################################################################################

def clear_screen():
        if platform.system() == 'Linux':
                os.system('clear')	# clear for logging the deviations
        if platform.system() == 'Windows':
                clear = lambda: os.system('cls')
                clear()
