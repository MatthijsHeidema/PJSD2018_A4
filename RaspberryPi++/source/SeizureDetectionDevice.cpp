/*
 * SeizureDetectionDevice.cpp
 *
 *  Created on: Jan 7, 2019
 *      Author: programmer
 */

#include "SeizureDetectionDevice.h"

bool SeizureDetectionDevice::pressureSensorLogic(const char* value) {
	int sensorValue = atoi(value);						//Convert received string to int

	if(sensorValue > 500 && deviceUpdate)				//If pressure sensor is triggered and flag is true so code is only
	{													//executed once each time someone triggers the sensor
		onDevice = true;
		time(&timeOnDevice);
		deviceUpdate = false;
		absentTooLong = false;
		movementCounter++;								//Increase movement counter for seizure detection

		if(intervalStart)								//Check if it's the start of a new interval for seizure detection
		{
			time(&intervalStartTime);
			intervalStart = false;
		}
	}
	else if(sensorValue < 500 && !deviceUpdate)			//If pressure sensor is no longer triggered
	{
		onDevice = false;
		deviceUpdate = true;
		time(&timeOffDevice);
	}

	if(difftime(time(0),intervalStartTime) > 8)			//Check if interval for seizure detection has passed
	{
		time(&intervalStartTime);						//Restart interval
		movementCounter = 0;							//Reset movement count
		intervalStart = true;
	}

	if(movementCounter > 5)
	{
		return true; 			//Returns true if the pressure sensor went below and above 500 more than 5 times within 8 seconds
	}							//This is then interpreted as the detection of an epileptic seizure
	else
	{
		return false;
	}
}

bool SeizureDetectionDevice::checkAbsence() {
	return absentTooLong;		//Getter for the main to see if Timothy has been in neither his chair or bed for too long
}
