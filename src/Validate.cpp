#include "definitions.hpp"

bool isSquareOnTheBoard(const int square){ //checks if the square is on the board
    return indexToFiles[square] == offBoardSquare ? false:true; //Returns true if that square is an off-board square
}

bool isASidePlaying(const int side){ //checks a side is playing
    return (side == white || side == black) ? true:false; //Returns true if a side is playing
}

bool isFileOrRankValid(const int fileOrRank){ //checks if the square is valid
    return (fileOrRank >= 0 && fileOrRank <= 7) ? true:false; //Returns true if that square valid
}

bool isPieceValidOrEmpty(const int piece){ //checks if the piece is valid or empty
    return (piece >= emptyPiece && piece <= blackKing) ? true:false; //Returns true if that piece is valid or empty
}

bool isPieceValid(const int piece){ //checks if the piece is valid
    return (piece >= whitePawn && piece <= blackKing) ? true:false; //Returns true if that piece is valid
}