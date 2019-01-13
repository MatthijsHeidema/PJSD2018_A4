/*
 * Bed.cpp
 *
 *  Created on: Dec 10, 2018
 *      Author: matthijs
 */

#include "Bed.h"
#include <ctime>

void Bed::sync() {


	file->updateDoc();

	string JA = "1";
	string NEE = "0";

	const char* bedLightToggle = comm->receiveValue("BedLight");
	const char* bedLightStatus = file->getStringValue("BedLight");
	int notificationTimeDay = file->getIntValue("updateTimeDay");
	int notificationTimeNight = file->getIntValue("updateTimeNight");

	if(!strcmp(bedLightToggle,"1"))				//Check if bedlight switch was toggled
	{
		if(!strcmp(bedLightStatus,"1"))			//If light was already on, turn it off
		{
			comm->sendValue("BedLight","0");
			file->edit("BedLight",NEE);
			bedLightToggle = "0";
		}
		if(!strcmp(bedLightStatus,"0"))			//If light was off, turn it on
		{
			comm->sendValue("BedLight","1");
			time(&timeBedLightOn);
			file->edit("BedLight",JA);
			bedLightToggle = "0";
		}
	}

	if(nighttimeCheck())
	{
		automaticLightOff(5, bedLightStatus);			//At night bed light will turn off automatically to preserve power

		if(nightUpdate){								//Flag used to only trigger this code at the start of the night
			file->edit("OutOfBedCount",outOfBedCount);	//Write the times out of bed to the JSON file to be displayed in browser
			outOfBedCount = 0;							//Reset count
			nightUpdate = false;
			time(&timeOffDevice);						//Restart timer at start of night so the notification isn't instantly
		}												//sent at the start of the night and Timothy has not gone to bed yet

		if(difftime(time(0),timeOffDevice) > notificationTimeNight)	//Check if Timothy has been out of bed too long
		{
			file->edit("OutOfBed",JA);					//Write to JSON to trigger notification for guard
			absentTooLong = true;						//Set boolean for the check inside the main loop
			outOfBedCount++;							//Increase out of bed too long count by one each time the interval passes
			cout << "Out of bed count omhoog" << endl;
			time(&timeOffDevice);						//Restart interval
		}
	}
	else
	{
		if(difftime(time(0),timeOffDevice) > notificationTimeDay)	//Check if Timothy hasn't been on his bed for longer than
		{															//the set time during the day
			absentTooLong = true;									//Set boolean for the check inside the main loop
		}
		nightUpdate = true;											//Reset flag so the code above can be exectued again once at
	}																//the start of the next night


	const char* pressureSensor = comm->receiveValue("PressureSensor");

	if(pressureSensorLogic(pressureSensor)){						//Check if an epileptic seizure was detected
		file->edit("EpilepsieAanvalBed",JA);						//Write to JSON to send a notification
	}

	file->updateFile();

}



void Bed::automaticLightOff(int timeUntilOff, const char* lightStatus) {

	if(difftime(time(0),timeBedLightOn) > timeUntilOff && !strcmp(lightStatus,"1"))	//Check if time has passed for the light
	{																				//to be turned off automatically and status
		comm->sendValue("BedLight","0");											//so it only gets sent once
		file->edit("BedLight","0");
	}
}
