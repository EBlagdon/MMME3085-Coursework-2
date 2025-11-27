#include <stdio.h>
#include <stdlib.h>
#define MAXCN 50  //max lines of gcode per character CHANGE IF NEEDED

typedef struct {
    float gcode[MAXCN][3];  // Array to hold GCode data for each character holding X, Y, and Pen positions
    int location;           // Position of the GCode in the GCode array
} CharacterFontData;

int FormatAndScaleFontData(FILE *SingleStrokeFont, int FontSize, CharacterFontData (*Letters)[]) {
    int asciiValue;
    int CharLinesofGCode;
    char line[128]; 
    while (fgets(line, sizeof(line), SingleStrokeFont)) {    //reads each line of the font file 
        if (sscanf(line, "999 %d %d", &asciiValue, &CharLinesofGCode) == 2) { //checks if the line is 999 which means new character
            (*Letters)[asciiValue].location = 0;
            for (int j = 0; j < CharLinesofGCode; j++) { //loops for amount of gcode lines for that character
                if (fgets(line, sizeof(line), SingleStrokeFont)) { //checks if there is a line there to read
                    float x, y;
                    int pen;
                    if (sscanf(line, "%f %f %d", &x, &y, &pen) == 3) {
                        (*Letters)[asciiValue].gcode[j][0] = x * FontSize / 18.0f; // Scale X
                        (*Letters)[asciiValue].gcode[j][1] = y * FontSize / 18.0f; // Scale Y
                        (*Letters)[asciiValue].gcode[j][2] = (float)pen;          // Pen position
                        (*Letters)[asciiValue].location++; // Increment location count
                    }
                    else {
                        printf("Could not read GCode on line %d\n", j);
                    }
                }
                else{
                    printf("end of file while reading character %d\n", asciiValue);
                }
            }
        }
    }   
    return 0;
}


















/*This function Formats the font data and gets the positions of each character in the GCode array. 
MAY WANT TO SET EACH CHAR IN OWN STRUCT BY CHANGING LINE 24*/

//May want to scale the data in the send function? to allow for arrays being ints instead of floats 
/*
int FormatAndScaleFontData( const int NumFont, FILE *SingleStrokeFont, int FontSize, int *CharGCode[], int *CharLocation) {
    int k=1;
    int i;
    int j=0;
    char currChar;
    int PositionDataX[MAXCN];
    int PositionDataY[MAXCN];   
    int PositionDataP;
    
    for (i=1; i<=NumFont; i++) {
        while (j + 1 < MAXCN && ((currChar = fgetc (SingleStrokeFont)) != ' ' && currChar != EOF)){  //Reads untill the first space and saves it to PositionDataX
            currChar = fgetc(SingleStrokeFont);
            }
        if (currChar = 999) { //checks if the line is a new character location
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

    */