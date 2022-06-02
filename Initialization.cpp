#include "Definitions.h"

#define random64BitNumber ((U64)rand() | (U64)rand() << 15 | (U64)rand() << 30 | (U64)rand() << 45 | ((U64)rand() & 0xf) << 60) // A random 64 bit number

int array120ToArray64[boardSquareNumber]; // To convert the 12x10 index to a 8x8 index
int array64ToArray120[64]; // To convert the 8x8 index to a 12x10 index
int indexToFiles[boardSquareNumber]; //Converts the index to a file letter
int indexToRanks[boardSquareNumber]; //Converts the index to a rank number

U64 setBitBoardMask[64]; // The set bitboard mask array
U64 clearBitBoardMask[64]; // The clear bitboard mask array  
U64 pieceHashKeys[13][boardSquareNumber]; //Hashes (stores) the pieces and their positions
U64 sideHashKey; // Hashes which side is it to move
U64 castleHashKey[16]; // Hashes the current castling permissions

void indexToFileAndRank(){ //Converts the 120 array index to a file and rank coordinate
    int file = aFile; //The first file on the board
    int rank = firstRank; //The first rank on the board

    for(int index = 0; index < boardSquareNumber; ++index){ //Sets everything to an offboard square
        indexToFiles[index] = offBoardSquare; //Sets all files to an offboard square
        indexToRanks[index] = offBoardSquare; //Sets all ranks to an offboard square
    }

    for (int rank = firstRank; rank <= eighthRank; ++rank){ // Loops through every rank on the board
        for (int file = aFile; file <= hFile; ++file){ // Loops through every file on the board
            int square120array = coordinatesTo120ArrayIndex(file, rank);
            indexToFiles[square120array] = file; //Sets the file on that index
            indexToRanks[square120array] = rank; //Sets the rank on that index
        }
    }
}

void initializeHashKeys(){
    for(int index = 0; index < 13; ++index){ //Loops for the first index of the piece hash key
        for(int secondIndex = 0; secondIndex < boardSquareNumber; ++ secondIndex){ //Loops for the second index of the piece hash key
            pieceHashKeys[index][secondIndex] = random64BitNumber; // Assigns a random 64 bit number for the piece hash key
        }
    }
    
    sideHashKey = random64BitNumber; //Assigns a 64 bit number to the side hash key

    for(int index = 0; index < 16; ++index){ //Loops for the index of the casle hash key
        castleHashKey[index] = random64BitNumber; // Assigns a random 64 bit number to the castle hash key
    }
}

void initializeBitBoardMask(){ //Initializes the mask for the bitboards
    for(int index = 0; index < 64; ++index){ //Zeroes out all of the values inside the arrays
        setBitBoardMask[index] = 0ULL; //Zeroes out all of the values inside the set bitboard mask array
        clearBitBoardMask[index] = 0ULL; //Zeroes out all of the values inside the clear bitboard mask array
    }

    for(int index = 0; index < 64; ++index){ //I don't know why we need to initialize the boards twice
        setBitBoardMask[index] = (1ULL << index); //Basically makes a 1 on that index
        clearBitBoardMask[index] = ~setBitBoardMask[index]; //Is the inverse of the set bitboard mask array
    }
}

void initializeArray120ToArray64(){ // Converts the index of the 120 array to the 64 array
    int square64Index; // Index of the 64 array to be replaced on the 120 array

    for (int index = 0; index < boardSquareNumber; ++index){ // Changes all of the items in the 120 array to an impossible number in the 64 array (64, since it goes from 0-63)
        array120ToArray64[index] = 64; //This is an impossible umber in the 64 array index
    }

    for (int index = 0; index < 64; ++index){ // Changes all of the items in the 64 array to an impossible number in the 120 array (120, since it goes from 0-119)
        array64ToArray120[index] = 120; //This is an impossible umber in the 120 array index
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
    initializeBitBoardMask();
    initializeHashKeys();
    indexToFileAndRank();
}