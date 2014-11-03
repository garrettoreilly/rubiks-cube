#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "valid.h"
#include "turns.h"

int main(int argc, char *argv[]) {

    // Check for a passed file
    if (argc != 2) {
        returnFalse();
    } 

    // Read file in
    FILE *input = fopen(argv[1], "r");
    // Make sure the file can be opened
    if (input == 0) {
        returnFalse();
    }

    // Generate valid cube representation
    unsigned char inputCube[20];
    generateCube(input, inputCube);

    return 0;
}
