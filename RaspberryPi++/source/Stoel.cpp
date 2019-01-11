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

	cout << "Delay tijd dag: " << notificationTimeDay << endl;
	cout << "Delay tijd nacht: " << notificationTimeNight << endl;

	if(pressureSensorLogic(pressureSensor))
	{
		file->edit("EpilepsieAanvalStoel",JA);
	}

	if(nighttimeCheck())
	{
		if(difftime(time(0),timeOffDevice) > notificationTimeDay && !onDevice)
		{
			absentTooLong = true;
		}
		if(difftime(time(0),timeOnDevice) > notificationTimeDay && onDevice)
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
		if(difftime(time(0),timeOffDevice) > notificationTimeNight && !onDevice)
		{
			absentTooLong = true;
		}
	}

	file->updateFile();
}

