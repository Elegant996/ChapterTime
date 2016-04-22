#include "Parser.h"

//Constructor.
Parser::Parser()
{
	filePath = "";
	format = Format::Unknown;
}

//Destructor.
Parser::~Parser()
{
}

//Parse command line parameters.
vector<Task> Parser::parseParameters(vector<string> parameters)
{
	vector<Task> tasks;
	//List of all valid parameters we can accept.
	regex validOffset("^(-|\\+)[0-9]+ms$");
	regex validSource("^(-){1}[0-9]{2}.[0-9]{3}$");
	regex validChange("^(-)changeTo{1}[0-9]{2}.[0-9]{3}$");
	regex_constants::match_flag_type fl = regex_constants::match_default;

	//Handle parameters.
	switch (parameters.size())
	{
	case 0:
		//No input or parameters; display all options available.
		cout << "ChapterTime v1.02 by Shane Panke" << endl;
		cout << "A CLI-based application that edits OGM and XML style chapters." << endl << endl;
		cout << "chaptertime sourcefile [-options]" << endl << endl;
		cout << "Options:" << endl;
		cout << setfill(' ') << setw(18) << left << "+/-100ms" << "apply a positive or negative chapter delay" << endl;
		cout << setfill(' ') << setw(18) << left << "-23.976/..." << "define source fps to be \"23.976\" (default), \"24.000\", ..." << endl;
		cout << setfill(' ') << setw(18) << left << "-changeTo24.000" << "change source fps to \"23.976\" (default), \"24.000\", ..." << endl;
		cout << setfill(' ') << setw(18) << left << "-zeroize" << "applies the first chapter as negative delay to all chapters" << endl;
		cout << setfill(' ') << setw(18) << left << "-removeLast" << "removes the last chapter in the source" << endl;
		cout << endl;
		break;
	case 1:
		//Check that we had a valid file before reporting that we have no parameters.
		file.open(parameters.front());
		if (file.good())
			cerr << "No options specified." << endl;
		else
			cerr << "Invalid input." << endl;
		break;
	default:
		//Get our file path for later.
		filePath = parameters.front();

		//Check that we have a valid file.
		file.open(filePath);
		if (!file.good())
		{
			//File path is not valid.
			cerr << "Invalid input." << endl;
			break;
		}

		//Store our file path and format.
		format = regex_search(filePath, regex(".xml$")) ? Format::XML : Format::OGM;

		//Add task and remove first parameter.
		tasks.insert(tasks.end(), Task(Task::Parse));
		parameters.erase(parameters.begin());

		//Parse each parameter and add it to the task list.
		for (auto &parameter : parameters)
		{
			smatch match;
			if (regex_match(parameter, validOffset))
			{
				//Get time offset now that we're sure it's valid.
				regex_search(parameter, match, (regex)"(-|\\+)[0-9]+");
				tasks.insert(tasks.end(), Task(Task::Delay, match.str()));
			}
			else if (regex_match(parameter, validSource))
			{
				//Get frame rate now that we're sure it's valid.
				regex_search(parameter, match, (regex)"[0-9]{2}.[0-9]{3}");
				tasks.insert(tasks.end(), Task(Task::Source, match.str()));
			}
			else if (regex_match(parameter, validChange))
			{
				//Get destination frame rate now that we're sure it's valid.
				regex_search(parameter, match, (regex)"[0-9]{2}.[0-9]{3}");
				tasks.insert(tasks.end(), Task(Task::ChangeTo, match.str()));
			}
			else if (parameter == "-zeroize")
				//Queue 'zeroizing' all chapters
				tasks.insert(next(tasks.begin()), Task(Task::Zeroize));
			else if (parameter == "-removeLast")
				//Queue the removal of the last Chapter.
				tasks.insert(next(tasks.begin()), Task(Task::RemoveLast));
			else
			{
				//Unknown parameter; clear tasks.
				tasks.clear();
				cout << "Invalid parameter(s)." << endl;
				break;
			}
		}

		//Compose the file after all tasks are complete.
		tasks.insert(tasks.end(), Task(Task::Compose));

		break;
	}

	return tasks;
}

//Get the file path.
string Parser::getFilePath()
{
	return filePath;
}

//Get the chapter format.
Parser::Format Parser::getFormat()
{
	return format;
}

