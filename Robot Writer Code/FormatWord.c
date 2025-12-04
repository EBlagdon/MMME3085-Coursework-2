#include <stdio.h>
#include <stdlib.h>
#define MAXCN 20

//This function Seperates the words from the text file and saves them into an array of strings
int ReadWord( FILE *WordFile, char (*WordContents)[], int *startposition)
{
    char currChar;
	fseek(WordFile, *startposition, SEEK_SET); //sets file position to startposition
        int i=0;
        currChar = fgetc(WordFile); //get first character
        while (i < MAXCN && currChar != ' ' && currChar != '\n' && currChar != EOF) { //Read the word until space, newline, or EOF
            (*WordContents)[i++] = (char)currChar; //saves letter to word contents 
            currChar = fgetc(WordFile); //get next character
        }
        *startposition = ftell(WordFile);        //Update startposition to current file position
        
    return 0;
}

