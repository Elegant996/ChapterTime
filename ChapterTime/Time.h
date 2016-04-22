#pragma once

#include <cmath>

class Time
{
public:
	Time(int hours=0, int minutes=0, int seconds=0, int milliseconds=0);
	~Time();

	Time& operator-=(const Time &right);
	Time& operator-(const Time &right);
	Time& operator*=(const float &right);
	Time& operator*(const float &right);
	Time& operator+=(const int &right);
	Time& operator+(const int &right);

	int getHours();
	int getMinutes();
	int getSeconds();
	int getMilliseconds();

	void setHours(int hours);
	void setMinutes(int minutes);
	void setSeconds(int seconds);
	void setMilliseconds(int milliseconds);	

private:
	int hours;
	int minutes;
	int seconds;
	int milliseconds;
};

