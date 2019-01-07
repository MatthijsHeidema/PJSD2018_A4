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
			time(&timeLightOn);
			file->edit("BedLight","1");
			bedLightToggle = "0";
		}
	}

	if(nighttimeCheck())
	{
		automaticLightOff(5);

		if(difftime(time(0),timeOutOfBed) > 5)
		{
			//cout << "Langer dan 5 seconden uit bed" << endl;
			file->edit("OutOfBed","1");
		}
	}


	const char* pressureSensor = comm->receiveValue("PressureSensor");
	cout << pressureSensor << endl;

	if(pressureSensorLogic(pressureSensor)){
		//cout << "EPILEPSIE AANVAL" << endl;
		file->edit("EpilepsieAanval","1");
	}

	file->updateFile();

}

bool Bed::pressureSensorLogic(const char* value) {
	int sensorValue = atoi(value);

	if(sensorValue > 500 && bedUpdate)
	{
		inBed = true;
		bedUpdate = false;
		movementCounter++;
		cout << "Beweging counter omhoog" << endl;


		if(intervalStart)
		{
			time(&intervalStartTime);
			intervalStart = false;
			cout << "Interval gestart" << endl;
		}
	}
	else if(sensorValue < 500 && !bedUpdate)
	{
		inBed = false;
		bedUpdate = true;
		time(&timeOutOfBed);
		cout << "Uit bed" << endl;
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

void Bed::automaticLightOff(int timeUntilOff) {

	const char* bedLightStatus = file->getStringValue("BedLight");

	if(difftime(time(0),timeLightOn) > timeUntilOff && !strcmp(bedLightStatus,"1"))
	{
		comm->sendValue("BedLight","0");
		file->edit("BedLight","0");
	}
}
