#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <ostream>

int main(int argc, char* argv[])
{
	// If user doesn't specify parameters 
	if (argc <= 1)
	{
		return 0;
	}
	
	const char* filePath = argv[1];
	// check if file can be opened
	int fileDescriptor = open(filePath, O_RDONLY);
	if (fileDescriptor < 0)
	{
		std::cout << "wcat: cannot open file" << std::endl;
		exit(1);
	}

	close(fileDescriptor);	

}
