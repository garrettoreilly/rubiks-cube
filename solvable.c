#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "solvable.h"

// Bit values of colors
unsigned char none = 0;
unsigned char red = 1;
unsigned char green = 2;
unsigned char yellow = 3;
unsigned char blue = 4;
unsigned char orange = 5;
unsigned char white = 6;

// Used to determine value of Z-axis
unsigned char adjacency[6][4] = {{2,3,4,6},{1,3,5,6},{1,2,4,5},{1,3,5,6},{2,3,4,6},{1,2,4,5}};

// Solved cube
unsigned char goalState[20] = {70,7,134,68,132,69,5,133,67,131,65,129,86,23,150,84,148,85,21,149};

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
    // Save the orientation of the cube from the file to a string
    int t;
    int tilesCount = 0;
    char tiles[55];
    while ((t = fgetc(input)) != EOF && tilesCount < 54) {
        if (t != '\n' && t != ' ') {
            tiles[tilesCount] = t;
            tilesCount++;
        }
    }
    tiles[54] = '\0';
    fclose(input);

    // False if there aren't 54 tiles
    if (tilesCount != 54) {
        returnFalse();
    }
    // False if there aren't 9 tiles of each color
    int colors[6] = {0, 0, 0, 0, 0, 0};
    for (int i = 0; i < tilesCount; i++) {
        if (tiles[i] == 'R') {
            colors[0] += 1;
            if (colors[0] > 9) {
                returnFalse();
            }
        } else if (tiles[i] == 'Y') {
            colors[1] += 1;
            if (colors[1] > 9) {
                returnFalse();
            }
        } else if (tiles[i] == 'B') {
            colors[2] += 1;
            if (colors[2] > 9) {
                returnFalse();
            }
        } else if (tiles[i] == 'O') {
            colors[3] += 1;
            if (colors[3] > 9) {
                returnFalse();
            }
        } else if (tiles[i] == 'W') {
            colors[4] += 1;
            if (colors[4] > 9) {
                returnFalse();
            }
        } else if (tiles[i] == 'G') {
            colors[5] += 1;
            if (colors[5] > 9) {
                returnFalse();
            }
        }
    }

    int cubieTileMatches[20][3] = {
        // 1
        {9, 0, 52},
        // 2
        {-1, 1, 53},
        // 3
        {17, 2, 54},
        // 4
        {10, 3, -1},
        // 5
        {16, 5, -1},
        // 6
        {11, 6, 12},
        // 7
        {-1, 7, 13},
        // 8
        {15, 8, 14},
        // 9
        {18, -1, 48},
        // 10
        {26, -1, 50},
        // 11
        {20, -1, 21},
        // 12
        {24, -1, 23},
        // 13
        {27, 42, 45},
        // 14
        {-1, 43, 46},
        // 15
        {35, 44, 47},
        // 16
        {28, 39, -1},
        // 17 
        {34, 41, -1},
        // 18
        {29, 36, 30},
        // 19
        {-1, 37, 31},
        // 20
        {33, 38, 32}
    };

    unsigned char inputCube[20];
    for (int i = 0; i < sizeof(cubieTileMatches)/sizeof(cubieTileMatches[0]); i++) {
        inputCube[i] = generateCube(tiles, cubieTileMatches[i][0], cubieTileMatches[i][1], cubieTileMatches[i][2]);
    }

    printf("Size of goal state: %lu\n", sizeof(goalState));
    printf("Size of input cube: %lu\n", sizeof(inputCube));
}

// Function for exiting upon discovery of invalid cube
void returnFalse() {
    printf("false\n");
    exit(0);
}

// Function for exiting upon proof of valid cube
void returnTrue() {
    printf("true\n");
    exit(0);
}

// Function to determine color of tile
unsigned char tileColor(char t) {
    if (t == 'R') {
        return red;
    } else if (t == 'G') {
        return green;
    } else if (t == 'Y') {
        return yellow;
    } else if (t == 'B') {
        return blue;
    } else if (t == 'O') {
        return orange;
    } else if (t == 'W') {
        return white;
    } else {
        return none;
    }
}

unsigned char z2TileColor(unsigned char x, unsigned char y, unsigned char z) {
    unsigned char possible[2];
    int possibleUsed = 0;
    int i = 0;
    int j = 0;
    int length = sizeof(adjacency[0]) / sizeof(adjacency[0][0]);
    while (i < length && j < length) {
        if (adjacency[x-1][i] == adjacency[y-1][j]) {
            possible[possibleUsed] = adjacency[x-1][i];
            possibleUsed++;
            i++;
            j++;
        } else if (adjacency[x-1][i] < adjacency[y-1][j]) {
            i++;
        } else {
            j++;
        }
    }
    if (z == possible[0]) {
        return 1;
    } else {
        return 2;
    }
}

unsigned char z4TileColor(unsigned char other, unsigned char z) {
    for (unsigned char i = 0; i < sizeof(adjacency[0])/sizeof(adjacency[0][0]); i++) {
        if (adjacency[other-1][i] == z) {
            return i;
        }
    }
    return 4;
}

unsigned char generateCube(char tiles[], int x, int y, int z) {

    unsigned char x_color = 0;
    unsigned char y_color = 0;
    unsigned char z_color = 0;

    if (x == -1) {
        x_color = 0;
        y_color = tileColor(tiles[y]);
        z_color = tileColor(tiles[z]);
        z_color = z4TileColor(y_color, z_color);
    } else if (y == -1) {
        x_color = tileColor(tiles[x]);
        y_color = 0;
        z_color = tileColor(tiles[z]);
        z_color = z4TileColor(x_color, z_color);
    } else if (z == -1) {
        x_color = tileColor(tiles[x]);
        y_color = tileColor(tiles[y]);
        z_color = 0;
    } else {
        x_color = tileColor(tiles[x]);
        y_color = tileColor(tiles[y]);
        z_color = tileColor(tiles[z]);
        z_color = z2TileColor(x_color, y_color, z_color);
    }
    return (x_color << 5) + (y_color << 2) + z_color;
}
