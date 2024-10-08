// c++ helper classes
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
// system call
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

// executes arguments give a location
void executeArgument(char** argv)
{
	// fork and run process
	int rc = fork();
	if (rc < 0)
	{
		// fork fricked
	}
	else if (rc == 0)
	{
		// if child
		execv(argv[0], argv);
		exit(0);
	}
	else
	{
		// if parent
		wait(NULL);		
	}
}

// process arguments from string and adds them to the arguments array, this excludes the command arguments itself
// returns number of arguments
int processArguments(char** argv, int argc, stringstream& cSStrm)
{
	char* char_array;
	string currW;
	while(cSStrm >> currW)
	{
		argc++;
		argv = (char**)realloc(argv, argc * sizeof(char*));	
		char_array = new char[currW.length()];
		strcpy(char_array, currW.c_str());
		argv[argc - 1] = char_array;
	}
	return argc;
}

// handles non built in commands
void handleCMD(string currW, stringstream& cSStrm, char** cmdPaths, int& cmdPathLen)
{
	// create array of arguments for the command
	char** argv;
	argv = (char**)malloc(sizeof(char*));
	int argc = 0;	
	
	// check if the command exist within the command path, alway use the last command
	string commandPath = "";
	for (int x = 1; x < cmdPathLen; x++)
	{
		string potCommandPath(cmdPaths[x]);
		stringstream ss;
		ss << potCommandPath << "/" << currW;
		potCommandPath = ss.str();
		char* char_array = (char*)malloc(potCommandPath.length() * sizeof(char));
		strcpy(char_array, potCommandPath.c_str());
		if (0 <= access(char_array, X_OK))
		{
			commandPath = potCommandPath;
			argv[0] = char_array;
			argc = 1;
		}
	}
	// error if not in any of the command paths
	if (commandPath == "")
	{
		char error_message[30] = "An error has occurred\n";
    		write(STDERR_FILENO, error_message, strlen(error_message));
	}
	// execute the command if it exist
	// parse the rest of input and execute command with parameters
	else
	{
		argc = processArguments(argv, argc, cSStrm);
		executeArgument(argv);
	}
	
	// free arguments
	for (int x = 0; x < argc; x++)
	{
		free(argv[x]);
	}
	free(argv);
}


void processCommand(string commandStr, char** cmdPaths, int& cmdPathLen)
{
	// arguments for command
	stringstream cSStrm(commandStr);
	string currW;
	cSStrm >> currW;

	if (commandStr == "exit") // this should be called first
	{
		exit(0);
	}
	else if (currW == "cd")
	{
		cSStrm >> currW;
		char* char_array = (char*)malloc(currW.length() * sizeof(char));
		strcpy(char_array, currW.c_str());
		if (cSStrm >> currW)
		{
			char error_message[30] = "An error has occurred\n";
    			write(STDERR_FILENO, error_message, strlen(error_message));
		}
		else
		{
			chdir(char_array);
		}
		free(char_array);
	}
	else if (currW == "path")
	{
		// reset to intial state in program
		for (int x = 1; x < cmdPathLen; x++)
		{
			free(cmdPaths[x]);
		}
		cmdPaths = (char**)realloc(cmdPaths, sizeof(char*));
		cmdPathLen = 1;
		// I reuse the processArgument function
		// Because it bascially does what I want
		cmdPathLen = processArguments(cmdPaths, cmdPathLen, cSStrm);	
	}
	else
	{
		handleCMD(currW, cSStrm, cmdPaths, cmdPathLen);
	}

	
}

int main(int argc, char* argv[])
{
	
	// set starting base directory to /
	chdir("/");
	// make array of possible program paths, can be change by path command
	char** cmdPaths = (char**)malloc(sizeof(char*));
	int cmdPathLen = 1;
	
	if (argc == 1)
	{
		string commandStr = "";
		// command fetching while loop
		while(!cin.eof())
		{
			cout << "wish> ";
			getline(cin, commandStr);
			processCommand(commandStr, cmdPaths, cmdPathLen);
		}
	}
	else
	{
		ifstream input;
		input.open(argv[1]);
		string commandStr = "";
		// command fetching while loop
		while(!input.eof())
		{
			getline(input, commandStr);
			processCommand(commandStr, cmdPaths, cmdPathLen);
		}
	}
	return 0;
}
