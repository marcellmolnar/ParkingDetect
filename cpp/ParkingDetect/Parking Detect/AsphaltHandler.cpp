#include "AsphaltHandler.h"

AsphaltHandler::AsphaltHandler() {
}

double AsphaltHandler::updateAsphaltColor(double value, double numberOfMovingPixels, double* grayValues, int numberOfValues) {
	if (numberOfMovingPixels < 30) {
		if (count > 0) {
			avg *= count;
			avg -= grayValues[0];
		}
		if (count < numberOfValues)
			count++;
		for (int i = 0; i < numberOfValues - 1; i++) {
			grayValues[i] = grayValues[i + 1];
		}
		grayValues[numberOfValues - 1] = value;

		avg += value;
		avg /= count;
	}
	return avg;
}


AsphaltHandler::~AsphaltHandler() {
}
