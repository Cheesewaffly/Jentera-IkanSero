#include "Definitions.h"

#define move(origin, destination, capture, promotion, castleOrEnPassantOrDoubleMove) (origin | (destination << 7) | (capture << 14) | (promotion << 20) | castleOrEnPassantOrDoubleMove) //A move

int slidingPiecesLoop[8] = {whiteBishop, whiteRook, whiteQueen, 0 , blackBishop, blackRook, blackQueen, 0}; //To loop between sliding pieces
int jumpingPiecesLoop[6] = {whiteKnight, whiteKing,  0 , blackKnight, blackKing, 0}; //To loop between jumping pieces
int slidingPiecesLoopIndex[2] = {0, 4}; //Tells when to start the piece sliding loop
int jumpingPiecesLoopIndex[2] = {0, 3}; //Tells when to start the piece jumping loop
int pieceDirections[13][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, //Move directions of an empty piece
    {0, 0, 0, 0, 0, 0, 0, 0}, //Move directions of a pawn
    {-21, -19, -12, -8, 8, 12, 19, 21}, //Move directions of a knight
    {-11, -9, 9, 11}, //Move directions of a bishop
    {-10, -1, 1, 10}, //Move directions of a rook
    {-10, -1, 1, 10, -11, -9, 9, 11}, //Move directions of a queen
    {-10, -1, 1, 10, -11, -9, 9, 11}, //Move directions of a king
    {0, 0, 0, 0, 0, 0, 0, 0}, //Move directions of a pawn
    {-21, -19, -12, -8, 8, 12, 19, 21}, //Move directions of a knight
    {-11, -9, 9, 11}, //Move directions of a bishop
    {-10, -1, 1, 10}, //Move directions of a rook
    {-10, -1, 1, 10, -11, -9, 9, 11}, //Move directions of a queen
    {-10, -1, 1, 10, -11, -9, 9, 11}, //Move directions of a king
};
int numberOfDirections[13] = {0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8};

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
    ASSERT(isPieceValidOrEmpty(capturedPiece)); //Checks if the captured piece is valid
    ASSERT(isSquareOnTheBoard(originalSquare)); //Checks if the original square is valid
    ASSERT(isSquareOnTheBoard(destinationSquare)); //Checks if the destination square is valid

    if(indexToRanks[originalSquare] == seventhRank){ //If the pawn if from the seventh rank
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, whiteQueen, 0), moveList); //Promotes to a white queen
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, whiteRook, 0), moveList); //Promotes to a white rook
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, whiteBishop, 0), moveList); //Promotes to a white bishop
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, whiteKnight, 0), moveList); //Promotes to a white knoght
    } else {
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, emptyPiece, 0), moveList); //Does not promote
    }
}

void addWhitePawnMove(const Board *position, const int originalSquare, const int destinationSquare, MoveList *moveList){ //Checks if a white pawn is promoting
    ASSERT(isSquareOnTheBoard(originalSquare)); //Checks if the original square is valid
    ASSERT(isSquareOnTheBoard(destinationSquare)); //Checks if the destination square is valid
    
    if(indexToRanks[originalSquare] == seventhRank){ //If the pawn if from the seventh rank
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, whiteQueen, 0), moveList); //Promotes to a white queen
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, whiteRook, 0), moveList); //Promotes to a white rook
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, whiteBishop, 0), moveList); //Promotes to a white bishop
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, whiteKnight, 0), moveList); //Promotes to a white knoght
    } else {
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, emptyPiece, 0), moveList); //Does not promote
    }
}

