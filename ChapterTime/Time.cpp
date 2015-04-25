#include "Time.h"

//Constructor
Time::Time(int hours, int minutes, int seconds, int milliseconds)
{
	this->hours = hours;
	this->minutes = minutes;
	this->seconds = seconds;
	this->milliseconds = milliseconds;
}

//Destructor
Time::~Time()
{
}

//Overload operator*=.
Time& Time::operator*=(const float &right)
{
	//Use the + operator overload.
	*this = *this * right;

	return *this;
}

//Overload operator*.
Time& Time::operator*(const float &right)
{
	//Get time in milliseconds.
	int totalTime = (*this).hours * 60 * 60 * 1000 + (*this).minutes * 60 * 1000 + (*this).seconds * 1000 + (*this).milliseconds;
	
	//Multiply it by the right side and round.
	totalTime = (int)roundf((float)totalTime * right);

	//Convert it back to standard format.
	(*this).milliseconds = totalTime % 1000;
	totalTime /= 1000;
	(*this).seconds = totalTime % 60;
	totalTime /= 60;
	(*this).minutes = totalTime % 60;
	totalTime /= 60;
	(*this).hours = totalTime;

	return (*this);
}

//Overload operator+=.
Time& Time::operator+=(const int &right)
{
	//Use the + operator overload.
	*this = *this + right;

	return *this;
}

//Overload operator+.
Time& Time::operator+(const int &right)
{
	//Get time in milliseconds.
	int totalTime = (*this).hours * 60 * 60 * 1000 + (*this).minutes * 60 * 1000 + (*this).seconds * 1000 + (*this).milliseconds;

	//Add the right or go to 0 if negative.
	totalTime = (totalTime + right) > 0 ? (totalTime + right) : 0;

	//Convert it back to standard format.
	(*this).milliseconds = totalTime % 1000;
	totalTime /= 1000;
	(*this).seconds = totalTime % 60;
	totalTime /= 60;
	(*this).minutes = totalTime % 60;
	totalTime /= 60;
	(*this).hours = totalTime;

	return (*this);
}

//Get Hours.
int Time::getHours()
{
	return hours;
}

//Get Minutes.
int Time::getMinutes()
{
	return minutes;
}

//Get Seconds.
int Time::getSeconds()
{
	return seconds;
}

//Get Millseconds.
int Time::getMilliseconds()
{
	return milliseconds;
}

//Set Hours.
void Time::setHours(int hours)
{
	this->hours = hours;
}

//Set Minutes.
void Time::setMinutes(int minutes)
{
	this->minutes = minutes;
}

//Set Seconds.
void Time::setSeconds(int seconds)
{
	this->seconds = seconds;
}

//Set Milliseconds.
void Time::setMilliseconds(int milliseconds)
{
	this->milliseconds = milliseconds;
}