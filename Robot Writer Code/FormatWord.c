#include <stdio.h>
#include <stdlib.h>
#define MAXCN 20

/*This function Seperates the words from the text file and saves them into an array of strings*/
int SeperateWords( FILE *SingleStrokeFont, char WordContents[], int *startposition)
{
    char currChar;
	SingleStrokeFont = fopen("SingleStrokeFont.txt", "r");
	if (!SingleStrokeFont) {
		perror("Failed to open SingleStrokeFont.txt");
		return 1;
	}
    else{
        int j = *startposition;
        while (j + 1 < MAXCN && ((currChar = fgetc (SingleStrokeFont)) != ' ' && currChar != EOF)){  //Reads untill the next space
            WordContents[j++] = (char)currChar; //saves letter to array
            }
        *startposition = j; //saves the start position of the next word next time the array is called
    }

	fclose(SingleStrokeFont);
	return 0;
}


