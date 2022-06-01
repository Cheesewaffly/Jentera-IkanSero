#include "Initialization.h"

const int bitTable[64] = {              //I don't even know what the f**k is going on here, got it from the chess programming wiki here: https://www.chessprogramming.org/BitScan
   63, 30,  3, 32, 25, 41, 22, 33,       //Nevermind, someone figured it out https://stackoverflow.com/questions/30680559/how-to-find-magic-bitboards
   15, 50, 42, 13, 11, 53, 19, 34,      //it also touches the magic numbers (don't ask what it is)
   61, 29,  2, 51, 21, 43, 45, 10,
   18, 47,  1, 54,  9, 57,  0, 35, 
   62, 31, 40,  4, 49,  5, 52, 26, 
   60,  6, 23, 44, 46, 27, 56, 16, 
    7, 39, 48, 24, 59, 14, 12, 55, 
   38, 28, 58, 20, 37, 17, 36,  8
};

int popBit(Unsigned64 *bitBoardAddress){ //This basically pops out (removes) 1 bit from the board (Don't ask how)
    Unsigned64 bitBoard = *bitBoardAddress ^ (*bitBoardAddress - 1); //What
    unsigned int fold = (unsigned) ((bitBoard & 0xffffffff) ^ (bitBoard >> 32)); //In the world
    *bitBoardAddress &= (*bitBoardAddress - 1); //Is going on here
    return bitTable[(fold * 0x783a9b23) >> 26]; //That 0x783a9b23 is a magic number, people smarter than us figured that out apparently
}

int countBit(Unsigned64 bitBoard){ //Takes a bitboard and counts the number of bits inside it
    int bitCounter; //A variable to count the bits
    for(bitCounter = 0; bitBoard; bitCounter++, bitBoard &= bitBoard - 1); //Loops through the bitboard then counts how many loops it has done until it is empty 
    return bitCounter; //How many loops it has done
}

void printBitBoard(Unsigned64 bitBoard){ //This is just to visualize the bitboards

    Unsigned64 shiftedBit = 1ULL; // The bit that is going to be shifted

    for (int rank = eighthRank; rank >= firstRank; --rank){ // Loops through every rank on the board in reverse
        for (int file = aFile; file <= hFile; ++file){ // Loops through every file on the board in reverse
            int square120Index = coordinatesTo120ArrayIndex(file, rank); //The index for the 120 array
            int square64Index = array120ToArray64[square120Index]; //The index for the 64 array

            if((shiftedBit << square64Index) & bitBoard){ //Prints an x if there is a pawn
                cout << "x";
            }
            else{ //Prints a - if there is no pawn
                cout << "-";
            }
        }
        cout << endl;
    }
}