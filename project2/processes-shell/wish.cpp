#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
	// error if more than 2 arguments
	if (argc > 2)
	{
		std::cout << "./wish [batch file]" << std::endl;
		return 1;
	}
	
	
	if (argc == 1)
	{
		std::string commandStr = "";
		// command fetching while loop
		while(commandStr != "exit")
		{
			std::cout << "wish> ";
			getline(std::cin, commandStr);	
		}
	}
	else
	{
		std::ifstream input;
		input.open(argv[1]);
		std::string commandStr = "";
		// command fetching while loop
		while(!input.eof() && commandStr != "exit")
		{
			getline(input, commandStr);	
		}
	}



	return 0;
}
