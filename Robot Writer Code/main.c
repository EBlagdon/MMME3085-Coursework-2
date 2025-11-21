#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
//#include <windows.h>
#include "rs232.h"
#include "serial.h"
#include "FormatFont.h"
#include "FormatWord.h"
#include "GCodeToRobot.h"


#define bdrate 115200               /* 115200 baud */

void SendCommands (char *buffer );

int main()
{

    //char mode[]= {'8','N','1',0};
    char buffer[100];

    // If we cannot open the port then give up immediately
    if ( CanRS232PortBeOpened() == -1 )
    {
        printf ("\nUnable to open the COM port (specified in serial.h) ");
        exit (0);
    }

    // Time to wake up the robot
    printf ("\nAbout to wake up the robot\n");

    // We do this by sending a new-line
    sprintf (buffer, "\n");
     // printf ("Buffer to send: %s", buffer); // For diagnostic purposes only, normally comment out
    PrintBuffer (&buffer[0]);
    Sleep(100);

    // This is a special case - we wait  until we see a dollar ($)
    WaitForDollar();

    printf ("\nThe robot is now ready to draw\n");

        //These commands get the robot into 'ready to draw mode' and need to be sent before any writing commands
    sprintf (buffer, "G1 X0 Y0 F1000\n");
    SendCommands(buffer);
    sprintf (buffer, "M3\n");
    SendCommands(buffer);
    sprintf (buffer, "S0\n");
    SendCommands(buffer);

    // OPEN SINGLE STROKE FONT .TXT
    FILE *SingleStrokeFont = fopen("SingleStrokeFont.txt", "r");
    //CHECK HAS OPENED
    if (!SingleStrokeFont) {
        printf("Failed to open SingleStrokeFont.txt");
        return 1;
    }
    //GET USER INPUT FONT SIZE
    int FontSize;
    printf("Enter font size (4-10): ");
    scanf("%d", &FontSize);
    //CHECK FONT SIZE IS BETWEEN 4 AND 10
    if (FontSize < 4 || FontSize > 10) {
        printf("Font size must be between 4 and 10.\n");
        return 1;
    }
   
    //CALL FORMAT AND SCALE FONT DATA FUNCTION
    CharacterFontData Letters[256];  //do i need to declare this again?
 
    FormatAndScaleFontData(SingleStrokeFont, FontSize, Letters);
    //OPEN Test.TXT
    FILE *WordFile = fopen("Test.txt", "r");
    //CHECK HAS OPENED
    if (!WordFile) {
        printf("Failed to open Test.txt");
        return 1;
    }

    //CALL SEPERATE WORDS FUNCTION
    int character;
    int numwords = 0;
    char *WordContents[100]; //array to hold words
    int startposition = 0;
    SeperateWords(WordFile, WordContents, startposition);

    //find number of words 
    while ((character = fgetc(WordFile)) != EOF) {
        if (character == ' ' || character == '\n') {
            numwords++;
        }
    }
    
    //FOR AMOUNT OF WORDS
    float Xpos = 0.0f; //Initial X position
    float Ypos = 0.0f; //Initial Y position
    char CurrentChar;
    float Spacing[2]; //array to hold spacing values in x and y direction
    for (int i = 0; i < numwords; i++) {
        SeperateWords(WordFile, WordContents, startposition); //FORMAT WORD FUNCTION
        FindWordSpacing(*WordContents, Spacing, FontSize); //WORD SPACING FUNCTION
        //FOR AMOUNT OF CHARACTERS IN WORD
        for (int j = 0; WordContents[j] != '\0'; j++) { //for the number of characters in the word
            char currentChar = *WordContents[j]; //GET CURRENT CHARACTER
            int asciiValue = (int)currentChar; //GET ASCII VALUE OF CHARACTER
            GCodeToRobot(Letters, asciiValue, Spacing, buffer); //GCODETOROBOT FUNCTION
        }
        for (int k = 0; k < MAXCN; k++) {   //resets wordcontents for next word
            WordContents[k] = 0; 
        }
    }
            


    //Set final conditions for robot
    





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

    // Before we exit the program we need to close the COM port
    CloseRS232Port();
    printf("Com port now closed\n");

    return (0);
}

// Send the data to the robot - note in 'PC' mode you need to hit space twice
// as the dummy 'WaitForReply' has a getch() within the function.
void SendCommands (char *buffer ){
    // printf ("Buffer to send: %s", buffer); // For diagnostic purposes only, normally comment out
    PrintBuffer (&buffer[0]);
    WaitForReply();
    Sleep(100); // Can omit this when using the writing robot but has minimal effect
    // getch(); // Omit this once basic testing with emulator has taken place
}
