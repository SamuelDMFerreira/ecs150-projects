#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <istream>
#include <stdint.h>

void processFile(const char* filePath, void* countBuffer, void* currCharBuffer, void* prevCharBuffer)
{
	int fileDescriptor = open(filePath, O_RDONLY);
	if (fileDescriptor < 0)
	{
		std::cerr << "cannot open file" << std::endl;
		exit(1);
	}

	int bytesRead;	
	// go through file and check for repeating values
	do
	{
		bytesRead = read(fileDescriptor, currCharBuffer, sizeof(char));

		if (bytesRead > 0 )
		{
			if (bytesRead > 0 && (*((char*)currCharBuffer) == *((char*)prevCharBuffer) || *((char*)prevCharBuffer) == '\0'))
			{
				*((uint32_t*)countBuffer) += 1;
			}
			else if (bytesRead > 0)
			{
				// if characters stop matching print out the information on previous character sequence
				write(STDOUT_FILENO, countBuffer, 4 * sizeof(char));
				write(STDOUT_FILENO, prevCharBuffer, sizeof(char));
				*((uint32_t*)countBuffer) = 1;
			}
			*((char*)prevCharBuffer) = *((char*)currCharBuffer);
		}
	} while (bytesRead > 0);	


	close(fileDescriptor);
}

int main(int argc, char* argv[])
{
	// error if not enough arguments
	if (argc < 2)
	{
		std::cout << "wzip: file1 [file2 ...]" << std::endl;
		exit(1);
	}
	
	void* countBuffer = malloc(4 * sizeof(char));
	*((uint32_t*)countBuffer) = 0;
	void* currCharBuffer = malloc(sizeof(char));
	void* prevCharBuffer = malloc(sizeof(char));
	*((char*)prevCharBuffer) = '\0';

	// go through each file and process
	for (int arg = 1; arg < argc; arg++)
	{
		const char* filePath = argv[arg];
		processFile(filePath, countBuffer, currCharBuffer, prevCharBuffer);
	}
	// print out the last number in the last file 
	write(STDOUT_FILENO, countBuffer, 4 * sizeof(char));
	write(STDOUT_FILENO, prevCharBuffer, sizeof(char));

	free(countBuffer);
	free(currCharBuffer);
	free(prevCharBuffer);
}
