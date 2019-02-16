#pragma once

class AsphaltHandler {
	int count = 0;
	double avg = 0;
public:
	AsphaltHandler();
	~AsphaltHandler();

	/* Tracks the color of the asphalt. Only update when there are no movement, and we take the average of numberOfValues values. */
	double updateAsphaltColor(double value, double numberOfWhitePixels, double* grayValues, int numberOfValues);
};

