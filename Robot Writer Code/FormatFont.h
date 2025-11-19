#ifndef CHARACTER_FONT_DATA_H
#define CHARACTER_FONT_DATA_H
#include <stdio.h>
#define MAXCN 50

typedef struct {
float gcode[MAXCN][3]; // X, Y, Pen for each GCode line 
int location; // Position of the character in the GCode array 
} CharacterFontData;

int FormatAndScaleFontData(FILE *SingleStrokeFont, int FontSize, CharacterFontData Letters[]);

#endif // CHARACTER_FONT_DATA_H