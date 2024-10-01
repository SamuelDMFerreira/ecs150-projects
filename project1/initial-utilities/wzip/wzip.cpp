#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <istream>
#include <stdint.h>

void processFile(const char* filePath, void* fourByteCountBuffer, void* byteCharacterBuffer, void* previousCharBuff)
{
	int fileDescriptor = open(filePath, O_RDONLY);
	if (fileDescriptor < 0)
	{
		std::cerr << "cannot open file" << std::endl;
		exit(1);
	}

	
	// go through file and check for repeating values
	int bytesRead = read(fileDescriptor, byteCharacterBuffer, sizeof(char));
	while (bytesRead > 0)
	{
		*((char*)previousCharBuff) = *((char*)byteCharacterBuffer);
		bytesRead = read(fileDescriptor, byteCharacterBuffer, sizeof(char));
		if (bytesRead > 0 && *((char*)previousCharBuff) == *((char*)byteCharacterBuffer))
		{
			*((uint32_t*)fourByteCountBuffer) += 1;
		}
		else if (bytesRead > 0)
		{
			// if characters stop matching print out the information on previous character sequence
			write(STDOUT_FILENO, fourByteCountBuffer, 4 * sizeof(char));
			write(STDOUT_FILENO, previousCharBuff, sizeof(char));
			*((uint32_t*)fourByteCountBuffer) = 1;
		}
	}	


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
	
	void* fourByteCountBuffer = malloc(4 * sizeof(char));
	void* byteCharacterBuffer = malloc(sizeof(char));
	void* previousCharBuff = malloc(sizeof(char)); 
	*((uint32_t*)fourByteCountBuffer) = 1;

	// go through each file and process
	for (int arg = 1; arg < argc; arg++)
	{
		const char* filePath = argv[arg];
		processFile(filePath, fourByteCountBuffer, byteCharacterBuffer, previousCharBuff);
	}
	// print out the last number in the last file 
	write(STDOUT_FILENO, fourByteCountBuffer, 4 * sizeof(char));
	write(STDOUT_FILENO, previousCharBuff, sizeof(char));
	
	free(fourByteCountBuffer);
	free(byteCharacterBuffer);
	free(previousCharBuff);
}
