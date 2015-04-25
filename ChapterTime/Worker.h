#pragma once

#include "Parser.h"
#include "Task.h"

using namespace std;

class Worker
{
public:
	Worker(vector<string> parameters);
	~Worker();

	void run();

private:
	vector<Chapter> processTasks(vector<Task> tasks);

	vector<string> parameters;
	float sourceFPS, destinationFPS;
	Parser parser;
};

