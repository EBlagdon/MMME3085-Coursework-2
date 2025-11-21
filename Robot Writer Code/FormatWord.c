#include <stdio.h>
#include <stdlib.h>
#define MAXCN 20

/*This function Seperates the words from the text file and saves them into an array of strings*/
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


