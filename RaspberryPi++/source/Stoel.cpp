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

	if(pressureSensorLogic(pressureSensor))
	{
		file->edit("EpilepsieAanvalStoel",JA);
	}

	if(nighttimeCheck())
	{
		if(difftime(time(0),timeOffDevice) > 5 && !onDevice)
		{
			absentTooLong = true;
		}
		if(difftime(time(0),timeOnDevice) > 5 && onDevice)
		{
			comm->sendValue("ChairVibrate","1");
			file->edit("StoelTril",JA);
		}
		if(!onDevice && !strcmp(vibrateStatus,"1"))
		{
			comm->sendValue("ChairVibrate","0");
			file->edit("StoelTril",NEE);
		}
	}
	else
	{
		if(difftime(time(0),timeOffDevice) > 10 && !onDevice)
		{
			absentTooLong = true;
		}
	}



	file->updateFile();


}

