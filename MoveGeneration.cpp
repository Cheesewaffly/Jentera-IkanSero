#include "Definitions.h"

#define move(origin, destination, capture, promotion, castleOrEnPassantOrDoubleMove) (origin | (destination << 7) | (capture << 14) | (promotion << 20) | castleOrEnPassantOrDoubleMove) //A move

void addNonCaptureMove(const Board *position, int move, MoveList *moveList){ //Adds a non capture move to the movelist
    moveList->moves[moveList->count].move = move; //The movelist structure contains an array of the move structure (hence the . operator)
    moveList->moves[moveList->count].postitionScore = 0;
    moveList->count++; //Adds that move to the counter
}

void addCaptureMove(const Board *position, int move, MoveList *moveList){ //Adds a capture move to the movelist
    moveList->moves[moveList->count].move = move; //The movelist structure contains an array of the move structure (hence the . operator)
    moveList->moves[moveList->count].postitionScore = 0;
    moveList->count++; //Adds that move to the counter
}

void addEnPassantMove(const Board *position, int move, MoveList *moveList){ //Adds an en passant capture move to the movelist
    moveList->moves[moveList->count].move = move; //The movelist structure contains an array of the move structure (hence the . operator)
    moveList->moves[moveList->count].postitionScore = 0;
    moveList->count++; //Adds that move to the counter
}

void addWhitePawnCaptureMove(const Board *position, const int originalSquare, const int destinationSquare, const int capturedPiece, MoveList *moveList){ //Checks if a white pawn is promoting
    if(indexToFiles[originalSquare] == seventhRank){ //If the pawn if from the seventh rank
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, whiteQueen, 0), moveList); //Promotes to a white queen
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, whiteRook, 0), moveList); //Promotes to a white rook
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, whiteBishop, 0), moveList); //Promotes to a white bishop
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, whiteKnight, 0), moveList); //Promotes to a white knoght
    }else{
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, emptyPiece, 0), moveList); //Does not promote
    }
}

void addWhitePawnMove(const Board *position, const int originalSquare, const int destinationSquare, MoveList *moveList){ //Checks if a white pawn is promoting
    if(indexToFiles[originalSquare] == seventhRank){ //If the pawn if from the seventh rank
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, whiteQueen, 0), moveList); //Promotes to a white queen
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, whiteRook, 0), moveList); //Promotes to a white rook
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, whiteBishop, 0), moveList); //Promotes to a white bishop
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, whiteKnight, 0), moveList); //Promotes to a white knoght
    }else{
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, emptyPiece, 0), moveList); //Does not promote
    }
}

void generateAllMoves(const Board *position,  MoveList *moveList){ // Generates all possible moves
    ASSERT(checkBoard(position));

    moveList->count = 0;
}