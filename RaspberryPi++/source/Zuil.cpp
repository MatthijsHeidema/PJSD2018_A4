/*
 * Zeil.cpp
 *
 *  Created on: Jan 7, 2019
 *      Author: jerry
 */

#include "Zuil.h"

void Zuil::sync() {

	file->updateDoc();
	string JA = "1";
	string NEE = "0";

	const char* Rooksensor = comm->receiveValue("Rooksensor");
	const char* alarmStatus = file ->getStringValue("SmokeAlarm");
	int sensorValue = atoi(Rooksensor);

	if(sensorValue > 300 && !RookStatus){
		comm->sendValue("RookMelderStatus",JA);
		file->edit("SmokeAlarm",JA);
		RookStatus = true;
		cout << "Rook gedetecteerd!" << endl;
	}else if(!strcmp(alarmStatus,NEE.c_str()) && RookStatus){
		comm->sendValue("RookMelderStatus",NEE);
		RookStatus = false;
		cout << "Alarm uitgeschakeld" << endl;
	}

	file->updateFile();
}
