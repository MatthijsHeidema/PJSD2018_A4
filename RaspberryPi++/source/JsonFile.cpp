/*
 * JsonFile.cpp
 *
 *  Created on: Nov 26, 2018
 *      Author: matthijs
 */

#include "JsonFile.h"

#include <iostream>
#include <string>
#include "writer.h"
#include "stringbuffer.h"

using namespace std;
using namespace rapidjson;

JsonFile::JsonFile(const char* filePath): filePath(filePath){ //Constructor either parses the contents of an existing file or creates a new file with contents {"NULL":"null"} if none is found
	doc = new Document;							//Document simulates a JSON document within the program
	file = new fstream;							//fstream allows input from and output to a file on the hard disk
	if (updateDoc()) {							//Read the contents of the file. If the file does not exist,
		doc->Parse("{\"NULL\" : \"null\"}");	//content of new file
		ofstream newFile;						//create a new file
		newFile.open(filePath);
		newFile.close();
	}
	updateFile();
}

JsonFile::JsonFile(const char* filePath, const char* jsonContent): filePath(filePath){	//
	doc = new Document;
	file = new fstream;

	doc->Parse(jsonContent);	//Fill doc with jsonContent
	//create new file (overwrites existing)
	ofstream newFile;
	newFile.open(filePath);
	newFile.close();

	updateFile(); //
}

int JsonFile::updateDoc() {	//Updates the doc with the current contents of file
	string temp;							//string to store the text from the file
	string line;							//string to store lines from the file

	file->open(filePath);					//open the file
	if (file->is_open()) {					//check if the file is available

		while (getline(*file, line)) {		//reads a line from the file and stores it in 'line'
			temp += line;					//add line to 'temp'
		}
		file->close();						//close the file
		doc->Parse(temp.c_str());			//parse the content of 'temp' to 'doc'

		return 0;

	} else {
		return 1;	//return 1 if it fails
	}
}

int JsonFile::updateFile() { //Updates the file with the current contents of doc
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc->Accept(writer);							//pair 'writer' with 'doc'

	file->open(filePath, ios::out | ios::trunc);	//open file as output, truncate file after outputting
	if (file->is_open()) {							//if file is available
		*file << buffer.GetString() << endl;		//output contents of 'doc' to the file
		file->close();								//close file

		return 0;

	} else {
		return 1;	//return 1 if it fails
	}
}

void JsonFile::print() {					//prints the contents of the file to the console

	assert(!updateDoc());					//make sure that updateDoc() doesn't fail
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc->Accept(writer);

	cout << buffer.GetString() << endl;
}

void JsonFile::fill(const char* input) {	//fills file with 'input'
	doc->Parse(input);
	assert(!updateFile());					//make sure updateFile() doesn't fail
}

int JsonFile::getIntValue(const char* key) {	//returns the value associated with 'key' as an int
	assert((*doc)[key].IsInt());				//make sure value is an integer
	return (*doc)[key].GetInt();
}

const char* JsonFile::getStringValue(const char* key) {	//returns the value associated with 'key' as a c-string
	assert((*doc)[key].IsString());						//make sure value is a string
	return (*doc)[key].GetString();
}

void JsonFile::edit(const char* key, int newVal) {		//change the value associated with 'key' to int 'newVal'
	assert(doc->HasMember(key));						//make sure the key exists
	(*doc)[key].SetInt(newVal);
}

void JsonFile::edit(const char* key, string newVal) {			//change the value associated with 'key' to string 'newVal'
	assert(doc->HasMember(key));								//make sure the key exists
	(*doc)[key].SetString(newVal.c_str(), newVal.length());
}