void addBlackPawnCaptureMove(const Board *position, const int originalSquare, const int destinationSquare, const int capturedPiece, MoveList *moveList){ //Checks if a black pawn is promoting
    ASSERT(isPieceValidOrEmpty(capturedPiece)); //Checks if the captured piece is valid
    ASSERT(isSquareOnTheBoard(originalSquare)); //Checks if the original square is valid
    ASSERT(isSquareOnTheBoard(destinationSquare)); //Checks if the destination square is valid

    if(indexToRanks[originalSquare] == secondRank){ //If the pawn if from the second rank
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, blackQueen, 0), moveList); //Promotes to a black queen
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, blackRook, 0), moveList); //Promotes to a black rook
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, blackBishop, 0), moveList); //Promotes to a black bishop
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, blackKnight, 0), moveList); //Promotes to a black knoght
    } else {
        addCaptureMove(position, move(originalSquare, destinationSquare, capturedPiece, emptyPiece, 0), moveList); //Does not promote
    }
}

void addBlackPawnMove(const Board *position, const int originalSquare, const int destinationSquare, MoveList *moveList){ //Checks if a black pawn is promoting
    ASSERT(isSquareOnTheBoard(originalSquare)); //Checks if the original square is valid
    ASSERT(isSquareOnTheBoard(destinationSquare)); //Checks if the destination square is valid

    if(indexToRanks[originalSquare] == secondRank){ //If the pawn if from the second rank
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, blackQueen, 0), moveList); //Promotes to a black queen
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, blackRook, 0), moveList); //Promotes to a black rook
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, blackBishop, 0), moveList); //Promotes to a black bishop
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, blackKnight, 0), moveList); //Promotes to a black knoght
    } else {
        addCaptureMove(position, move(originalSquare, destinationSquare, emptyPiece, emptyPiece, 0), moveList); //Does not promote
    }
}


