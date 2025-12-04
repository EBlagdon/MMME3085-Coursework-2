#include <stdio.h>
#include <stdlib.h>
#define MAXCN 50  //max lines of gcode per character CHANGE IF NEEDED

struct CharacterFontData{
    float gcode[MAXCN][3];  // Array to hold GCode data for each character holding X, Y, and Pen positions
    int location;
} 
CharacterFontData;


int FormatAndScaleFontData(FILE *SingleStrokeFont, struct CharacterFontData (*Letters)[]) {
    int asciiValue;
    int CharLinesofGCode;
    char line[15]; //sets max line length for reading font file (uses 14 in font file but using 14 makes it blow up)
    while (fgets(line, sizeof(line), SingleStrokeFont)) {    //reads each line of the font file 
        if (sscanf(line, "999 %d %d", &asciiValue, &CharLinesofGCode)) { //checks if the line is 999 which means new character.
            (*Letters)[asciiValue].location = 0;
            for (int j = 0; j < CharLinesofGCode; j++) { //loops for amount of gcode lines for that character
                if (fgets(line, sizeof(line), SingleStrokeFont)) { //checks if there is a line there to read
                    float x, y; //have to store x y and pen as floats because integers do not save correctly. Why i may ask? who knows
                    float pen;
                    sscanf(line, "%f %f %f", &x, &y, &pen); 
                        (*Letters)[asciiValue].gcode[j][0] = x; // Store X    
                        (*Letters)[asciiValue].gcode[j][1] = y; // Store Y
                        (*Letters)[asciiValue].gcode[j][2] = pen;          // Pen position
                        (*Letters)[asciiValue].location++; // Increment location count
                }
                else{
                    printf("end of file while reading character %d\n", asciiValue);
                }
            }
        }
    }   
    return 0;
}







