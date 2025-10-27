#include <stdio.h>
#include <stdlib.h>

// Main () - execution starts here
int main (void)
{
    // Declare variables
    int SampleData[10];


    for ( int i = 0 ; i < 1000 ; i++ )
    {
        SampleData[i] = i;   // Populate the array
        printf ("Values in a is %d \n", SampleData[i]);
    }
    return (0);     // Exit indicating success
}