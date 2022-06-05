#include "Definitions.h"

#define move(origin, destination, capture, promotion, castleOrEnPassantOrDoubleMove) (origin | (destination << 7) | (capture << 14) | (promotion << 20) | castleOrEnPassantOrDoubleMove) //A move

int slidingPiecesLoop[8] = {whiteBishop, whiteRook, whiteQueen, 0 , blackBishop, blackRook, blackQueen}; //To loop between sliding pieces
int jumpingPiecesLoop[6] = {whiteKnight, whiteKing,  0 , blackKnight, blackKing}; //To loop between jumping pieces
int slidingPiecesLoopIndex[2] = {0, 4};
int jumpingPiecesLoopIndex[2] = {0, 3};

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
    if(indexToRanks[originalSquare] == seventhRank){ //If the pawn if from the seventh rank
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, whiteQueen, isPromotion), moveList); //Promotes to a white queen
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, whiteRook, isPromotion), moveList); //Promotes to a white rook
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, whiteBishop, isPromotion), moveList); //Promotes to a white bishop
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, whiteKnight, isPromotion), moveList); //Promotes to a white knoght
    } else {
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, emptyPiece, 0), moveList); //Does not promote
    }
}

void addWhitePawnMove(const Board *position, const int originalSquare, const int destinationSquare, MoveList *moveList){ //Checks if a white pawn is promoting
    if(indexToRanks[originalSquare] == seventhRank){ //If the pawn if from the seventh rank
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, whiteQueen, isPromotion), moveList); //Promotes to a white queen
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, whiteRook, isPromotion), moveList); //Promotes to a white rook
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, whiteBishop, isPromotion), moveList); //Promotes to a white bishop
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, whiteKnight, isPromotion), moveList); //Promotes to a white knoght
    } else {
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, emptyPiece, 0), moveList); //Does not promote
    }
}

void addBlackPawnCaptureMove(const Board *position, const int originalSquare, const int destinationSquare, const int capturedPiece, MoveList *moveList){ //Checks if a black pawn is promoting
    if(indexToRanks[originalSquare] == secondRank){ //If the pawn if from the second rank
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, blackQueen, isPromotion), moveList); //Promotes to a black queen
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, blackRook, isPromotion), moveList); //Promotes to a black rook
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, blackBishop, isPromotion), moveList); //Promotes to a black bishop
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, blackKnight, isPromotion), moveList); //Promotes to a black knoght
    } else {
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, emptyPiece, 0), moveList); //Does not promote
    }
}

void addBlackPawnMove(const Board *position, const int originalSquare, const int destinationSquare, MoveList *moveList){ //Checks if a black pawn is promoting
    if(indexToRanks[originalSquare] == secondRank){ //If the pawn if from the second rank
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, blackQueen, isPromotion), moveList); //Promotes to a black queen
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, blackRook, isPromotion), moveList); //Promotes to a black rook
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, blackBishop, isPromotion), moveList); //Promotes to a black bishop
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, blackKnight, isPromotion), moveList); //Promotes to a black knoght
    } else {
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, emptyPiece, 0), moveList); //Does not promote
    }
}


