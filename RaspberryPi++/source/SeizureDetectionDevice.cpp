/*
 * SeizureDetectionDevice.cpp
 *
 *  Created on: Jan 7, 2019
 *      Author: programmer
 */

#include "SeizureDetectionDevice.h"

bool SeizureDetectionDevice::pressureSensorLogic(const char* value) {
	int sensorValue = atoi(value);

	if(sensorValue > 500 && deviceUpdate)
	{
		onDevice = true;
		time(&timeOnDevice);
		deviceUpdate = false;
		absentTooLong = false;
		movementCounter++;
		cout << "Beweging counter omhoog" << endl;

		if(intervalStart)
		{
			time(&intervalStartTime);
			intervalStart = false;
			cout << "Interval gestart" << endl;
		}
	}
	else if(sensorValue < 500 && !deviceUpdate)
	{
		onDevice = false;
		deviceUpdate = true;
		time(&timeOffDevice);
		cout << "Uit bed of stoel" << endl;
	}

	if(difftime(time(0),intervalStartTime) > 8)
	{
		cout << "Interval voorbij" << endl;
		time(&intervalStartTime);
		movementCounter = 0;
		intervalStart = true;
	}

	if(movementCounter > 5)
	{
		return true; 			//return true als er meer dan 5 bewegingen binnnen het interval worden gedetecteerd
	}							//dit is gedefinieerd als een detectie van een epilepsieaanval
	else
	{
		return false;
	}
}

bool SeizureDetectionDevice::checkAbsence() {
	return absentTooLong;
}
