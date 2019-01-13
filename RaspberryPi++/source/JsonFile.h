/*
 * JsonFile.h
 *
 *  Created on: Nov 26, 2018
 *      Author: matthijs
 */
#include "document.h"
#include <fstream>

#ifndef JSONFILE_H_
#define JSONFILE_H_

class JsonFile {
public:
	JsonFile(const char* filePath);								//Constructor either parses the contents of an existing file or creates a new file with contents {"NULL":"null"} if none is found
	JsonFile(const char* filePath, const char* jsonContent);	//Constructor creates a new file or overwrites existing file and fills it with jsonContent
	int updateFile();											//Updates the file with the current contents of doc
	int updateDoc();											//Updates the file with the current contents of file
	void fill(const char* input);								//fills file with 'input'
	void edit(const char* key, std::string newVal);				//change the value associated with 'key' to string 'newVal'
	void edit(const char* key, int newVal);						//change the value associated with 'key' to int 'newVal'
	void print(void);											//prints the contents of the file to the console
	int getIntValue(const char* key);							//returns the value associated with 'key' as an int
	const char* getStringValue(const char* key);				//returns the value associated with 'key' as a c-string

private:
	const char* filePath;		//path to the JSON file
	rapidjson::Document* doc;	//Model that simulates the JSON file within the program
	std::fstream* file;			//In/outputstream to the JSON file
};

#endif /* JSONFILE_H_ */