void generateAllMoves(const Board *position,  MoveList *moveList){ // Generates all possible moves
    ASSERT(checkBoard(position)); //Checks if the board is in order

    moveList->count = 0;

    int piece = emptyPiece; //The default piece is an empty square
    int side = position->side; //the side playing
    int pawnSquare = 0;

    if(side==white){ //if the side playing is white
        for(int pieceIndex = 0; pieceIndex < position->pieceNumber[whitePawn]; pieceIndex++){ //Loops through every white pawn on the board
            pawnSquare = position->pieceList[whitePawn][pieceIndex]; //Takes the position of that white pawn
            ASSERT(checkBoard(position)); //Checks if the board is in order

            if(position->boardPieces[pawnSquare + 10] == emptyPiece){ //Checks if the square in fron of the pawn is empty
                addWhitePawnMove(position, pawnSquare, pawnSquare + 10, moveList); //Adds that move to the movelist
                if(indexToRanks[pawnSquare] == secondRank && position->boardPieces[pawnSquare + 20] == emptyPiece){ //checks if it's on the second rank with two squares empty in front of it
                    addNonCaptureMove(position, move(pawnSquare, pawnSquare + 20, emptyPiece, emptyPiece, doublePawnMove), moveList); //Adds that move to the movelist
                }
            }

            if(isSquareOnTheBoard(pawnSquare + 9) && pieceColour[position->boardPieces[pawnSquare + 9]] == black){ //Adds capture to the move list
                addWhitePawnCaptureMove(position, pawnSquare, pawnSquare + 9, position->boardPieces[pawnSquare + 9], moveList); //Adds that move to the movelist
            }

            if(isSquareOnTheBoard(pawnSquare + 11) && pieceColour[position->boardPieces[pawnSquare + 11]] == black){ //Adds capture to the left
                addWhitePawnCaptureMove(position, pawnSquare, pawnSquare + 11, position->boardPieces[pawnSquare + 11], moveList); //Adds that move to the movelist
            }

            if(pawnSquare + 9 == position->enPassantSquare){ //Adds capture to the en passant right
                addCaptureMove(position, move(pawnSquare, pawnSquare + 9, emptyPiece, emptyPiece, enPassantMove), moveList); //Adds that move to the movelist
            }

            if(pawnSquare + 11 == position->enPassantSquare){ //Adds capture to the en passant left
                addCaptureMove(position, move(pawnSquare, pawnSquare + 11, emptyPiece, emptyPiece, enPassantMove), moveList); //Adds that move to the movelist
            }
        }
    } else { //if the side playing is black
        for(int pieceIndex = 0; pieceIndex < position->pieceNumber[blackPawn]; pieceIndex++){ //Loops through every black pawn on the board
            pawnSquare = position->pieceList[blackPawn][pieceIndex]; //Takes the position of that black pawn
            ASSERT(checkBoard(position)); //Checks if the board is in order

            if(position->boardPieces[pawnSquare - 10] == emptyPiece){ //Checks if the square in fron of the pawn is empty
                addBlackPawnMove(position, pawnSquare, pawnSquare - 10, moveList); //Adds that move to the movelist
                if(indexToRanks[pawnSquare] == seventhRank && position->boardPieces[pawnSquare - 20] == emptyPiece){ //checks if it's on the seventh rank with two squares empty in front of it
                    addNonCaptureMove(position, move(pawnSquare, pawnSquare - 20, emptyPiece, emptyPiece, doublePawnMove), moveList); //Adds that move to the movelist
                }
            }

            if(isSquareOnTheBoard(pawnSquare - 9) && pieceColour[position->boardPieces[pawnSquare - 9]] == white){ //Adds capture to the move list
                addBlackPawnCaptureMove(position, pawnSquare, pawnSquare - 9, position->boardPieces[pawnSquare - 9], moveList); //Adds that move to the movelist
            }

            if(isSquareOnTheBoard(pawnSquare - 11) && pieceColour[position->boardPieces[pawnSquare - 11]] == white){ //Adds capture to the left
                addBlackPawnCaptureMove(position, pawnSquare, pawnSquare - 11, position->boardPieces[pawnSquare - 11], moveList); //Adds that move to the movelist
            }

            if(pawnSquare - 9 == position->enPassantSquare){ //Adds capture to the en passant right
                addCaptureMove(position, move(pawnSquare, pawnSquare - 9, emptyPiece, emptyPiece, enPassantMove), moveList); //Adds that move to the movelist
            }

            if(pawnSquare - 11 == position->enPassantSquare){ //Adds capture to the en passant left
                addCaptureMove(position, move(pawnSquare, pawnSquare - 11, emptyPiece, emptyPiece, enPassantMove), moveList); //Adds that move to the movelist
            }
        }
    }
    
}