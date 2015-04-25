#pragma once

#include <string>

using namespace std;

class Task
{
public:
	enum Type
	{
		Parse,
		Delay,
		Source,
		ChangeTo,
		RemoveLast,
		Compose
	};

	Task(int type, string value="");
	~Task();

	int getType();
	string getValue();

	void setType(int type);
	void setValue(string value);

private:
	int type;
	string value;
};

