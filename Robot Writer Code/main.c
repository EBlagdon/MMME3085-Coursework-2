#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
//#include <windows.h>
#include "rs232.h"
#include "serial.h"
#include "FormatFont.h"
#include "FormatWord.h"
#include "CharacterToGcode.h"


#define bdrate 115200               /* 115200 baud */
#define X 0
#define Y 1

#define debug 1 //1 for debug mode, 0 for normal operation

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
    if (!SingleStrokeFont) { //CHECK HAS OPENED
        printf("Failed to open SingleStrokeFont.txt");
        return 1;
    }

    //GET USER INPUT FONT SIZE
    int l=0; // variable to check valid font size
    int FontSize;
    while (l==0) {
        printf("Enter font size (4-10): ");
        scanf("%d", &FontSize);
        if (FontSize > 3 && FontSize < 11) { //CHECK FONT SIZE IS BETWEEN 4 AND 10
            l=1; // if valid font size, exit the loop
        }
        else {
            printf("Font size must be between 4 and 10.\n");
        }

    }
   
    //CALL FORMAT AND SCALE FONT DATA FUNCTION
    struct CharacterFontData Letters[128] = {0}; //array to hold font data for 128 ASCII characters (can be expanded as needed)
 
    FormatAndScaleFontData(SingleStrokeFont, &Letters); // formats and scales all the font data to be stored locally
    fclose(SingleStrokeFont); //close font file

    FILE *WordFile = fopen("Test.txt", "r"); //OPEN Test.TXT
    if (!WordFile) { //CHECK HAS OPENED
        printf("Failed to open Test.txt");
        return 1;
    }

    //sets up variables for word formatting and drawing
    int currChar;
    int numwords = 0;
    char WordContents[25]; //array to hold words, maximum word length of 25 characters (anything longer and will be longer than 100mm on minimum font size)
    int startposition = 0; // sets start position to 0 to read the first word

    //find number of words 
    while ((currChar = fgetc(WordFile)) != EOF) { //whilst there are characters in the file
        if (currChar == ' ' || currChar == '\n') { // if the character is a space or new line
            numwords++; //add to word count
        }
    }
    numwords++; //add one to account for last word (not followed by space)
    
    //Seperate and Print Words
    float Spacing[2]; //array to hold spacing values in x and y direction
    Spacing[X] = Letters[13].gcode[0][X]* ((float)(FontSize)/18.0f); //initial x position
    Spacing[Y] = Letters[13].gcode[0][Y]* ((float)(FontSize)/18.0f); //initial y position
    Spacing[Y] = Spacing[Y] - (20.0f *FontSize/18.0f); //add margin at top of page
    for (int i = 0; i < numwords; i++) { //for the number of words in the file
        ReadWord(WordFile, &WordContents, &startposition); //Find the characters in the current word
        FindWordSpacing(&Letters, WordContents, &Spacing, FontSize); //Find the spacing for the start of the current word
        for (int j = 0; WordContents[j] != '\0'; j++) { //for the number of characters in the word
            char currChar = WordContents[j]; //GET CURRENT CHARACTER
            int asciiValue = (int)currChar; //GET ASCII VALUE OF CHARACTER    
            GCodeToRobot(&Letters, asciiValue, Spacing, buffer, FontSize); //Print the GCode for the current character
            Spacing[X] = Spacing[X] + 18*(FontSize/18.0f); //update spacing x position for next letter
        }
        for (int k = 0; k < 25; k++) {   //resets wordcontents for next word
            WordContents[k] = 0; 
        }
    }
    fclose(WordFile); //close text file

    //Set final conditions for robot
    sprintf (buffer, "G1 X0 Y0 F1000\n");
    SendCommands(buffer);
    sprintf (buffer, "M3\n");
    SendCommands(buffer);
    sprintf (buffer, "S0\n");
    SendCommands(buffer);

    // Before we exit the program we need to close the COM port
    CloseRS232Port();
    printf("Com port now closed\n");

    return (0);
}


// Send the data to the robot - note in 'PC' mode you need to hit space twice
// as the dummy 'WaitForReply' has a getch() within the function.
/*
void SendCommands (char *buffer ){
    // printf ("Buffer to send: %s", buffer); // For diagnostic purposes only, normally comment out
    PrintBuffer (&buffer[0]);
    WaitForReply();
    Sleep(100); // Can omit this when using the writing robot but has minimal effect
    // getch(); // Omit this once basic testing with emulator has taken place
}
*/