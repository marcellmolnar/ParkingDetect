#pragma once
//C++
#include <vector>
//own
#include "Parking_lot.h"
#include "Parking_spot.h"

class CarHandler {
	Parking_lot parking_lot;
	std::vector<Parking_spot> carCoords;
	int numOfCars;
	bool listEmpty;
public:
	CarHandler();
	~CarHandler();
	bool isEmpty();
	void matchNewCars(int* list, int num);
};

