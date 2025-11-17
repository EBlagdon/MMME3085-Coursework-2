#include <stdio.h>
#include <stdlib.h>
#define MAXCN 500

/*This function Formats the font data and gets the positions of each character in the GCode array. 
MAY WANT TO SET EACH CHAR IN OWN STRUCT BY CHANGING LINE 24*/

//May want to scale the data in the send function? to allow for arrays being ints instead of floats 

int FormatAndScaleFontData( const int NumFont, FILE *SingleStrokeFont, int FontSize, int *CharGCode[], int *CharLocation) {
    int k=1;
    int i;
    int j=0;
    int PositionDataP;
    char PositionDataX[MAXCN];
    char PositionDataY[MAXCN];
    char currChar;
    float CharGCode[3][NumFont]; //Array to hold the gcode data for each character
    
    for (i=1; i<=NumFont; i++) {
        while (j + 1 < MAXCN && ((currChar = fgetc (SingleStrokeFont)) != ' ' && currChar != EOF)){  //Reads untill the first space and saves it to PositionDataX
            PositionDataX[j++] = currChar;
            }
        if (PositionDataX[1] == 9) { //checks if the line is a new character location
            CharLocation[k] = i;          //saves the location of the character if the data is for a new character
            k=k+1;
        } 
        else {
            int X = sizeof(PositionDataX) / sizeof(PositionDataX[0]); //combines the data from PositionData into a single integer
            CharGCode[1][i] = ConcatenateArr(PositionDataX, X)*FontSize/18; //saves the x gcode data for the character in array
        
    
        while (j + 1 < MAXCN && ((currChar = fgetc (SingleStrokeFont)) != ' ' && currChar != EOF)){  //Reads untill the second space and saves it to PositionDataY
            PositionDataY[j++] = currChar;}
        int Y = sizeof(PositionDataY) / sizeof(PositionDataY[0]); //combines the data from PositionData into a single integer
        CharGCode[2][i] = ConcatenateArr(PositionDataY, Y)*FontSize/18; //saves the y gcode data for the character in array

        while (j + 1 < MAXCN && ((currChar = fgetc (SingleStrokeFont)) != ' ' && currChar != EOF)){  //Reads untill the third space and saves it to PositionDataP
            PositionDataP = currChar;}
        CharGCode[3][i] = PositionDataP;
        } //saves the pen gcode data for the character in array
        j = 0; //Resets j for next line
        }
    }