#include "Definitions.h"

int popBit(U64 *bitBoardAddress){ //This basically pops out (removes) 1 bit from the board
    int leastSignificantBit = __builtin_ctzll(*bitBoardAddress); //gets the least significant bit on the bitboard
    *bitBoardAddress &= (*bitBoardAddress - 1); //Pops the least significant bit on the bitboard
    return leastSignificantBit; //Returns  the least significant bit on the bitboard
}

int countBit(U64 bitBoard){ //Takes a bitboard and counts the number of bits inside it
    return __builtin_popcountll(bitBoard); //Returns the number of set bits
}

void printBitBoard(U64 bitBoard){ //This is just to visualize the bitboards

    U64 shiftedBit = 1ULL; // The bit that is going to be shifted

    for (int rank = eighthRank; rank >= firstRank; --rank){ // Loops through every rank on the board in reverse
        for (int file = aFile; file <= hFile; ++file){ // Loops through every file on the board in reverse
            int square120Index = coordinatesTo120ArrayIndex(file, rank); //The index for the 120 array
            int square64Index = array120ToArray64[square120Index]; //The index for the 64 array

            if((shiftedBit << square64Index) & bitBoard){cout << " p ";} //Prints an x if there is a pawn
            else{cout << " . ";} //Prints a - if there is no pawn
        }
        cout << endl;
    }
}