#include "definitions.hpp"

#define move(origin, destination, capture, promotion, castleOrEnPassantOrDoubleMove) (origin | (destination << 7) | (capture << 14) | (promotion << 20) | castleOrEnPassantOrDoubleMove) //A move

const int slidingPiecesLoop[8] = {whiteBishop, whiteRook, whiteQueen, 0 , blackBishop, blackRook, blackQueen, 0}; //To loop between sliding pieces
const int jumpingPiecesLoop[6] = {whiteKnight, whiteKing,  0 , blackKnight, blackKing, 0}; //To loop between jumping pieces
const int slidingPiecesLoopIndex[2] = {0, 4}; //Tells when to start the piece sliding loop
const int jumpingPiecesLoopIndex[2] = {0, 3}; //Tells when to start the piece jumping loop
const int pieceDirections[13][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, //Move directions of an empty piece
    {0, 0, 0, 0, 0, 0, 0, 0}, //Move directions of a pawn
    {-21, -19, -12, -8, 8, 12, 19, 21}, //Move directions of a knight
    {-11, -9, 9, 11, 0, 0, 0, 0}, //Move directions of a bishop
    {-10, -1, 1, 10, 0, 0, 0, 0}, //Move directions of a rook
    {-10, -1, 1, 10, -11, -9, 9, 11}, //Move directions of a queen
    {-10, -1, 1, 10, -11, -9, 9, 11}, //Move directions of a king
    {0, 0, 0, 0, 0, 0, 0, 0}, //Move directions of a pawn
    {-21, -19, -12, -8, 8, 12, 19, 21}, //Move directions of a knight
    {-11, -9, 9, 11, 0, 0, 0, 0}, //Move directions of a bishop
    {-10, -1, 1, 10, 0, 0, 0, 0}, //Move directions of a rook
    {-10, -1, 1, 10, -11, -9, 9, 11}, //Move directions of a queen
    {-10, -1, 1, 10, -11, -9, 9, 11}, //Move directions of a king
};
const int numberOfDirections[13] = {0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8};

static void addNonCaptureMove(const Board *position, int move, MoveList *moveList){ //Adds a non capture move to the movelist
    moveList->moves[moveList->count].move = move; //The movelist structure contains an array of the move structure (hence the . operator)
    moveList->moves[moveList->count].postitionScore = 0;
    moveList->count++; //Adds that move to the counter
}

static void addCaptureMove(const Board *position, int move, MoveList *moveList){ //Adds a capture move to the movelist
    moveList->moves[moveList->count].move = move; //The movelist structure contains an array of the move structure (hence the . operator)
    moveList->moves[moveList->count].postitionScore = 0;
    moveList->count++; //Adds that move to the counter
}

static void addEnPassantMove(const Board *position, int move, MoveList *moveList){ //Adds an en passant capture move to the movelist
    moveList->moves[moveList->count].move = move; //The movelist structure contains an array of the move structure (hence the . operator)
    moveList->moves[moveList->count].postitionScore = 0;
    moveList->count++; //Adds that move to the counter
}