//Parse OGM for chapters.
vector<Chapter> Parser::parseOGM()
{
	//Create a chapter list for us to store the information.
	vector<Chapter> chapters;

	//Prepare to handle the file.
	ifstream file;
	file.exceptions(ifstream::failbit | ifstream::badbit);
	//Valid format for chapters.
	regex UTF8("^\xEF\xBB\xBF");
	regex validTime("^CHAPTER([0-9]){2}=([0-9]){2}:([0-9]){2}:([0-9]){2}.([0-9]){3}$");
	regex validName("^CHAPTER([0-9]){2}NAME=(.*)$");

	//Attempt to open the file.
	try
	{
		//Open the file.
		file.open(filePath);

		//Strings for time and name when read.
		string timeLine, nameLine;
		//Read the file.
		while (file.good() && getline(file, timeLine))
		{
			if (regex_search(timeLine, UTF8))
				timeLine = timeLine.substr(3);

			//Check if the lines follows our desired format.
			smatch timeMatch, nameMatch;
			if (!regex_search(timeLine, timeMatch, validTime))
			{
				//Not a chapter or invalid format.
				chapters.clear();

				return chapters;
			}

			//Store the chapter's index and time since it's location is static.
			int index = stoi(timeMatch.str().substr(7, 2));
			int hours = stoi(timeMatch.str().substr(10, 2));
			int minutes = stoi(timeMatch.str().substr(13, 2));
			int seconds = stoi(timeMatch.str().substr(16, 2));
			int milliseconds = stoi(timeMatch.str().substr(19, 3));
			Time time(hours, minutes, seconds, milliseconds);

			getline(file, nameLine);
			if (!regex_search(nameLine, nameMatch, validName))
			{
				//Not a chapter or invalid format.
				chapters.clear();

				return chapters;
			}

		//Get name of chapter since it's location is also static.
		string name = nameMatch.str().substr(14, nameLine.length() - 1);

		//Now we can create the chapter!
		chapters.insert(chapters.end(), Chapter(time, index, name));
		}
	}
	//Failed to open the file.
	catch (ifstream::failure e)
	{
		//Close the file if it's open; we're done reading.
		if (file.is_open())
			file.close();

		//Return our list of chapters.
		return chapters;
	}

	//Close the file, we're done reading.
	file.close();

	//Return our list of chapters.
	return chapters;
}

//Parse XML for chapters.
vector<Chapter> Parser::parseXML()
{
	//Create a chapter list for us to store the information.
	vector<Chapter> chapters;

	//Setup for file handling.
	xml_parse_result result = doc.load_file(filePath.c_str());

	//File was not opened successfully.
	if (result.status != xml_parse_status::status_ok)
		return chapters;

	//Attempt to read the file.
	try
	{
		//Find all chapters in the XML.
		xml_node entries = doc.child("Chapters").child("EditionEntry");
		for (xml_node entry = entries.first_child(); entry; entry = entry.next_sibling())
		{
			if (strcmp(entry.name(), "ChapterAtom") == 0)
			{
				//Store the chapter's time since it's location is static.
				string timeNode = entry.child("ChapterTimeStart").text().get();
				int hours = stoi(timeNode.substr(0, 2));
				int minutes = stoi(timeNode.substr(3, 2));
				int seconds = stoi(timeNode.substr(6, 2));
				int milliseconds = stoi(timeNode.substr(9, 3));
				Time time(hours, minutes, seconds, milliseconds);

				//Now add the chapter to the list.
				Chapter chapter(time);
				chapters.insert(chapters.end(), chapter);
			}
		}
	}
	//Error while reading the file.
	catch (ifstream::failure e)
	{
		//Return our list of chapters.
		return chapters;
	}

	//Return our list of chapters.
	return chapters;
}

//Remove last chapter from XML.
void Parser::removeLastXML()
{
	//Grab the last chapter entry and remove it.
	xml_node lastChapter;
	xml_node entries = doc.child("Chapters").child("EditionEntry");
	for (xml_node entry = entries.last_child(); entry; entry = entry.previous_sibling())
		if (strcmp(entry.name(), "ChapterAtom") == 0)
		{
			//Store location of chapter and leave this loop.
			lastChapter = entry;
			break;
		}

	//Remove the last chapter.
	entries.remove_child(lastChapter);
}

//Compose file after all parameters have been applied.
void Parser::composeOGM(vector<Chapter> chapters)
{
	ofstream file;
	file.open(filePath, ofstream::out | ofstream::trunc);

	//Write the OGM file from scratch.
	for (auto &chapter : chapters)
	{
		file << "CHAPTER" << setfill('0') << setw(2) << chapter.getIndex() << "="
			<< setfill('0') << setw(2) << chapter.getTime().getHours() << ":"
			<< setfill('0') << setw(2) << chapter.getTime().getMinutes() << ":"
			<< setfill('0') << setw(2) << chapter.getTime().getSeconds() << "."
			<< setfill('0') << setw(3) << chapter.getTime().getMilliseconds() << endl;
		file << "CHAPTER" << setfill('0') << setw(2) << chapter.getIndex() << "NAME=" << chapter.getName() << endl;
	}

	//Close the file; we're done!
	file.close();
}

//Compose file after all parameters have been applied.
void Parser::composeXML(vector<Chapter> chapters)
{
	//Find all chapters in the XML.
	xml_node entries = doc.child("Chapters").child("EditionEntry");
	for (xml_node entry = entries.first_child(); entry; entry = entry.next_sibling())
		if (strcmp(entry.name(), "ChapterAtom") == 0)
		{
			//Grab the lead chapter in the list.
			Chapter chapter = chapters.front();

			//Assemble new chapter time.
			stringstream chapterTime;
			chapterTime << setfill('0') << setw(2) << chapter.getTime().getHours() << ":"
				<< setfill('0') << setw(2) << chapter.getTime().getMinutes() << ":"
				<< setfill('0') << setw(2) << chapter.getTime().getSeconds() << "."
				<< setfill('0') << setw(3) << chapter.getTime().getMilliseconds() << "000000";

			//Update the chapter time.
			entry.child("ChapterTimeStart").text() = chapterTime.str().c_str();

			//Remove the chapter as we have processed it.
			chapters.erase(chapters.begin());
		}

	//Save changes to the file.
	doc.save_file(filePath.c_str());
}