#define FILE_PATH_LAMP "/var/www/html/lamp.json"
#define IP_LAMP "192.168.4.4"
#define PORT_LAMP 3000

#include <stdio.h>
#include "Lamp.h"
#include "Door.h"

using namespace std;

int main()
{
	//Construct devices
	//Lamp lamp(IP_LAMP, FILE_PATH_LAMP, PORT_LAMP);
	//Lamp lamp("172.16.4.4", "/var/www/html/lamp.json" , 3005);

	Door door("192.168.4.10", "/var/www/html/Door.json" , 3010);
	//Connect to devices
	//lamp.connectToServer();
	door.connectToServer();

	while(1) {
		//lamp.sync();
		//usleep(1000000);
		door.sync();
		usleep(1000000);
	}

	return 0;
}
