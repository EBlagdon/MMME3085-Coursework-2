#ifndef GCODETOROBOT_H
#define GCODETOROBOT_H

#include "FormatFont.h" 
#include "FormatWord.h"  

int GCodeToRobot(CharacterFontData (*Letters)[], int asciiValue, float Spacing[], char buffer[]);

int FindWordSpacing(CharacterFontData (*Letters)[], char WordContents[], float (*Spacing)[], int FontSize);

void SendCommands (char *buffer );

//int SendtoRobot(float x, float y, int pen);
#endif // GCODETOROBOT_H