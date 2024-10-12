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
/*int processArguments(char** argv, int argc, stringstream& cSStrm)
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
}*/

// handles non built in commands
/*void handleCMD(string currW, stringstream& cSStrm, char** cmdPaths, int& cmdPathLen)
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
}*/


/*void processCommand(string commandStr, char** cmdPaths, int& cmdPathLen)
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

	
}*/

// helper class to store all the information relating to commands
class Commands 
{
	public:
		char*** cmdStrs;
		int numOfCmds;
		int* numArgsPerCmd;

	Commands(char*** ncmdStrs, int nnumOfCmds, int* nnumArgsPerCmd)
	{
		cmdStrs = ncmdStrs;
		numOfCmds = nnumOfCmds;
		numArgsPerCmd = nnumArgsPerCmd;
	}

	void ttttest()
	{
		cout << "uss" << endl;
	}
};


// this method takes the user input string and processes it
// It will be split into commands which are seperated by an & on the line
// Each command, a char** array, are further sub divided into arguments
// each argument is a char* string.
Commands processCommands(string inputLine)
{
	char*** commands = (char***)malloc(sizeof(char**));
	int numOfCmds = 0;
	int currNumArgs = 0;
	int* numArgsPerCmd = (int*)malloc(sizeof(int));

	stringstream strm(inputLine);
	string currWord;

	while (strm >> currWord)
	{
		// allocate new command if the & shows up
		if (currWord == "&")
		{
			numArgsPerCmd[numOfCmds - 1] = currNumArgs;
			numOfCmds++;
			commands = (char***)realloc(commands, sizeof(char**) * numOfCmds);
			commands[numOfCmds - 1] = (char**)malloc(sizeof(char*));	
			numArgsPerCmd = (int*)realloc(numArgsPerCmd, numOfCmds * sizeof(int));
			currNumArgs = 0;
		}
		// if user input any commands on line put them in
		else if (numOfCmds == 0)
		{	
			numOfCmds++;
			commands[0] = (char**)malloc(sizeof(char*));
			numArgsPerCmd = (int*)realloc(numArgsPerCmd, numOfCmds * sizeof(int));
			currNumArgs++;
			char* char_array = new char[currWord.length()];
			strcpy(char_array, currWord.c_str());
			commands[numOfCmds - 1][currNumArgs - 1] = char_array;
		}
		// in any other case add argument/parameter to an individual command 
		else
		{
			currNumArgs++;
			commands[numOfCmds - 1] = (char**)realloc(commands[numOfCmds - 1], currNumArgs * sizeof(char*));
			char* char_array = new char[currWord.length()];
			strcpy(char_array, currWord.c_str());
			commands[numOfCmds - 1][currNumArgs - 1] = char_array;
					
		}
	}

	Commands cmds(commands, numOfCmds, numArgsPerCmd);

	return cmds;
}

void processLineInput(string inputLine, char** cmdPaths, int cmdPathLen)
{
	Commands commands = processCommands(inputLine);
	bool isChild = false;
	int currCmd;
	for (currCmd = 0; currCmd < commands.numOfCmds; currCmd++)
	{
		int pid = fork();
		if (pid == 0)
		{
			isChild = true;
			break;
		}
		else 
		{
			int status;
			waitpid(pid, &status, 0);
		}
	}

	if (isChild)
	{	
		cout << currCmd << endl;
		exit(0);
	}

}

int main(int argc, char* argv[])
{
	
	// set starting base directory to /
	chdir("/");
	// make array of possible program paths, can be change by path command
	char** cmdPaths = (char**)malloc(sizeof(char*));
	int cmdPathLen = 1;
	
	// just for testing when reorganizing my code 
	string path = "/bin"; 
	char* char_array = (char*)malloc(path.length() * sizeof(char));
	strcpy(char_array, path.c_str());
	cmdPaths[0] = char_array;

	
	if (argc == 1)
	{
		string inputLine = "";
		// command fetching while loop
		while(!cin.eof())
		{
			cout << "wish> ";
			getline(cin, inputLine);
			processLineInput(inputLine, cmdPaths, cmdPathLen);
		}
	}
	else
	{
		ifstream inputFile;
		inputFile.open(argv[1]);
		string inputLine = "";
		// command fetching while loop
		while(!inputFile.eof())
		{
			getline(inputFile, inputLine);
			processLineInput(inputLine, cmdPaths, cmdPathLen);
		}
	}
	return 0;
}
