#ifndef CharacterToGcode_H
#define CharacterToGcode_H

#include "FormatFont.h" 
#include "FormatWord.h"  

int GCodeToRobot(struct CharacterFontData (*Letters)[], int asciiValue, float Spacing[], char buffer[], int FontSize);

int FindWordSpacing(struct CharacterFontData (*Letters)[], char WordContents[], float (*Spacing)[], int FontSize);

void SendCommands (char *buffer );

#endif // CharacterToGcode_H