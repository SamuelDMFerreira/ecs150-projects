#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <ostream>

void printFile(const char* filePath)
{
	// check if file can be opened
	int fileDescriptor = open(filePath, O_RDONLY);
	if (fileDescriptor < 0)
	{
		std::cout << "wcat: cannot open file" << std::endl;
		exit(1);
	}
	
	// print out full file until end of file line
	void *buffer = malloc(sizeof(char));
	int bytesRead = sizeof(char);
	while (read(fileDescriptor, buffer, bytesRead) > 0)
	{
		write(1, buffer, bytesRead);
	}
	free(buffer);	

	close(fileDescriptor);	

}

int main(int argc, char* argv[])
{
	// If user doesn't specify parameters 
	if (argc <= 1)
	{
		return 0;
	}
	
	for (int argNum = 1; argNum < argc; argNum++)
	{
		const char* filePath = argv[argNum];
		printFile(filePath);
	}
}
