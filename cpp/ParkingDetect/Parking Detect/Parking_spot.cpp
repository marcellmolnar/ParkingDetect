//own
#include "Parking_spot.h"

using namespace std;
using namespace shapes;

int Parking_spot::num = 0;
Parking_spot::Parking_spot(int pos) {
	position = pos;
	isOccupied = true;
	isOccupiedNew = false;
	occupiedSince = clock();
	newStatusSince = clock();
	myNum = num;
	num++;
}

Parking_spot::Parking_spot(const Parking_spot& other) {
	position = other.position;
	isOccupied = other.isOccupied;
	isOccupiedNew = other.isOccupiedNew;
	occupiedSince = other.occupiedSince;
	newStatusSince = other.newStatusSince;
	myNum = num;
	num++;
}

Parking_spot::~Parking_spot() {
}

int Parking_spot::getPos() const {
	return position;
}

double clkToMilliseconds(clock_t ticks) {
	return (ticks / (double)CLOCKS_PER_SEC)*1000.0;
}


void Parking_spot::updateStatus(bool occupied) {
	if(isOccupiedNew != occupied) {
		isOccupiedNew = occupied;
		newStatusSince = clock();
	}
	else {
		if (isOccupied != isOccupiedNew) {
			clock_t diff = clock() - newStatusSince;
			if (clkToMilliseconds(diff) > 1000) {
				isOccupied = isOccupiedNew;
				if (isOccupied) {
					occupiedSince = clock();
				}
			}
		}
	}
}

bool Parking_spot::shouldThrowOut() const {
	if (isOccupied == false) {
		return true;
	}
	return false;
}

bool Parking_spot::getStat() const {
	return isOccupied;
}
