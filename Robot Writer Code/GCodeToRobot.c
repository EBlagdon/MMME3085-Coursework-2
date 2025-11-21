#include <stdio.h>
#include <stdlib.h>
#include "rs232.h"
#include "serial.h"
#include "FormatFont.h"
#include "FormatWord.h"
#define X 0
#define Y 1
#define P 2

int FindWordSpacing(char WordContents[], float *Spacing[], int *FontSize) {
    float SpaceBetweenWords = 20.0f * (*FontSize) / 18.0f; //calculate space between words based on font size CHECK SPACING IS 20
    int WordLength = sizeof(WordContents) / sizeof(WordContents[0]); //has / character 1 as size of calculates the bytes, not number of rows
    if ((*Spacing)[X] + SpaceBetweenWords + (WordLength * 16.0f * ((*FontSize)/18)) > 100.0f) { //check if word length is 16 
        if ((WordLength * 16.0f * (*FontSize)/18) > 100.0f) { //if word itself is too long
            printf("Word too long to fit on page.\n");
            return -1; 
        }  
        else { //else move to next line
        (*Spacing)[Y] = (*Spacing)[Y] - (20.0f * (*FontSize)/18); //move down a line
        (*Spacing)[X] = 0.0f; //reset x position
        }
    //else Y position stays the same and X changes
    (*Spacing)[X] = (*Spacing)[X] + SpaceBetweenWords; //set spacing x value
    }
    return 0;
}

int GCodeToRobot(CharacterFontData Letters[], int asciiValue, float Spacing[], char buffer[]) {
Letters[asciiValue]; //get character data from struct using ascii value
for (int i = 0; i < MAXCN; i++) { //for each line of gcode
    float x = Letters[asciiValue].gcode[i][X]+Spacing[X]; //get x position
    float y = Letters[asciiValue].gcode[i][Y]+Spacing[Y]; //get y position
    int pen = (int)Letters[asciiValue].gcode[i][P]; //get pen position
    //send to robot via serial
    //SendtoRobot(x, y, pen);
    sprintf(buffer, "G1 X%.2f Y%.2f\n", x, y); //format gcode command
    SendCommands(buffer);
    sprintf(buffer, "S%d\n", pen); //format pen command
    SendCommands(buffer);
    return 0;
}
}

/*
    EXAMPLE OF CODE TO SEND TO ROBOT
    sprintf (buffer, "G0 X-13.41849 Y0.000\n");
    SendCommands(buffer);
    sprintf (buffer, "S1000\n");
    SendCommands(buffer);

    */