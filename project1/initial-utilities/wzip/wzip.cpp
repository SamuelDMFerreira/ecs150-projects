#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <istream>


void processFile(const char* filePath)
{
	int fileDescriptor = open(filePath, O_RDONLY);
	if (fileDescriptor < 0)
	{
		std::cerr << "cannot open file" << std::endl;
		exit(1);
	}

	close(fileDescriptor);
}

int main(int argc, char* argv[])
{
	// error if not enough arguments
	if (argc < 2)
	{
		std::cerr << "wzip: file1 [file2 ...]" << std::endl;
		exit(1);
	}

	// go through each file and process
	for (int arg = 1; arg < argc; arg++)
	{
		const char* filePath = argv[arg];
		processFile(filePath);
	}
}
