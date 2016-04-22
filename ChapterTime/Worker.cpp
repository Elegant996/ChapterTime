#include "Worker.h"

//Constructor.
Worker::Worker(vector<string> parameters)
{
	this->parameters = parameters;
	this->sourceFPS = 23.976f;
	this->destinationFPS = 23.976f;
}

//Destructor.
Worker::~Worker()
{
}

void Worker::run()
{
	//Get all tasks based on parameters.
	vector<Task> tasks = parser.parseParameters(parameters);
	
	//No tasks; finished.
	if (tasks.empty())
		return;

	//Generate all chapters from tasks.
	vector<Chapter> chapters = processTasks(tasks);

	//No chapters; finished.
	if (chapters.empty())
		return;

	//Output some info.
	cout << "Operation complete." << endl;
	
}

//Process a list of tasks.
vector<Chapter> Worker::processTasks(vector<Task> tasks)
{
	//List of chapters.
	vector<Chapter> chapters;
	
	//Complete all tasks.
	for (auto &task : tasks)
	{
		//Handle all the different tasks.
		switch (task.getType())
		{
		case Task::Parse:
			//Output some info.
			cout << "Extracting chapters from \"" << parser.getFilePath() << "\"..." << endl;

			//Get chapters from file.
			if (parser.getFormat() == Parser::Format::OGM)
				chapters = parser.parseOGM();
			else
				chapters = parser.parseXML();

			//No chapters means no tasks; clear and return our empty list of chapters.
			if (chapters.empty())
			{
				tasks.clear();
				cout << "ERROR: File has an invalid format or does not exist!" << endl;
				return chapters;
			}

			break;
		case Task::Delay:
			//Output some info.
			cout << "Applying " << task.getValue() << "ms delay to chapters..." << endl;

			//Add the delay to all chapters except the first.
			for (auto chapter = next(chapters.begin()); chapter != chapters.end(); ++chapter)
				(*chapter) += stoi(task.getValue());

			break;
		case Task::Source:
			//Change the source FPS.
			sourceFPS = stof(task.getValue());

			break;
		case Task::ChangeTo:
			//Change the destination FPS.
			destinationFPS = stof(task.getValue());

			//Adjust timings if source is different than destination.
			if (sourceFPS != destinationFPS)
			{
				//Output some info.
				cout << "Changing FPS from " << sourceFPS << " to " << destinationFPS << endl;

				//Apply change to every chapter.
				for (auto &chapter : chapters)
					chapter *= (destinationFPS / sourceFPS);
			}

			break;
		case Task::Zeroize:
			cout << "'Zeroizing' chapters..." << endl;

			//Delay all chapters based on the first one.
			for (auto chapter = chapters.rbegin(); chapter != chapters.rend(); ++chapter)
				(*chapter) -= chapters.front();

			break;
		case Task::RemoveLast:
			cout << "Removing last chapter..." << endl;

			//Remove the last chapter.
			chapters.erase(chapters.end() - 1);

			//Remove XML's last chapter.
			if (parser.getFormat() == Parser::Format::XML)
				parser.removeLastXML();

			break;
		case Task::Compose:
			//Output some info.
			cout << "Writing chapters to file..." << endl;

			//Save changes to the file.
			if (parser.getFormat() == Parser::Format::OGM)
				parser.composeOGM(chapters);
			else
				parser.composeXML(chapters);

			break;
		}
	}

	return chapters;
}
