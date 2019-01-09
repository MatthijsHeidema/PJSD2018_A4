/*
 * definitions.h
 *
 *  Created on: Jan 9, 2019
 *      Author: programmer
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define PORT 3000

#define FILE_PATH_MAIN "/var/www/html/main.json"
#define JSON_MAIN "{\"TooLongAbsent\" : \"0\"}"

#define FILE_PATH_LAMP "/var/www/html/lamp.json"
#define IP_LAMP "192.168.4.4"
#define JSON_LAMP "{\"ToggleLed\":\"0\",\"Color\":\"White\",\"PIR\":\"0\"}"

#define FILE_PATH_BED "/var/www/html/bed.json"
#define IP_BED "192.168.4.5"
#define JSON_BED "{\"BedLight\":\"0\",\"OutOfBed\":\"1\",\"EpilepsieAanvalBed\":\"0\"}"

#define FILE_PATH_CHAIR "/var/www/html/chair.json"
#define IP_CHAIR "192.168.4.6"
#define JSON_CHAIR "{\"EpilepsieAanvalStoel\":\"0\",\"StoelTril\":\"0\"}"

#define FILE_PATH_DOOR "/var/www/html/door.json"
#define IP_DOOR "192.168.4.7"
#define JSON_DOOR

#define FILE_PATH_WINDOW "/var/www/html/window.json"
#define IP_WINDOW "192.168.4.8"
#define JSON_WINDOW

#define FILE_PATH_FRIDGE "/var/www/html/fridge.json"
#define IP_GRIDGE "192.168.4.9"
#define JSON_FRIDGE

#define FILE_PATH_COLUMN "/var/www/html/column.json"
#define IP_COLUMN "192.168.4.10"
#define JSON_COLUMN

#endif /* DEFINITIONS_H_ */
