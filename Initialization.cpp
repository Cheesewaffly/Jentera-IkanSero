#include "Initialization.h"

int array120ToArray64[boardSquareNumber]; // To convert the 12x10 index to a 8x8 index
int array64ToArray120[64]; // To convert the 8x8 index to a 12x10 index

void initializeArray120ToArray64(){ // Converts the index of the 120 array to the 64 array
    int square64Index = 0; // Index of the 64 array to be replaced on the 120 array

    for (int index = 0; index < boardSquareNumber; ++index){ // Changes all of the items in the 120 array to an impossible number in the 64 array (64, since it goes from 0-63)
        array120ToArray64[index] = 64;
    }

    for (int index = 0; index < 64; ++index){ // Changes all of the items in the 64 array to an impossible number in the 120 array (120, since it goes from 0-119)
        array64ToArray120[index] = 120;
    }

    for (int rank = firstRank; rank <= eighthRank; ++rank){ // Loops through every rank on the board
        for (int file = aFile; file <= hFile; ++file){ // Loops through every file on the board
            int square120Index = coordinatesTo120ArrayIndex(file, rank); // Gets the index of the square in the 120 array
            array64ToArray120[square64Index] = square120Index; // Converts the 64 array index to the 120 array index
            array120ToArray64[square120Index] = square64Index; // Converts the 120 array index to the 64 array index
            square64Index++; // Adds 1 to the 64 array index
        }
    }
}

void initializeAll(){
    initializeArray120ToArray64();
}