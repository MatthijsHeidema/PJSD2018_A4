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
	doc = new Document;
	file = new fstream;
	if (updateDoc()) {
		doc->Parse("{\"NULL\" : \"null\"}");
		ofstream newFile;
		newFile.open(filePath);
		newFile.close();
	}
	updateFile();
}

JsonFile::JsonFile(const char* filePath, const char* jsonContent): filePath(filePath){
	doc = new Document;
	file = new fstream;

	doc->Parse(jsonContent);
	ofstream newFile;
	newFile.open(filePath);
	newFile.close();

	updateFile();
}

int JsonFile::updateDoc() {	//Updates the doc with the current contents of file
	string temp;
	string line;

	file->open(filePath);
	if (file->is_open()) {

		while (getline(*file, line)) {
			temp += line;
		}
		file->close();
		doc->Parse(temp.c_str());

		return 0;

	} else {
		return 1;
	}
}

int JsonFile::updateFile() { //Updates the file with the current contents of doc
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc->Accept(writer);

	file->open(filePath, ios::out | ios::trunc);
	if (file->is_open()) {
		*file << buffer.GetString() << endl;
		file->close();

		return 0;

	} else {
		return 1;
	}
}

void JsonFile::print() {

	assert(!updateDoc());
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc->Accept(writer);

	cout << buffer.GetString() << endl;
}

void JsonFile::fill(const char* input) {
	doc->Parse(input);
	assert(!updateFile());
}

int JsonFile::getIntValue(const char* key) {
	assert((*doc)[key].IsInt());
	return (*doc)[key].GetInt();
}

const char* JsonFile::getStringValue(const char* key) {
	assert((*doc)[key].IsString());
	return (*doc)[key].GetString();
}

void JsonFile::edit(const char* key, int newVal) {
	assert(doc->HasMember(key));
	(*doc)[key].SetInt(newVal);
}

void JsonFile::edit(const char* key, string newVal) {
	assert(doc->HasMember(key));
	(*doc)[key].SetString(newVal.c_str(), newVal.length());
}