void generateAllMoves(const Board *position,  MoveList *moveList){ // Generates all possible moves
    ASSERT(checkBoard(position)); //Checks if the board is in order

    moveList->count = 0;

    int piece = emptyPiece; //The default piece is an empty square
    int side = position->side; //the side playing
    int square = 0; //The location of a piece
    int destinationSquare = 0; //the square the piece will go to
    int direction = 0; //The direction of a move
    int pieceIndex = 0; //To index a piece

    if(side==white){ //if the side playing is white
        for(int index = 0; index < position->pieceNumber[whitePawn]; index++){ //Loops through every white pawn on the board
            square = position->pieceList[whitePawn][index]; //Takes the position of that white pawn
            ASSERT(isSquareOnTheBoard(square)); //Checks if square is valid

            if(position->boardPieces[square + 10] == emptyPiece){ //Checks if the square in fron of the pawn is empty
                addWhitePawnMove(position, square, square + 10, moveList); //Adds that move to the movelist
                if(indexToRanks[square] == secondRank && position->boardPieces[square + 20] == emptyPiece){ //checks if it's on the second rank with two squares empty in front of it
                    addNonCaptureMove(position, move(square, square + 20, emptyPiece, emptyPiece, doublePawnMove), moveList); //Adds that move to the movelist
                }
            }

            if(isSquareOnTheBoard(square + 9) && pieceColour[position->boardPieces[square + 9]] == black){ //Adds capture to the move list
                addWhitePawnCaptureMove(position, square, square + 9, position->boardPieces[square + 9], moveList); //Adds that move to the movelist
            }

            if(isSquareOnTheBoard(square + 11) && pieceColour[position->boardPieces[square + 11]] == black){ //Adds capture to the left
                addWhitePawnCaptureMove(position, square, square + 11, position->boardPieces[square + 11], moveList); //Adds that move to the movelist
            }

            if(square + 9 == position->enPassantSquare){ //Adds capture to the en passant right
                addCaptureMove(position, move(square, square + 9, emptyPiece, emptyPiece, enPassantMove), moveList); //Adds that move to the movelist
            }

            if(square + 11 == position->enPassantSquare){ //Adds capture to the en passant left
                addCaptureMove(position, move(square, square + 11, emptyPiece, emptyPiece, enPassantMove), moveList); //Adds that move to the movelist
            }
        }
    } else { //if the side playing is black
        for(int index = 0; index < position->pieceNumber[blackPawn]; index++){ //Loops through every black pawn on the board
            square = position->pieceList[blackPawn][index]; //Takes the position of that black pawn
            ASSERT(isSquareOnTheBoard(square)); //Checks if square is valid

            if(position->boardPieces[square - 10] == emptyPiece){ //Checks if the square in fron of the pawn is empty
                addBlackPawnMove(position, square, square - 10, moveList); //Adds that move to the movelist
                if(indexToRanks[square] == seventhRank && position->boardPieces[square - 20] == emptyPiece){ //checks if it's on the seventh rank with two squares empty in front of it
                    addNonCaptureMove(position, move(square, square - 20, emptyPiece, emptyPiece, doublePawnMove), moveList); //Adds that move to the movelist
                }
            }

            if(isSquareOnTheBoard(square - 9) && pieceColour[position->boardPieces[square - 9]] == white){ //Adds capture to the move list
                addBlackPawnCaptureMove(position, square, square - 9, position->boardPieces[square - 9], moveList); //Adds that move to the movelist
            }

            if(isSquareOnTheBoard(square - 11) && pieceColour[position->boardPieces[square - 11]] == white){ //Adds capture to the left
                addBlackPawnCaptureMove(position, square, square - 11, position->boardPieces[square - 11], moveList); //Adds that move to the movelist
            }

            if(square - 9 == position->enPassantSquare){ //Adds capture to the en passant right
                addCaptureMove(position, move(square, square - 9, emptyPiece, emptyPiece, enPassantMove), moveList); //Adds that move to the movelist
            }

            if(square - 11 == position->enPassantSquare){ //Adds capture to the en passant left
                addCaptureMove(position, move(square, square - 11, emptyPiece, emptyPiece, enPassantMove), moveList); //Adds that move to the movelist
            }
        }
    }

    pieceIndex = slidingPiecesLoopIndex[side]; //Tells the sliding pieces array when to start the loop (depensing on the color)
    piece = slidingPiecesLoop[pieceIndex]; //Tells what pieces are used then move to the next piece
    while(piece){ //While the value of piece is not 0
        ASSERT(isPieceValid(piece)); //Checks if the piece is valid

        piece = slidingPiecesLoop[++pieceIndex]; //Go to the next piece
    }

    pieceIndex = jumpingPiecesLoopIndex[side]; //Tells the jumping pieces array when to start the loop (depensing on the color)
    piece = jumpingPiecesLoop[pieceIndex]; //Tells what pieces are used then move to the next piece
    while(piece){ //While the value of piece is not 0
        ASSERT(isPieceValid(piece)); //Checks if the piece is valid

        for(int index = 0; index < position->pieceNumber[piece]; ++index){ //Loops through every jumping piece on the board
            square = position->pieceList[piece][index]; //Takes the position of that piece
            ASSERT(isSquareOnTheBoard(square)); //Checks if square is valid
            cout << "a " << pieceCharacter[piece] << " is on " << printAlgebraicSquareNotation(square) << endl;

            for(int directionIndex = 0; directionIndex < numberOfDirections[piece]; ++directionIndex){
                direction = pieceDirections[piece][directionIndex];
                destinationSquare = square + direction;
                if(indexToFiles[destinationSquare] == offBoardSquare){ //Checks if the square is off-board
                    continue;
                }

                if(position->boardPieces[destinationSquare]){ //Checks if theres a piece on that destination square
                    if(pieceColour[position->boardPieces[destinationSquare]] == side ^ 1){
                        cout << "Capture Move on " << printAlgebraicSquareNotation(destinationSquare) << endl;
                    }
                    continue;
                }

                cout << "Normal Move on " << printAlgebraicSquareNotation(destinationSquare) << endl;
            }
        }
        piece = jumpingPiecesLoop[++pieceIndex]; //Go to the next piece
    }
    
}