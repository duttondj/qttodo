// Danny Dutton
// ECE 3574
// 2015-08-27
// Project 0: Todo
// This is a todo utility that can keep track of a todo list.
// The list is stored in a text file named todo.txt but any name can be used.
// Items are displayed using "list". They are numbered. Completed tasks are put at the bottom.
// Items are added using "add Name of task here" or 'add "name of task here"'.
// Items are marked done by using "do #" where # is the item number from the list output.

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
	string row;						// Line from text file
	int numNotDone = 0;				// iterators for counting line items
	int numDone = 0;				// iterators for counting line items
	int n = 2;						// Used to denote the begining of the string to be added
	int itemnum;					// Item number to be marked as done
	string newtask = "";			// Task to be added to list
	string action;					// Action to be performed on todo list
	string filename = "todo.txt";	// File name to use for todo list
	string fflag = "-f";			// Use this so we can compare strings with c-style strings

	vector<string> notdone;			// list of incomplete tasks
	vector<string> done;			// list of complete tasks

	// Check if there are any command line arguments. If not: display error and exit main.
	try
	{
		// Argument count should be two: run command at least one action.
		if (argc < 2)
		{
			throw "Usage: todo [-f file] command";
		}
		if (!(fflag.compare(argv[1])) && argc < 4)
		{
			throw "Usage: todo [-f file] command";
		}
	}
	catch (const char* msg) {
		cerr << msg << endl;
		return 0;
	}

	// Open file passed from command line arguments or just "todo.txt" if not specified
	ifstream inFile;
	if ( !(fflag.compare(argv[1])))
	{
		filename = argv[2];
	}

	// Check if file was opened correctly. If not: display error, free memory and exit main.
	try
	{
		inFile.open(filename.c_str());

		if (!inFile.good())
		{
			throw "File failed to open (file not found).";
		}
	}
	catch (const char* msg) {
		cerr << msg << endl;
		return 0;
	}

	// Read all the lines in the file and add the tasks to the right vector
	while(getline(inFile, row))
	{
		if (row.substr(0,1) == "[")
		{
			if(row.substr(0,3) == "[ ]")
			{
				notdone.push_back(row);
				numNotDone++;
			}
			else if (row.substr(0,3) == "[x]")
			{
				done.push_back(row);
				numDone++;
			}
		}
	}

	// Close file
	inFile.close();

	// What are we being told to do? add, do, list?
	// Check if using custom file
	if (!(fflag.compare(argv[1])))
	{
		action = argv[3];
		cout << "USING CUSTOM FILE" << endl;
	}
	else
	{
		action = argv[1];
	}

	// print out the whole list
	if (action == "list")
	{
		for (int k = 0; k < (numNotDone+numDone); k++)
		{
			// Print the not done list
			if (k < numNotDone)
			{
				cout << k+1 << ":" << notdone[k] << endl;
			}
			/// Print the done list
			else
			{
				cout << k+1 << ":" << done[k - numNotDone] << endl;
			}		
		}

		// We do not want to touch the file again so let's exit out
		return 0;
	}

	// add new task
	else if (action == "add")
	{
		// Start the arg count a little later if using custom filename
		if (!(fflag.compare(argv[1])))
		{
			n = 4;
		}
		
		// Check to see if there is actually a new task
		try
		{
			if (n >= argc)
			{
				throw "No new task to be added";
			}
		}
		catch (const char* msg) {
			cerr << msg << endl;
			return 0;
		}
		
		// Add the first argument and a checkbox
		newtask.append("[ ] ");
		newtask.append(argv[n]);
		n++;

		// Add any extra "arguments" that were seperated by spaces and not quoted
		for (; n < argc; n++)
		{
			newtask.append(" ");
			newtask.append(argv[n]);
		}

		// Add new task and inc count
		notdone.push_back(newtask);
		numNotDone++;
	}

	// do a task
	else if (action == "do")
	{
		
		// Choose the right arg to use as the task number
		if (!(fflag.compare(argv[1])))
		{
			itemnum = atoi(argv[4]) - 1;
		}
		else
		{
			itemnum = atoi(argv[2]) - 1;
		}
		
		// Check if task is already done or if it exists
		try
		{
			if (itemnum >= numNotDone)
			{
				throw "That task number does not exist or is already complete";
			}
		}
		catch (const char* msg) {
			cerr << msg << endl;
			return 0;
		}

		// Check the box, add task to done list, remove from not done list, dec not done count, inc done count
		notdone[itemnum].replace(1,1,"x");
		done.push_back(notdone[itemnum]);
		notdone.erase(notdone.begin()+itemnum);
		numNotDone--;
		numDone++;
	}

	// None of the correct commands were supplied so throw an error
	else
	{
		cerr << "No command supplied" << endl << "Usage: todo [-f file] command" << endl;
		return 0;
	}

	// Write the lists to the file after modifying
	ofstream outFile(filename.c_str());
	for (int k = 0; k < (numNotDone + numDone); k++)
	{
		if (k<numNotDone)
		{
			outFile << notdone[k] << endl;
		}
		else
		{
			outFile << done[k-numNotDone] << endl;
		}
	}

	// Close the output file
	outFile.close();

}