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

	void* fourByteCountBuffer = malloc(4 * sizeof(char));
	void* byteCharacterBuffer = malloc(sizeof(char));
	*((uint32_t*)fourByteCountBuffer) = 1;
	
	// go through file and check for repeating values
	int bytesRead = read(fileDescriptor, byteCharacterBuffer, sizeof(char));
	while (bytesRead > 0)
	{
		char previousReadByte = *((char*)byteCharacterBuffer);
		bytesRead = read(fileDescriptor, byteCharacterBuffer, sizeof(char));
		if (previousReadByte == *((char*)byteCharacterBuffer))
		{
			*((uint32_t*)fourByteCountBuffer) += 1;
		}
		else
		{
			// if characters stop matching print out the information on previous character sequence
			write(STDOUT_FILENO, fourByteCountBuffer, 4 * sizeof(char));
			void* printPreviousBuffer = malloc(sizeof(char));
			*((char*)printPreviousBuffer) = previousReadByte;
			write(STDOUT_FILENO, printPreviousBuffer, sizeof(char));
			free(printPreviousBuffer);
			*((uint32_t*)fourByteCountBuffer) = 1;
		}
	}

	free(fourByteCountBuffer);
	free(byteCharacterBuffer);

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
