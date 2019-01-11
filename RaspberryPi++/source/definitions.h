/*
 * definitions.h
 *
 *  Created on: Jan 9, 2019
 *      Author: programmer
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define PORT 3000

#define IP_LAMP 	"172.16.4.10"
#define IP_BED	 	"172.16.4.11"
#define IP_FRIDGE 	"172.16.4.12"
#define IP_CHAIR 	"172.16.4.13"
#define IP_DOOR 	"172.16.4.14"
#define IP_WINDOW 	"172.16.4.15"
#define IP_PILLAR 	"172.16.4.16"


#define FILE_PATH_MAIN "/var/www/html/main.json"
#define JSON_MAIN "{\"TooLongAbsent\" : \"0\"}"

#define FILE_PATH_LAMP "/var/www/html/lamp.json"
#define JSON_LAMP "{\"ToggleLed\":\"0\",\"Color\":\"White\",\"PIR\":\"0\"}"

#define FILE_PATH_BED "/var/www/html/bed.json"
#define JSON_BED "{\"BedLight\":\"0\",\"OutOfBed\":\"1\",\"OutOfBedCount\":0,\"EpilepsieAanvalBed\":\"0\",\"updateTimeDay\":15,\"updateTimeNight\":10}"

#define FILE_PATH_CHAIR "/var/www/html/chair.json"
#define JSON_CHAIR "{\"EpilepsieAanvalStoel\":\"0\",\"StoelTril\":\"0\",\"updateTimeDay\":15,\"updateTimeNight\":10}"

#define FILE_PATH_DOOR "/var/www/html/door.json"
#define JSON_DOOR "{\"doorStatus\":\"0\"}"

#define FILE_PATH_WINDOW "/var/www/html/window.json"
#define JSON_WINDOW "{\"windowStatus\":\"0\",\"aangepast\":\"0\",\"ledBrightness\":\"0\"}"

#define FILE_PATH_FRIDGE "/var/www/html/fridge.json"
#define JSON_FRIDGE "{\"OpenTooLong\":\"0\"}"

#define FILE_PATH_COLUMN "/var/www/html/column.json"
#define JSON_COLUMN "{\"SmokeAlarm\":\"0\"}"

#endif /* DEFINITIONS_H_ */
