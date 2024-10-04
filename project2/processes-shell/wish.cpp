#include <iostream>

int main(int argc, char* argv[])
{
	// error if more than 2 arguments
	if (argc > 2)
	{
		std::cout << "./wish [batch file]" << std::endl;
		return 1;
	}

	// command fetching while loop
	/*string command = "";	
	while(command != "exit")
	{
		
	}*/

	return 0;
}
