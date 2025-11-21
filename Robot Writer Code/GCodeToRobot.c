#include <stdio.h>
#include <stdlib.h>
#include "rs232.h"
#include "serial.h"
#include "FormatFont.h"
#include "FormatWord.h"
#define X 0
#define Y 1
#define P 2
#define MAXCN 50

int FindWordSpacing(CharacterFontData (*Letters)[], char WordContents[], float (*Spacing)[2], int FontSize) {
    float SpaceBetweenWords = 20.0f * (FontSize) / 18.0f; //calculate space between words based on font size CHECK SPACING IS 20
    int WordLength = sizeof(WordContents) / sizeof(WordContents[0]); //has / character 1 as size of calculates the bytes, not number of rows
    if ((*Spacing)[X] + SpaceBetweenWords + (WordLength * 16.0f * ((FontSize)/18.0f)) > 100.0f) { //check if word length is 16 
        if ((WordLength * 16.0f * (FontSize)/18) > 100.0f) { //if word itself is too long
            printf("Word too long to fit on page.\n");
            return -1; 
        }  
        else { //else move to next line
        //(*Spacing)[Y] = (*Spacing)[Y] + Letters[10].gcode[0][Y]; //move down a line
        (*Spacing)[Y] = (*Spacing)[Y] -5.0f + (*Letters)[10].gcode[0][Y]; //move down a line with 5mm gap CHECK WHICH ONE RIGHT
        (*Spacing)[X] = (*Letters)[13].gcode[0][X]; //reset x position
        }
    }
    //else Y position stays the same and X changes
    else {
        if ((*Spacing)[X] == 0.0f) { //if at start of line
            (*Spacing)[X] = (*Letters)[13].gcode[0][X]; //set to initial x position
        }
        else {
        (*Spacing)[X] = (*Spacing)[X] + SpaceBetweenWords; //set spacing x value
        }
    }
    return 0; 
}

void SendCommands (char *buffer ){
    // printf ("Buffer to send: %s", buffer); // For diagnostic purposes only, normally comment out
    PrintBuffer (&buffer[0]);
    WaitForReply();
    Sleep(100); // Can omit this when using the writing robot but has minimal effect
    // getch(); // Omit this once basic testing with emulator has taken place
}

int GCodeToRobot(CharacterFontData (*Letters)[], int asciiValue, float Spacing[], char buffer[]) {
(*Letters)[asciiValue]; //get character data from struct using ascii value
for (int i=0; i < (*Letters)[asciiValue].location; i++) { //for each line of gcode
    float x = (*Letters)[asciiValue].gcode[i][X]+Spacing[X]; //get x position
    float y = (*Letters)[asciiValue].gcode[i][Y]+Spacing[Y]; //get y position
    int pen = (int)(*Letters)[asciiValue].gcode[i][P]; //get pen position
    pen = pen * 1000; //convert pen position to servo value
    //SendtoRobot(x, y, pen);
    sprintf(buffer, "G1 X%.2f Y%.2f\n", x, y); //format gcode command
    SendCommands(buffer);
    if (i >0) { // for all but first line
    if (pen != (*Letters)[asciiValue].gcode[i-1][P]){ //only send pen command if pen position changes
        sprintf(buffer, "S%d\n", pen); //format pen command
        SendCommands(buffer);
    }
    }
}
}

/*
    EXAMPLE OF CODE TO SEND TO ROBOT
    sprintf (buffer, "G0 X-13.41849 Y0.000\n");
    SendCommands(buffer);
    sprintf (buffer, "S1000\n");
    SendCommands(buffer);

    */