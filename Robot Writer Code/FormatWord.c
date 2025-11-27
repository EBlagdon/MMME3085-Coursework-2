#include <stdio.h>
#include <stdlib.h>
#define MAXCN 20

//This function Seperates the words from the text file and saves them into an array of strings
int ReadWord( FILE *WordFile, char (*WordContents)[], int *startposition)
{
    char currChar;
	fseek(WordFile, *startposition, SEEK_SET); //sets file position to startposition
        int i=0;
        currChar = fgetc(WordFile);
        while (i < MAXCN && currChar != ' ' && currChar != '\n' && currChar != EOF) { //Read the word until space, newline, or EOF
            (*WordContents)[i++] = (char)currChar; //saves letter to word contents 
            currChar = fgetc(WordFile);
        }
        *startposition = ftell(WordFile);        //Update startposition to current file position
        
    return 0;
}

/*
int SeperateWords( FILE *WordFile, char (*WordContents)[], int *startposition)
{
    char currChar;
	fseek(WordFile, *startposition, SEEK_SET); //sets file position to startposition
        int j = *startposition;
        int i=0;
        while (j + 1 < MAXCN && ((currChar = fgetc (WordFile)) != ' ' && currChar != EOF)){  //Reads untill the next space
            (*WordContents)[i++] = (char)currChar; //saves letter to array
            j++;
            }
       *startposition = j + 1; //saves the start position of the next word next time the array is called

    return 0;
}
*/
