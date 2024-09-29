#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <istream>
#include <stdint.h>


void processFile(const char* filePath)
{
	int fileDescriptor = open(filePath, O_RDONLY);
	if (fileDescriptor < 0)
	{
		std::cerr << "cannot open file" << std::endl;
		exit(1);
	}

	void* numBuffer = malloc(4 * sizeof(char));
	void* charBuffer = malloc(sizeof(char));
	
	int readByte = read(fileDescriptor, numBuffer, 4 * sizeof(char));
	readByte = read(fileDescriptor, charBuffer, sizeof(char));

	while(readByte > 0)
	{
		for (uint32_t x = 0; x < (*((uint32_t*)numBuffer)); x++)
		{
			write(STDOUT_FILENO, charBuffer, sizeof(char));
		}
		readByte = read(fileDescriptor, numBuffer, 4 * sizeof(char));
		readByte = read(fileDescriptor, charBuffer, sizeof(char));
	}

	free(numBuffer);
	free(charBuffer);

	close(fileDescriptor);	

}

int main(int argc, char* argv[])
{	
	// error if not enough arguments
	if (argc < 2)
	{
		std::cerr << "wunzip: file1 [file2 ...]" << std::endl;
		exit(1);
	}

	// go through each file and process
	for (int arg = 1; arg < argc; arg++)
	{
		const char* filePath = argv[arg];
		processFile(filePath);
	}
}
