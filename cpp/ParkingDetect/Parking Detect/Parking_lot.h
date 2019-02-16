#pragma once
//C++
#include <iostream>
#include <vector>
//own
#include "Parking_spot.h"

class Parking_lot{
	Parking_spot* parkingCars;
	int numOfElements;
public:
	Parking_lot();
	~Parking_lot();
	void addNewWithCoordinate(int pos);
	void remove(int i);
	const Parking_spot& get(int i) const;
	Parking_spot& get(int i);
	int Parking_lot::getNumberOfElements();
};

