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

void processCommand(string commandStr)
{
	// arguments for command
	char** argv;
	argv = (char**)malloc(sizeof(char*));
	int argc = 1;	
	stringstream cSStrm(commandStr);
	string currW;
	cSStrm >> currW;

	if (commandStr == "exit") // this should be called first
	{
		exit(0);
	}
	else if (currW == "ls")
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
		// loop through for other args
		while(cSStrm >> currW)
		{
			argc++;
			argv = (char**)realloc(argv, argc * sizeof(char*));	
			char_array = new char[currW.length()];
			strcpy(char_array, currW.c_str());
			argv[argc - 1] = char_array;
		}
	}

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
