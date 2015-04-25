#include "Task.h"

//Constructor.
Task::Task(int type, string value)
{
	this->type = type;
	this->value = value;
}

//Destructor.
Task::~Task()
{
}

//Get Type.
int Task::getType()
{
	return type;
}

//Get Value.
string Task::getValue()
{
	return value;
}

//Set Type.
void Task::setType(int type)
{
	this->type = type;
}

//Set Value.
void Task::setValue(string value)
{
	this->value = value;
}
