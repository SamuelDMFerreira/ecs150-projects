#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <istream>

// checks if word is in line
bool isWordInLine(char* lineBuffer, char* word)
{
	int lineIndex = 0;
	bool wordIsInLine = false;
	while(lineBuffer[lineIndex] != '\0')
	{
		// if a character in word and line match, go ahead and check next few character to see if it fully matches
		if (lineBuffer[lineIndex] == word[0])
		{
			int wordIndex = 0;
			bool allCharMatch = true;
			// check next word 
			while(word[wordIndex] != '\0' && word[wordIndex] != '\n')
			{
				// no matching word if line end without full match or the match is full
				if (lineBuffer[lineIndex + wordIndex] == '\0' || word[wordIndex] != lineBuffer[lineIndex + wordIndex])
				{
					allCharMatch = false;
					break;
				}
				wordIndex++;
			}
			wordIsInLine = allCharMatch;
		}
		if (wordIsInLine)
		{	
			break;
		}
		lineIndex++;
	}
	return wordIsInLine;
}

// read one line and check if its has the word
// returns false if end of line is reached, otherwise return true
bool processLine(void* mainBuffer, int fileDescriptor, char* word)
{
	// line buffer stats
	char* lineBuffer = (char*) malloc(sizeof(char));
	int currIndex = 0;
	int lineBufferSize = sizeof(char);

	int byteRead = sizeof(char);
	int wereByteRead = read(fileDescriptor, mainBuffer, byteRead);
	// add line from file to line buffer
	// end line at newLine character or when at end of file
	while(wereByteRead > 0 && (*((char*)mainBuffer)) != '\n')
	{
		// add character read in mainBuffer to lineBuffer
		lineBuffer[currIndex] = (*((char*)mainBuffer));
		currIndex++;
		lineBufferSize += sizeof(char);
		lineBuffer = (char*) realloc(lineBuffer, lineBufferSize);

		wereByteRead = read(fileDescriptor, mainBuffer, byteRead);
	}
	// add end of string character, to be used by isWordInLine
	//currIndex++;
	//lineBufferSize += sizeof(char);
	//lineBuffer = (char*) realloc(lineBuffer, lineBufferSize);
	lineBuffer[currIndex] = '\0';

	if (isWordInLine(lineBuffer, word))
	{
		// don't print out null character
		write(1, ((void*)lineBuffer), lineBufferSize - sizeof(char));	
		write(1, "\n", sizeof(char));
	}

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
		std::cout << "wgrep: cannot open file" << std::endl;
		exit(1);
	}
	void* mainBuffer = malloc(sizeof(char));
	//int byteRead = sizeof(char);

	while(processLine(mainBuffer, fileDescriptor, word)){}

	free(mainBuffer);
	close(fileDescriptor);	
}

void processSTDIN(char* word)
{
        void* mainBuffer = malloc(sizeof(char));

        while(processLine(mainBuffer, STDIN_FILENO, word)){}

        free(mainBuffer);
}

int main(int argc, char* argv[])
{
        // check if no arguments where put in
        if (argc <= 1)
        {
                std::cout << "wgrep: searchterm [file ...]" << std::endl;
                exit(1);
        }
        else if (argc == 2)
        {
                processSTDIN(argv[1]);
        }
        else
        {
                // go through each file
                for (int arg = 2; arg < argc; arg++)
                {
                        const char* filePath = argv[arg];
                        processFile(filePath, argv[1]);
                }
        }

        return 0;
}
