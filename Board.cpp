#include "Definitions.h"

void resetBoard(Board *position){ //A function to reset basically everything the board
    for(int index = 0; index < boardSquareNumber; ++index){ //Loops through all the squares on the 120 array board
        position->boardPieces[index] = offBoardSquare; //Sets all these squares to an off-board square, which is basically the same as an empty square i don't know why we're differentiating
    }

    for(int index = 0; index < 64; ++index){ //Loops through all the squares on the 64 array board
        position->boardPieces[array64ToArray120[index]]; //Sets all these squares to an empty square
    }

    for(int index = 0; index < 3; ++index){ //Loops through all the squares on the board for the classifications of the pieces
        position->bigPieceNumber[index] = 0; //Resets the number of big pieces on the board
        position->minorPiecesNumber[index] = 0; //Resets the number of minor pieces on the board
        position->majorPiecesNumber[index] = 0; //Resets the number of major pieces on the board
        position->pawns[index] = 0ULL; //Resets the number of pawns on the board
    }

    for(int index = 0; index < 13; ++index){ //Loops through all the squares on the board for the number of the types of pieces
        position->pieceNumber[index] = 0; //Resets the number of pieces on the board
    }

    position->kingSquare[white] = position->kingSquare[black] = emptySquare; //Sets the king squares to empty squares
    position->side = both; //Sets the side to play to both (neither white nor black)
    position->enPassant = emptySquare; //Sets ne en passant square to an empty square
    position->fiftyMoveRule = 0; //Resets the fifty move counter
    position->castlePermission = 0; //Resets castling permissions
    position->ply = 0; //Resets the half-move count
    position->hisPly = 0; //Resets the half-move history
    position->positionKey = 0ULL; //Resets the current position key
}