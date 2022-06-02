#include "Definitions.h"

U64 generatePositionKeys(const Board *position){ //Generates a unique key for each position
    U64 finalKey = 0; //The key itself starts as 0
    int piece = emptyPiece; //Sets the default piece value

    for(int square120Index = 0; square120Index < boardSquareNumber; ++square120Index){ //Loops through all the squares on the 120 array board
        piece = position->boardPieces[square120Index]; //The variable piece is equal to whatever the value the current square is
        if(piece != emptySquare && piece != emptyPiece && piece != offBoardSquare){ //Checks wether the square is a border square or an empty square or an off-board square
            ASSERT(piece >= whitePawn && piece <= blackKing); //Makes sure that the piece is in the range of a white pawn to a black king as defined
            finalKey ^= pieceHashKeys[piece][square120Index]; //Hashes that square with a piece into the final position key (a nice use of the XOR operator!)
        }
    }

    if(position->side == white){finalKey ^= sideHashKey;} //Checks which side is to play

    if(position->enPassant != emptySquare){ //Checks if the en passant square is empty or not
        ASSERT(position->enPassant >= 0 && position->enPassant < boardSquareNumber); //Makes sure that the square is actually on the board
        finalKey ^= pieceHashKeys[emptyPiece][position->enPassant]; //Hashes the en passant square to the final position key
    }

    ASSERT(position->castlePermission >= 0 && position->castlePermission <= 15); //Makes sure that the castling permissions is in the defined range
    finalKey ^= castleHashKey[position->castlePermission]; //Hashes the castling permissions to the final position key

    return finalKey; //Returns the final position key
}