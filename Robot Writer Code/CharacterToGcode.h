#ifndef CharacterToGcode_H
#define CharacterToGcode_H

#include "FormatFont.h" 
#include "FormatWord.h"  

int GCodeToRobot(CharacterFontData (*Letters)[], int asciiValue, float Spacing[], char buffer[]);

int FindWordSpacing(CharacterFontData (*Letters)[], char WordContents[], float (*Spacing)[], int FontSize);

void SendCommands (char *buffer );

#endif // CharacterToGcode_H