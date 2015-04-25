#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <stdio.h>
#include <sstream>
#include <string>

#include "libs/pugixml/pugixml.hpp"

#include "Chapter.h"
#include "Task.h"

using namespace std;
using namespace pugi;

class Parser
{
public:
	enum Format
	{
		Unknown,
		OGM,
		XML
	};

	Parser();
	~Parser();

	vector<Task> parseParameters(vector<string> parameters);
	string getFilePath();
	Format getFormat();
	vector<Chapter> parseOGM();
	vector<Chapter> parseXML();
	void removeLastXML();
	void composeOGM(vector<Chapter> chapters);
	void composeXML(vector<Chapter> chapters);

private:
	string filePath;
	Format format;
	xml_document doc;
};

