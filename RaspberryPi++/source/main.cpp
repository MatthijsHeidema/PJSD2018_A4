#include <stdio.h>
#include "JsonFile.h"
#include "Lamp.h"
#include "Door.h"
#include "Bed.h"
#include "Stoel.h"
#include "Fridge.h"
#include "Window.h"
#include "Zuil.h"

#include "definitions.h"

using namespace std;

int main()
{
	//Construct JSON object for main
	JsonFile main(FILE_PATH_MAIN, JSON_MAIN);

	//Construct devices
	Lamp lamp(IP_LAMP, FILE_PATH_LAMP , PORT, JSON_LAMP);
	Bed bed(IP_BED, FILE_PATH_BED, PORT, JSON_BED);
	Stoel chair(IP_CHAIR, FILE_PATH_CHAIR, PORT, JSON_CHAIR);
	Door door(IP_DOOR, FILE_PATH_DOOR, PORT, JSON_DOOR);
	Fridge fridge(IP_FRIDGE, FILE_PATH_FRIDGE, PORT, JSON_FRIDGE);
	Window window(IP_WINDOW, FILE_PATH_WINDOW, PORT, JSON_WINDOW);
	Zuil zuil(IP_PILLAR, FILE_PATH_PILLAR, PORT, JSON_PILLAR);

	//Connect to devices
	lamp.connectToServer();
	bed.connectToServer();
	chair.connectToServer();
	door.connectToServer();
	fridge.connectToServer();
	window.connectToServer();
	zuil.connectToServer();
	
	while(1) {

		//Loops through all devices, sync function handles everything
		lamp.sync();
		bed.sync();
		chair.sync();
		door.sync();
		fridge.sync();
		window.sync();
		zuil.sync();
		//usleep(50000);

		if(bed.checkAbsence() && chair.checkAbsence())	//Check if Timothy has not been in his chair or bed for
		{												//the set amount of time to determine whether a notification
			string yes = "1";
			main.edit("TooLongAbsent",yes);				//should be sent to the guard.
			cout << "CheckAbsence" << endl;
		}

	}

	return 0;
}
