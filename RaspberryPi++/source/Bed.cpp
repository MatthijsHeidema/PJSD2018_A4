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

	const char* bedLightToggle = comm->receiveValue("BedLight");
	//cout << "Licht toggle: " << bedLightToggle << endl;
	const char* bedLightStatus = file->getStringValue("BedLight");
	//cout << "Licht status: " << bedLightStatus << endl;


	if(!strcmp(bedLightToggle,"1"))
	{
		//cout << "Light toggled" << endl;
		if(!strcmp(bedLightStatus,"1"))
		{
			comm->sendValue("BedLight","0");
			file->edit("BedLight","0");
			bedLightToggle = "0";
		}
		if(!strcmp(bedLightStatus,"0"))
		{
			comm->sendValue("BedLight","1");
			time(&timeBedLightOn);
			file->edit("BedLight","1");
			bedLightToggle = "0";
		}
	}

	if(nighttimeCheck())
	{
		automaticLightOff(5);

		if(difftime(time(0),timeOffDevice) > 5)
		{
			//cout << "Langer dan 5 seconden uit bed" << endl;
			file->edit("OutOfBed","1");
		}
	}


	const char* pressureSensor = comm->receiveValue("PressureSensor");
	cout << pressureSensor << endl;

	if(pressureSensorLogic(pressureSensor)){
		//cout << "EPILEPSIE AANVAL" << endl;
		file->edit("EpilepsieAanvalBed","1");
	}

	file->updateFile();

}



void Bed::automaticLightOff(int timeUntilOff) {

	const char* bedLightStatus = file->getStringValue("BedLight");

	if(difftime(time(0),timeBedLightOn) > timeUntilOff && !strcmp(bedLightStatus,"1"))
	{
		comm->sendValue("BedLight","0");
		file->edit("BedLight","0");
	}
}
