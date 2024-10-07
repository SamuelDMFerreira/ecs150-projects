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


// purpose: handles the process the ls argument and executing
// returns number of arguments
int handleLS(char** argv, int argc, stringstream& cSStrm)
{
	string lsPath; // check for right ls string path
	if (0 <= access("/bin/ls", X_OK))
	{
		lsPath = "/bin/ls";
	}
	else if (0 <= access("/usr/bin/ls", X_OK))
	{
		lsPath = "/usr/bin/ls";
	}
	else
	{
		exit(1);
	}
	
	// put file path for ls into arguments
	char* char_array = new char[lsPath.length()];
	strcpy(char_array, lsPath.c_str());
	argv[0] = char_array;
	argc++;
	// loop through for other args
	argc = processArguments(argv, argc, cSStrm);
	executeArgument(argv);
	return argc;
}


void processCommand(string commandStr, char** cmdPaths, int& cmdPathLen)
{
	// arguments for command
	char** argv;
	argv = (char**)malloc(sizeof(char*));
	int argc = 0;	
	stringstream cSStrm(commandStr);
	string currW;
	cSStrm >> currW;

	if (commandStr == "exit") // this should be called first
	{
		exit(0);
	}
	else if (currW == "ls")
	{
		argc = handleLS(argv, argc, cSStrm);
	}
	else if (currW == "cd")
	{
		cSStrm >> currW;
		char* char_array = (char*)malloc(currW.length() * sizeof(char));
		strcpy(char_array, currW.c_str());
		if (cSStrm >> currW)
		{
			cout << "too many arguments for cd" << endl;
			exit(1);
		}
		chdir(char_array);
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
				argc++;
			}
		}
		if (commandPath == "")
		{
			cout << "command doesn't exist in current directory" << endl;
			exit(1);
		}
		else
		{
			argc = processArguments(argv, argc, cSStrm);
			executeArgument(argv);
		}

	}

	
	// free arguments
	for (int x = 0; x < argc; x++)
	{
		free(argv[x]);
	}
	free(argv);
}

int main(int argc, char* argv[])
{
	// error if more than 2 arguments
	if (argc > 2)
	{
		cout << "./wish [batch file]" << endl;
		return 1;
	}
	
	// set starting base directory to /
	chdir("/");
	// make array of possible program paths, can be change by path command
	char** cmdPaths = (char**)malloc(sizeof(char*));
	int cmdPathLen = 1;
		
	if (argc == 1)
	{
		string commandStr = "";
		// command fetching while loop
		while(commandStr != "exit")
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
		while(!input.eof() && commandStr != "exit")
		{
			getline(input, commandStr);
			processCommand(commandStr, cmdPaths, cmdPathLen);
		}
	}
	return 0;
}
