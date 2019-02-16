#include <iostream>
#include "carHandler.h"
#include "constants.h"


using namespace std;

CarHandler::CarHandler() {
	listEmpty = true;
	numOfCars = 0;
}

bool CarHandler::isEmpty() {
	return listEmpty;
}

inline int distanceBetween(int a, int b) {
	if (a > b)
		return a - b;
	return b - a;
}

void CarHandler::matchNewCars(int* currentCars, int num) {
	if (isEmpty() && num > 0) {
		for (int i = 0; i < num; i++) {
			parking_lot.addNewWithCoordinate(currentCars[i]);
		}
		listEmpty = false;
		return;
	}

	// Match new cars with the last ones
	vector<bool> isNewCar(num, true);
	for (int i = 0; i < parking_lot.getNumberOfElements(); i++) {
		bool match = false;
		for (int currCarIndex = 0; currCarIndex < num; currCarIndex++) {
			if (distanceBetween(parking_lot.get(i).getPos(), currentCars[currCarIndex]) < MAX_MOVEMENT_OF_A_PARKING_CAR) {
				if (isNewCar[currCarIndex]){ // One new car should match with only one car
					match = true;
					isNewCar[currCarIndex] = false;
				}
			}
		}
		parking_lot.get(i).updateStatus(match);
	}

	// Put in new cars
	for (int currCarIndex = 0; currCarIndex < num; currCarIndex++) {
		if (isNewCar[currCarIndex]) {
			if (currentCars[currCarIndex] > MIN_COORDINATE) {
				//cout << "new" << endl;
			}
			parking_lot.addNewWithCoordinate(currentCars[currCarIndex]);
		}
	}

	// Throw out cars that is not parking anymore
	int subtract = 0;
	for (int i = 0; i < parking_lot.getNumberOfElements(); i++) {
		if (parking_lot.get(i).shouldThrowOut()) {
			cout << "removing: " << parking_lot.get(i).getPos() << endl;
			parking_lot.remove(i - subtract);
			subtract += 1;
		}
	}
}

CarHandler::~CarHandler() {
	cout << "died handler" << endl;
}
