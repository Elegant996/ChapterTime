#include "Chapter.h"

//Constructor
Chapter::Chapter(Time time, int index, string name)
{
	this->time = time;
	this->index = index;
	this->name = name;
}

//Copy constructor
Chapter::Chapter(const Chapter &chapter)
{
	this->time = chapter.time;
	this->index = chapter.index;
	this->name = chapter.name;
}

//Destructor
Chapter::~Chapter()
{
}

//Overload operator-=.
Chapter& Chapter::operator-=(const Chapter &right)
{
	//Use the - operator overload.
	*this = *this - right;

	return *this;
}

//Overload operator+.
Chapter& Chapter::operator-(const Chapter &right)
{
	//Use Time's -= operator overload.
	(*this).time -= right.time;

	return (*this);
}

//Overload operator*=.
Chapter& Chapter::operator*=(const float &right)
{
	//Use the + operator overload.
	*this = *this * right;

	return *this;
}

//Overload operator+.
Chapter& Chapter::operator*(const float &right)
{
	//Use Time's += operator overload.
	(*this).time *= right;

	return (*this);
}

//Overload operator+=.
Chapter& Chapter::operator+=(const int &right)
{
	//Use the + operator overload.
	*this = *this + right;

	return *this;
}

//Overload operator+.
Chapter& Chapter::operator+(const int &right)
{
	//Use Time's += operator overload.
	(*this).time += right;

	return (*this);
}

//Get Index.
int Chapter::getIndex()
{
	return index;
}

//Get Time.
Time Chapter::getTime()
{
	return time;
}

//Get Name.
string Chapter::getName()
{
	return name;
}

//Set Index.
void Chapter::setIndex(int index)
{
	this->index = index;
}

//Set Time.
void Chapter::setTime(Time time)
{
	this->time = time;
}

//Set Name.
void Chapter::setName(string name)
{
	this->name = name;
}
