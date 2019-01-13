/*
 * Stoel.cpp
 *
 *  Created on: Jan 7, 2019
 *      Author: programmer
 */

#include "Stoel.h"
#include <ctime>

void Stoel::sync() {

	file->updateDoc();
	string JA = "1";
	string NEE = "0";

	const char* pressureSensor = comm->receiveValue("PressureSensor");
	const char* vibrateStatus = file->getStringValue("StoelTril");
	int notificationTimeDay = file->getIntValue("updateTimeDay");
	int notificationTimeNight = file->getIntValue("updateTimeNight");

	if(pressureSensorLogic(pressureSensor))		//Check if epileptic seizure is detected
	{
		file->edit("EpilepsieAanvalStoel",JA);	//Write to JSON to send a notification to guard
	}

	if(nighttimeCheck())
	{
		if(difftime(time(0),timeOffDevice) > notificationTimeNight && !onDevice)	//Check if Timothy has not been on his chair
		{																			//for longer than set time at night
			absentTooLong = true;													//Set boolean for the check in main loop
		}
		if(difftime(time(0),timeOnDevice) > notificationTimeNight && onDevice)		//Check if Timothy has been in his chair for
		{																			//longer than the set time
			comm->sendValue("ChairVibrate","1");									//Make the chair vibrate so Timothy doesn't fall
			file->edit("StoelTril",JA);												//asleep
		}
		if(!onDevice && !strcmp(vibrateStatus,"1"))									//Stop vibrating when Timothy gets up
		{
			comm->sendValue("ChairVibrate","0");
			file->edit("StoelTril",NEE);
		}
	}
	else
	{
		if(difftime(time(0),timeOffDevice) > notificationTimeDay && !onDevice)		//Check if Timothy has not been on his chair
		{																			//for longer than the set time during the day
			absentTooLong = true;													//Set boolean
		}
	}

	file->updateFile();
}

