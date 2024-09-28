#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <istream>

// read one line and check if its has the word
// returns false if end of line is reached, otherwise return true
bool processLine(void* mainBuffer, int fileDescriptor, char* word)
{
	// line buffer stats
	char* lineBuffer = malloc(sizeof(char));
	int currIndex = 0;
	int lineBufferSize = sizeof(char);

	int byteRead = sizeof(char);
	int wereByteRead = read(fileDescriptor, mainBuffer, byteRead);
	// add line from file to line buffer
	// end line at newLine character or when at end of file
	while(wereByteRead > 0 && (*((char*)mainBuffer)) != '\n')
	{
		// add character read in mainBuffer to lineBuffer
		currIndex++;
		lineBufferSize += sizeof(char);
		lineBuffer = (char*) realloc(lineBuffer, lineBufferSize);
		lineBuffer[currIndex] = (*((char*)mainBuffer));

		wereByteRead = read(fileDescriptor, mainBuffer, byteRead);
	}

	// temp right line to file
	write(1, ((void*)lineBuffer), lineBufferSize);

	free(lineBuffer);

	if (wereByteRead > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


// go through file and find words that match with "word"
void processFile(const char* filePath, char* word)
{
	int fileDescriptor = open(filePath, O_RDONLY);
	if (fileDescriptor < 0)
	{
		std::cerr << "<wgrep: cannot open file" << std::endl;
		exit(1);
	}
	void* mainBuffer = malloc(sizeof(char));
	//int byteRead = sizeof(char);

	while(processLine(mainBuffer, fileDescriptor, word)){}

	free(mainBuffer);
	close(fileDescriptor);	
}

int main(int argc, char* argv[])
{
	// check if no arguments where put in
	if (argc <= 1)
	{
		std::cout << "wgrep: searchterm [file ...]" << std::endl;
		exit(1);
	}
	// go through each file 
	for (int arg = 2; arg < argc; arg++)
	{
		const char* filePath = argv[arg];
		processFile(filePath, argv[1]);
	}
	return 0;
}
