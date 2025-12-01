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
    float SpaceBetweenWords = 12.0f * (FontSize) / 18.0f; //calculate space between words based on font size
    int WordLength = 0;
    for (int i=0; i < sizeof(WordContents); i++) { //for each point in the wordcontents array
        if (WordContents[i] != 0) { //if the space isnt blank 
            WordLength++; //add to word length
        }
    }
    //int WordLength = sizeof(WordContents) / sizeof(WordContents[0]); //has / character 1 as size of calculates the bytes, not number of rows
    if ((*Spacing)[X] + SpaceBetweenWords + (WordLength * 16.0f * ((FontSize)/18.0f)) > 100.0f) { //check if word length plus current x pos plus space between words is greater than 100mm
        if ((WordLength * 16.0f * (FontSize)/18) > 100.0f) { //if word itself is too long by itself
            printf("Word too long to fit on page.\n"); // print error message
            return 1; 
        }  
        else { //else move to next line
        (*Spacing)[Y] = (*Spacing)[Y] + (*Letters)[10].gcode[0][Y] * ((float)(FontSize)/18.0f); //move down a line based on line height of font
        (*Spacing)[X] = (*Letters)[13].gcode[0][X] * ((float)(FontSize)/18.0f); //reset x position
        }
    }
    //else Y position stays the same and X changes
    else {
        if ((*Spacing)[X] == 0.0f) { //if at start of line
            (*Spacing)[X] = (*Letters)[13].gcode[0][X] * ((float)(FontSize)/18.0f); //set to initial x position
        }
        else {
        (*Spacing)[X] = (*Spacing)[X] + SpaceBetweenWords; //set spacing x value from end of last word plus space between words
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

int GCodeToRobot(CharacterFontData (*Letters)[], int asciiValue, float Spacing[], char buffer[], int FontSize) {
    CharacterFontData currentChar = (*Letters)[asciiValue]; //get character data from struct using ascii value
for (int i=0; i < (*Letters)[asciiValue].location; i++) { //for each line of gcode
    float x = (*Letters)[asciiValue].gcode[i][X]* ((float)(FontSize)/18.0f)+Spacing[X]; //get x position
    float y = (*Letters)[asciiValue].gcode[i][Y]* ((float)(FontSize)/18.0f)+Spacing[Y]; //get y position
    int pen = (int)(*Letters)[asciiValue].gcode[i][P]; //get pen position
    pen = pen * 1000; //convert pen position to servo value
    sprintf(buffer, "G1 X%.2f Y%.2f\n", x, y); //format gcode command
    SendCommands(buffer);
    if (i == 0) { // for first line
        sprintf(buffer, "S%d\n", pen); //format pen command
        SendCommands(buffer);
    }
    if (i >0) { // for all but first line
        if (pen/1000 != (*Letters)[asciiValue].gcode[i-1][P]){ //only send pen command if pen position changes
            sprintf(buffer, "S%d\n", pen); //format pen command
            SendCommands(buffer);
    }
    }
}
return 0; 
}

    // These are sample commands to draw out some information - these are the ones you will be generating.
    /*
    sprintf (buffer, "G0 X-13.41849 Y0.000\n");
    SendCommands(buffer);
    sprintf (buffer, "S1000\n");
    SendCommands(buffer);
    sprintf (buffer, "G1 X-13.41849 Y-4.28041\n");
    SendCommands(buffer);
    sprintf (buffer, "G1 X-13.41849 Y0.0000\n");
    SendCommands(buffer);
    sprintf (buffer, "G1 X-13.41089 Y4.28041\n");
    SendCommands(buffer);
    sprintf (buffer, "S0\n");
    SendCommands(buffer);
    sprintf (buffer, "G0 X-7.17524 Y0\n");
    SendCommands(buffer);
    sprintf (buffer, "S1000\n");
    SendCommands(buffer);
    sprintf (buffer, "G0 X0 Y0\n");
    SendCommands(buffer);
    */