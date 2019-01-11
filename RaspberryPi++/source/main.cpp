#include <stdio.h>
#include "JsonFile.h"
#include "Lamp.h"
#include "Door.h"
#include "Bed.h"
#include "Stoel.h"
#include "Fridge.h"

#include "definitions.h"

using namespace std;

int main()
{
	//Construct JSON object for main
	//JsonFile main(FILE_PATH_MAIN, JSON_MAIN);

	//Lamp lamp("192.168.4.4", "/var/www/html/lamp.json" , 3005);
	//Bed bed("192.168.4.8", "/var/www/html/bed.json", 3000);
	//Stoel stoel("192.168.4.9", "/var/www/html/chair.json", 3001);
	//Door door("192.168.4.10", "/var/www/html/door.json" , 3010);
	Fridge fridge("172.16.4.20", FILE_PATH_FRIDGE, 3020);
	//Connect to devices
	//lamp.connectToServer();
	//stoel.connectToServer();
	//bed.connectToServer();
	fridge.connectToServer();
	
	while(1) {
		//lamp.sync();
		//stoel.sync();
		//bed.sync();
		fridge.sync();
		usleep(500000);
	}

	return 0;
}
