#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void processCommand(string commandStr)
{
	// arguments for command
	//char** argv;
	stringstream cSStrm(commandStr);
	string currW;
	cSStrm >> currW;

	if (commandStr == "exit") // this should be called first
	{
		exit(0);
	}
	else if (currW == "ls")
	{
		cout << "epic LS call" << endl;
	}

}

int main(int argc, char* argv[])
{
	// error if more than 2 arguments
	if (argc > 2)
	{
		cout << "./wish [batch file]" << endl;
		return 1;
	}
	
	
	if (argc == 1)
	{
		string commandStr = "";
		// command fetching while loop
		while(commandStr != "exit")
		{
			cout << "wish> ";
			getline(cin, commandStr);
			processCommand(commandStr);
		}
	}
	else
	{
		ifstream input;
		input.open(argv[1]);
		string commandStr = "";
		// command fetching while loop
		while(!input.eof() && commandStr != "exit")
		{
			getline(input, commandStr);
			processCommand(commandStr);
		}
	}
	return 0;
}
