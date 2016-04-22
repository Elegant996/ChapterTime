#pragma once

#include <string>

#include "Time.h"

using namespace std;

class Chapter
{
public:
	Chapter(Time time, int index = 0, string name = "");
	Chapter(const Chapter &chapter);
	~Chapter();

	Chapter& operator-=(const Chapter &right);
	Chapter& operator-(const Chapter &right);
	Chapter& operator*=(const float &right);
	Chapter& operator*(const float &right);
	Chapter& operator+=(const int &right);
	Chapter& operator+(const int &right);

	int getIndex();
	Time getTime();
	string getName();

	void setIndex(int index);
	void setTime(Time time);
	void setName(string name);

private:
	int index;
	Time time;
	string name;
};

