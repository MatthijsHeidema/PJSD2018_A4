// Client side C/C++ program to demonstrate Socket programming
// Geeks for Geeks and additions by Mathieu

#define FILE_PATH "/home/pi/Documents/PI_client/client.json"
#define FILE_PATH_LAMP "/var/www/html/lamp.json"
#define IP_LAMP "192.168.3.1"
#define PORT_LAMP 3000

#include <stdio.h>
#include "Lamp.h"

using namespace std;

int main()
{
	Lamp lamp(IP_LAMP, FILE_PATH_LAMP, PORT_LAMP);

	while(1) {

		lamp.sync();
	}

	return 0;
}
