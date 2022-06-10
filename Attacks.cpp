#include "Definitions.h"

const int knightMoveDirection[8] = {-21, -19, -12, -8, 8, 12, 19, 21};  //The move direction of the different piece types
const int bishopMoveDirection[4] = {-11, -9, 9, 11};                    //North = -10, south = 10, west = -1, east = 1
const int rookMoveDirection[4] = {-10, -1, 1, 10};                      //Each piece is a combination of these directions
const int kingMoveDirection[8] = {-11, -10, -9, -1, 1, 9, 10, 11};

bool squareUnderAttack(const int square, const int attackingSide, const Board *position){ //Determines if a square is under attack
    int piece, temporarySquare, directionOfAttack; //Some variable definitions

    ASSERT(isSquareOnTheBoard(square)); //checks if the square is on the board
    ASSERT(isASidePlaying(attackingSide)); //checks a side is playing
    ASSERT(checkBoard(position));  //A function to crosscheck wether the information on the board is correct

    if(attackingSide == white){ //Determines if a square is attacked by a pawn
        if(position->boardPieces[square - 11] == whitePawn || position->boardPieces[square - 9] == whitePawn){ //Determines if a square is attacked by a white pawn
            return true;
        }
    } else{
        if(position->boardPieces[square + 11] == blackPawn || position->boardPieces[square + 9] == blackPawn){ //Determines if a square is attacked by a black pawn
            return true;
        }
    }

    for(int index = 0; index < 8; ++index){ //Determines if a square is attacked by a knight
        piece = position->boardPieces[square + knightMoveDirection[index]]; //Gets the piece where a knight should be
        if(piece != offBoardSquare && isItAKnight[piece] && pieceColour[piece] == attackingSide){ //If that piece is a knight of the same colour as the attacking side
            return true;
        }
    }

    for(int index = 0; index < 4; ++index){ //Determines if a square is attacked by a rook or queen
        directionOfAttack = rookMoveDirection[index]; //Checks one of the four directions of a rook
        temporarySquare = square + directionOfAttack; //Continues the squares along that direction of attack
        piece = position->boardPieces[temporarySquare]; //Check if there is or isnt a piece on that square
        while(piece != offBoardSquare){ //Continues to loop as long as its not off-board
            if(piece != emptyPiece){ //Checks if it has hit a piecealong that direction
                if(isItARookOrQueen[piece] && pieceColour[piece] == attackingSide){ //checks if that piece is a rook or a queen
                    return true;
                }
                break;
            }
            temporarySquare += directionOfAttack; //Continues along the direction of attack
            piece = position->boardPieces[temporarySquare]; //Gets that piece on that new square
        }
    }

    for(int index = 0; index < 4; ++index){ //Determines if a square is attacked by a bishop or queen
        directionOfAttack = bishopMoveDirection[index]; //Checks one of the four directions of a bishop
        temporarySquare = square + directionOfAttack; //Continues the squares along that direction of attack
        piece = position->boardPieces[temporarySquare]; //Check if there is or isnt a piece on that square
        while(piece != offBoardSquare){ //Continues to loop as long as its not off-board
            if(piece != emptyPiece){ //Checks if it has hit a piecealong that direction
                if(isItABishopOrQueen[piece] && pieceColour[piece] == attackingSide){ //checks if that piece is a bishop or a queen
                    return true;
                }
                break;
            }
            temporarySquare += directionOfAttack; //Continues along the direction of attack
            piece = position->boardPieces[temporarySquare]; //Gets that piece on that new square
        }
    }

    for(int index = 0; index < 8; ++index){ //Determines if a square is attacked by a king
        piece = position->boardPieces[square + kingMoveDirection[index]]; //Gets the piece where a king should be
        if(piece != offBoardSquare && isItAKing[piece] && pieceColour[piece] == attackingSide){ //If that piece is a king of the same colour as the attacking side
            return true;
        }
    }

    return false;
}