static void addWhitePawnCaptureMove(const Board *position, const int originalSquare, const int destinationSquare, const int capturedPiece, MoveList *moveList){ //Checks if a pawn is promoting
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

static void addWhitePawnMove(const Board *position, const int originalSquare, const int destinationSquare, MoveList *moveList){ //Checks if a white pawn is promoting
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

static void addBlackPawnCaptureMove(const Board *position, const int originalSquare, const int destinationSquare, const int capturedPiece, MoveList *moveList){ //Checks if a pawn is promoting
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

static void addBlackPawnMove(const Board *position, const int originalSquare, const int destinationSquare, MoveList *moveList){ //Checks if a black pawn is promoting
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

    moveList->count = 0; //counts how many moves in a position

    int piece = emptyPiece; //The default piece is an empty square
    int side = position->side; //the side playing
    int square{}; //The location of a piece
    int targetSquare{}; //the square the piece will go to
    int direction{}; //The direction of a move
    int pieceIndex{}; //To index a piece

    if(side == white){ //if the side playing is white
        for(int index{}; index < position->pieceNumber[whitePawn]; index++){ //Loops through every white pawn on the board
            square = position->pieceList[whitePawn][index]; //Takes the position of that white pawn
            ASSERT(isSquareOnTheBoard(square)); //Checks if square is valid

            if(position->boardPieces[square + 10] == emptyPiece){ //Checks if the square in fron of the pawn is empty
                addWhitePawnMove(position, square, square + 10, moveList); //Adds that move to the movelist
                if(indexToRanks[square] == secondRank && position->boardPieces[square + 20] == emptyPiece){ //checks if it's on the second rank with two squares empty in front of it
                    addNonCaptureMove(position, move(square, square + 20, emptyPiece, emptyPiece, doublePawnMove), moveList); //Adds that move to the movelist
                }
            }

            if(indexToFiles[square + 9] != offBoardSquare && pieceColour[position->boardPieces[square + 9]] == black){ //Adds capture to the move list
                addWhitePawnCaptureMove(position, square, square + 9, position->boardPieces[square + 9], moveList); //Adds that move to the movelist
            }

            if(indexToFiles[square + 11] != offBoardSquare && pieceColour[position->boardPieces[square + 11]] == black){ //Adds capture to the left
                addWhitePawnCaptureMove(position, square, square + 11, position->boardPieces[square + 11], moveList); //Adds that move to the movelist
            }

            if(position->enPassantSquare != emptySquare){ //Checks if the en passant square is on the board
                if(square + 9 == position->enPassantSquare){ //Adds capture to the en passant right
                    addEnPassantMove(position, move(square, square + 9, emptyPiece, emptyPiece, enPassantMove), moveList); //Adds that move to the movelist
                }

                if(square + 11 == position->enPassantSquare){ //Adds capture to the en passant left
                    addEnPassantMove(position, move(square, square + 11, emptyPiece, emptyPiece, enPassantMove), moveList); //Adds that move to the movelist
                }
            }
        }

        if(position->castlePermission & whiteKingsideCastling){ //Checks if kingside castling is still possible
            if(position->boardPieces[F1] == emptyPiece && position->boardPieces[G1] == emptyPiece){ //Checks if G1 and F1 is empty for kingside castling
                if(!squareUnderAttack(E1, black, position) && !squareUnderAttack(F1, black, position)){ //Checks if E1 and F1 is under attack
                    addNonCaptureMove(position, move(E1, G1, emptyPiece, emptyPiece, castlingMove), moveList); //Adds the move to the move list
                }
            }

        }

        if(position->castlePermission & whiteQueensideCastling){ //Checks if queenside castling is still possible
            if(position->boardPieces[D1] == emptyPiece && position->boardPieces[C1] == emptyPiece && position->boardPieces[B1] == emptyPiece){ //Checks if D1, C1 and B1 is empty for castling
                if(!squareUnderAttack(E1, black, position) && !squareUnderAttack(D1, black, position)){ //Checks if E1 and D1 is under attack
                    addNonCaptureMove(position, move(E1, C1, emptyPiece, emptyPiece, castlingMove), moveList); //Adds the move to the move list
                }
            }

        }
    }else{ //if the side playing is black
        for(int index{}; index < position->pieceNumber[blackPawn]; index++){ //Loops through every black pawn on the board
            square = position->pieceList[blackPawn][index]; //Takes the position of that black pawn
            ASSERT(isSquareOnTheBoard(square)); //Checks if square is valid

            if(position->boardPieces[square - 10] == emptyPiece){ //Checks if the square in fron of the pawn is empty
                addBlackPawnMove(position, square, square - 10, moveList); //Adds that move to the movelist
                if(indexToRanks[square] == seventhRank && position->boardPieces[square - 20] == emptyPiece){ //checks if it's on the seventh rank with two squares empty in front of it
                    addNonCaptureMove(position, move(square, square - 20, emptyPiece, emptyPiece, doublePawnMove), moveList); //Adds that move to the movelist
                }
            }

            if(indexToFiles[square - 9] != offBoardSquare && pieceColour[position->boardPieces[square - 9]] == white){ //Adds capture to the move list
                addBlackPawnCaptureMove(position, square, square - 9, position->boardPieces[square - 9], moveList); //Adds that move to the movelist
            }

            if(indexToFiles[square - 11] != offBoardSquare && pieceColour[position->boardPieces[square - 11]] == white){ //Adds capture to the left
                addBlackPawnCaptureMove(position, square, square - 11, position->boardPieces[square - 11], moveList); //Adds that move to the movelist
            }

            if(position->enPassantSquare != emptySquare){ //Checks if the en passant square is on the board
                if(square - 9 == position->enPassantSquare){ //Adds capture to the en passant right
                    addEnPassantMove(position, move(square, square - 9, emptyPiece, emptyPiece, enPassantMove), moveList); //Adds that move to the movelist
                }

                if(square - 11 == position->enPassantSquare){ //Adds capture to the en passant left
                    addEnPassantMove(position, move(square, square - 11, emptyPiece, emptyPiece, enPassantMove), moveList); //Adds that move to the movelist
                }
            }
        }

        if(position->castlePermission & blackKingsideCastling){ //Checks if kingside castling is still possible
            if(position->boardPieces[F8] == emptyPiece && position->boardPieces[G8] == emptyPiece){ //Checks if G8 and F8 is empty for kingside castling
                if(!squareUnderAttack(E8, white, position) && !squareUnderAttack(F8, white, position)){ //checks if E8 and F8 is under attack
                    addNonCaptureMove(position, move(E8, G8, emptyPiece, emptyPiece, castlingMove), moveList); //Adds the move to the move list
                }
            }

        }

        if(position->castlePermission & blackQueensideCastling){ //Checks if queenside castling is still possible
            if(position->boardPieces[D8] == emptyPiece && position->boardPieces[C8] == emptyPiece && position->boardPieces[B8] == emptyPiece){ //Checks if D8, C8 and B8 is empty for castling
                if(!squareUnderAttack(E8, white, position) && !squareUnderAttack(D8, white, position)){ //checks if E8 and D8 is under attack
                    addNonCaptureMove(position, move(E8, C8, emptyPiece, emptyPiece, castlingMove), moveList); //Adds the move to the move list
                }
            }

        }
    }

    pieceIndex = slidingPiecesLoopIndex[side]; //Tells the sliding pieces array when to start the loop (depensing on the color)
    piece = slidingPiecesLoop[pieceIndex++]; //Tells what pieces are used then move to the next piece
    while(piece){ //While the value of piece is not 0
        ASSERT(isPieceValid(piece)); //Checks if the piece is valid

        for(int index{}; index < position->pieceNumber[piece]; ++index){ //Loops through every sliding piece on the board
            square = position->pieceList[piece][index]; //Takes the position of that piece
            ASSERT(isSquareOnTheBoard(square)); //Checks if square is valid

            for(int directionIndex{}; directionIndex < numberOfDirections[piece]; ++directionIndex){ //Loops through all the directrion the piece can go to
                direction = pieceDirections[piece][directionIndex]; //Gets the direction
                targetSquare = square + direction; //Sets the destination square

                while(indexToFiles[targetSquare] != offBoardSquare){ // keep going while the destination square still on the board
                    if(position->boardPieces[targetSquare] != emptyPiece){ //Checks if theres a piece on that destination square
                        if(pieceColour[position->boardPieces[targetSquare]] == side ^ 1){ //if the piece on the destination square a different color from the piece moving
                            addCaptureMove(position, move(square, targetSquare, position->boardPieces[targetSquare], emptyPiece, 0), moveList); //Adds the move to the move list
                        }
                        break; //breaks the loops once the piece hits another piece
                    }
                    addNonCaptureMove(position, move(square, targetSquare, emptyPiece, emptyPiece, 0), moveList); //Adds the move to the move list
                    targetSquare += direction; // Move along that direction
                }
            }
        }
        piece = slidingPiecesLoop[pieceIndex++]; //Go to the next piece
    }

    pieceIndex = jumpingPiecesLoopIndex[side]; //Tells the jumping pieces array when to start the loop (depensing on the color)
    piece = jumpingPiecesLoop[pieceIndex++]; //Tells what pieces are used then move to the next piece
    while(piece){ //While the value of piece is not 0
        ASSERT(isPieceValid(piece)); //Checks if the piece is valid

        for(int index{}; index < position->pieceNumber[piece]; ++index){ //Loops through every jumping piece on the board
            square = position->pieceList[piece][index]; //Takes the position of that piece
            ASSERT(isSquareOnTheBoard(square)); //Checks if square is valid

            for(int directionIndex{}; directionIndex < numberOfDirections[piece]; ++directionIndex){ //Loops through all the directrion the piece can go to
                direction = pieceDirections[piece][directionIndex]; //Gets the direction
                targetSquare = square + direction; //Sets the destination square

                if(indexToFiles[targetSquare] == offBoardSquare){ //Checks if the square is off-board
                    continue;
                }

                if(position->boardPieces[targetSquare] != emptyPiece){ //Checks if theres a piece on that destination square
                    if(pieceColour[position->boardPieces[targetSquare]] == side ^ 1){ //if the piece on the destination square a different color from the piece moving
                        addCaptureMove(position, move(square, targetSquare, position->boardPieces[targetSquare], emptyPiece, 0), moveList); //Adds the move to the move list
                    }
                    continue;
                }
                addNonCaptureMove(position, move(square, targetSquare, emptyPiece, emptyPiece, 0), moveList); //Adds the move to the move list
            }
        }
        piece = jumpingPiecesLoop[pieceIndex++]; //Go to the next piece
    }

}