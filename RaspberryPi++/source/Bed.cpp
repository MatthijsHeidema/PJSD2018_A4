/*
 * Bed.cpp
 *
 *  Created on: Dec 10, 2018
 *      Author: matthijs
 */

#include "Bed.h"

void Bed::sync() {

	file->updateDoc();
	comm->connectToServer();
	const char* Night = file->getStringValue("Night");
	comm->sendValue("Night", Night);
	const char* outOfBed = comm->receiveValue("OutOfBed");
	const char* seizure = comm->receiveValue("Seizure");
	comm->disconnectFromServer();
	file->edit("OutOfBed", outOfBed);
	file->edit("Seizure", seizure);
	file->updateFile();
}
