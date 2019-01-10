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

	if(!strcmp(bedLightToggle,"1"))
	{
		if(!strcmp(bedLightStatus,"1"))
		{
			comm->sendValue("BedLight","0");
			file->edit("BedLight",NEE);
			bedLightToggle = "0";
		}
		if(!strcmp(bedLightStatus,"0"))
		{
			comm->sendValue("BedLight","1");
			time(&timeBedLightOn);
			file->edit("BedLight",JA);
			bedLightToggle = "0";
		}
	}

	if(nighttimeCheck())
	{
		automaticLightOff(5, bedLightStatus);

		if(difftime(time(0),timeOffDevice) > 5)
		{
			file->edit("OutOfBed",JA);
			absentTooLong = true;
		}
	}
	else
	{
		if(difftime(time(0),timeOffDevice) > 10)
		{
			absentTooLong = true;
		}
	}


	const char* pressureSensor = comm->receiveValue("PressureSensor");
	cout << pressureSensor << endl;

	if(pressureSensorLogic(pressureSensor)){
		file->edit("EpilepsieJAvalBed",JA);
	}

	file->updateFile();

}



void Bed::automaticLightOff(int timeUntilOff, const char* lightStatus) {

	if(difftime(time(0),timeBedLightOn) > timeUntilOff && !strcmp(lightStatus,"1"))
	{
		comm->sendValue("BedLight","0");
		file->edit("BedLight","0");
	}
}
