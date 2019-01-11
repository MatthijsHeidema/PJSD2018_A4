#include <stdio.h>
#include "JsonFile.h"
#include "Lamp.h"
#include "Door.h"
#include "Bed.h"
#include "Stoel.h"
#include "Fridge.h"
#include "Window.h"
//#include "Column.h"

#include "definitions.h"

using namespace std;

int main()
{
	//Construct JSON object for main
	JsonFile main(FILE_PATH_MAIN, JSON_MAIN);

	Lamp lamp(IP_LAMP, FILE_PATH_LAMP , 3000, JSON_LAMP);
	Bed bed(IP_BED, FILE_PATH_BED, 3000, JSON_BED);
	Stoel chair(IP_CHAIR, FILE_PATH_CHAIR, 3000, JSON_CHAIR);
	Door door(IP_DOOR, FILE_PATH_DOOR, 3000, JSON_DOOR);
	Fridge fridge(IP_FRIDGE, FILE_PATH_FRIDGE, 3000, JSON_FRIDGE);
	Window window(IP_WINDOW, FILE_PATH_WINDOW, 3000, JSON_WINDOW);


	//Connect to devices
	lamp.connectToServer();
	bed.connectToServer();
	chair.connectToServer();
	door.connectToServer();
	fridge.connectToServer();
	window.connectToServer();
	
	while(1) {
		lamp.sync();
		bed.sync();
		chair.sync();
		door.sync();
		fridge.sync();
		window.sync();
		usleep(100000);
	}

	return 0;
}
