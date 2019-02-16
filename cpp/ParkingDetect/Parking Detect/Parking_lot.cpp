#include "Parking_lot.h"

using namespace std;

Parking_lot::Parking_lot() {
	numOfElements = 0;
	parkingCars = nullptr;
}


Parking_lot::~Parking_lot() {
	cout << "died lot" << endl;
}

void Parking_lot::addNewWithCoordinate(int pos) {
	if (pos > MIN_COORDINATE) {
		cout << "new car at: " << pos << endl;
	}
	numOfElements += 1;
	Parking_spot* tempVec = new Parking_spot[numOfElements];
	for (int i = 0; i < numOfElements - 1; i++) {
		tempVec[i] = parkingCars[i];
	}
	tempVec[numOfElements - 1] = Parking_spot(pos);
	if (parkingCars != nullptr) {
		delete[] parkingCars;
	}
	parkingCars = tempVec;
}

void Parking_lot::remove(int posToDel) {
	if (parkingCars[posToDel].getPos() > MIN_COORDINATE) {
		cout << "left from: " << parkingCars[posToDel].getPos() << endl;
	}
	numOfElements -= 1;
	if (numOfElements > 0) {
		Parking_spot* tempVec = new Parking_spot[numOfElements];
		for (int i = 0, j = 0; i < numOfElements; i++, j++) {
			if (i != posToDel) {
				tempVec[j] = parkingCars[i];
			}
			else {
				j--;
			}
		}
		if (parkingCars != nullptr) {
			delete[] parkingCars;
		}
		parkingCars = tempVec;
	}
	else {
		parkingCars = nullptr;
	}
}

const Parking_spot& Parking_lot::get(int i) const{
	return parkingCars[i];
}

Parking_spot& Parking_lot::get(int i) {
	return parkingCars[i];
}

int Parking_lot::getNumberOfElements() {
	return numOfElements;
}