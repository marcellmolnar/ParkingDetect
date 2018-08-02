import datetime

class Parking_lot:
	def __init__(self,rect):
		self.corner1 = rect[0]
		self.corner2 = rect[1]
		self.corner3 = rect[2]
		self.corner4 = rect[3]
		self.minx = self.corner1[0]
		if self.corner2[0] < self.minx: self.minx = self.corner2[0]
		self.miny = self.corner1[1]
		if self.corner4[1] < self.miny: self.miny = self.corner4[1]
		self.maxx = self.corner3[0]
		if self.corner4[0] > self.maxx: self.maxx = self.corner4[0]
		self.maxy = self.corner2[1]
		if self.corner4[1] > self.maxy: self.maxy = self.corner4[1]
		self.isOccupied = True
		self.occupiedSince = datetime.datetime.now()
		self.isOccupiedNew = False
		self.newStatusSince = datetime.datetime.now()
	def scale(self,scale):
		self.corner1 = (int (self.corner1[0] * scale), int (self.corner1[1] * scale))
		self.corner2 = (int (self.corner2[0] * scale), int (self.corner2[1] * scale))
		self.corner3 = (int (self.corner3[0] * scale), int (self.corner3[1] * scale))
		self.corner4 = (int (self.corner4[0] * scale), int (self.corner4[1] * scale))
	def offset(self,offset):
		offsetx = offset[0]
		offsety = offset[1]
		corner1 = (int (self.corner1[0] + offsetx), int (self.corner1[1] + offsety))
		corner2 = (int (self.corner2[0] + offsetx), int (self.corner2[1] + offsety))
		corner3 = (int (self.corner3[0] + offsetx), int (self.corner3[1] + offsety))
		corner4 = (int (self.corner4[0] + offsetx), int (self.corner4[1] + offsety))
		return Parking_lot(corner1,corner2,corner3,corner4)
	def getBoundingRect(self):
		return [(self.minx,self.miny),(self.minx,self.maxy),(self.maxx,self.maxy),(self.maxx,self.miny)]
	def getRect(self):
		return [self.corner1, self.corner2, self.corner3, self.corner4]
	def contains(self, point):
		return (self.minx < point[0] and point[0] < self.maxx and self.miny < point[1] and point[1] < self.maxy)
	def haveCommonAreaWith(self, parking_lot):
		A = not (self.corner4[0] < parking_lot.corner1[0] and parking_lot.corner3[0] < parking_lot.corner2[0])
		B = not (self.corner1[0] > parking_lot.corner4[0] and parking_lot.corner2[0] > parking_lot.corner3[0])
		return A and B
	def center(self):
		return (int((self.minx+self.maxx)/2), int((self.miny+self.maxy)/2) )
	def distanceFrom(self, other):
		dist = (self.center())[0] - (other.center())[0]
		if dist < 0: dist = -dist
		return dist
	def throwOut(self, index):
		timeDiff = datetime.datetime.now()-self.newStatusSince
		timeSinceLastChange = timeDiff.total_seconds()
		sqlQuery(index, (datetime.datetime.now()-self.occupiedSince).total_seconds(), self.isOccupied)
	def updateStatus(self,status):
		if status != self.isOccupiedNew:
			self.isOccupiedNew = status
			self.newStatusSince = datetime.datetime.now()
		else: 
			if self.isOccupied != self.isOccupiedNew:
				timeDiff = datetime.datetime.now()-self.newStatusSince
				timeSinceLastChange = timeDiff.total_seconds()
				if timeSinceLastChange > 1:
					self.isOccupied = self.isOccupiedNew
					if self.isOccupied == False:
						sqlQuery(0, (datetime.datetime.now()-self.occupiedSince).total_seconds(), self.isOccupied)
					else:
						self.occupiedSince = datetime.datetime.now()

def sqlQuery(index, parkingTimeSec, newStatus):
	#print "change at: "+str(index)+", parking time: "+"{:7.2f}".format(parkingTimeSec)
	pass
