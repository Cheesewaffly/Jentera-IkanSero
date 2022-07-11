#include "definitions.hpp"
#include <iomanip>

char *printAlgebraicSquareNotation(const int square){ //Converts the square index to an algebraic notation
    static char squareNotation[3]; //needs 3 characters including the enter key

    int file = indexToFiles[square]; //Gets the file from the board
    int rank = indexToRanks[square]; //Gets the rank from the board

    sprintf(squareNotation, "%c%c", ('a' + file), ('1' + rank)); //Generates the algebraic notation

    return squareNotation; //Returns the algebraic notation
}

char *printMove(const int move){ //Prints the move notation
    static char moveNotation[6]; //needs 6 characters including the enter key

    int originalFile = indexToFiles[originalSquare(move)]; //Gets the original file from the board
    int originalRank = indexToRanks[originalSquare(move)]; //Gets the original rank from the board
    int destinationFile = indexToFiles[destinationSquare(move)]; //Gets the destination file from the board
    int destinationRank = indexToRanks[destinationSquare(move)]; //Gets the destination rank from the board
    int promotion = piecePromotedTo(move);

    if(promotion){ //Checks if the move is a promotion
        char promotionCharacter = 'q'; //The default promotion is a queen
        if(isItAKnight[promotion]){ //Checks if the promoted piece is a knight
            promotionCharacter = 'n';
        } else if(isItARookOrQueen[promotion] && !isItABishopOrQueen[promotion]){ //Checks if the promoted piece is a rook
            promotionCharacter = 'r';
        } else if(!isItARookOrQueen[promotion] && isItABishopOrQueen[promotion]){ //Checks if the promoted piece is a rook
            promotionCharacter = 'b';
        }
        sprintf(moveNotation, "%c%c%c%c%c", ('a' + originalFile), ('1' + originalRank), ('a' + destinationFile), ('1' + destinationRank), promotionCharacter); //Builds the move string
    } else {
        sprintf(moveNotation, "%c%c%c%c", ('a' + originalFile), ('1' + originalRank), ('a' + destinationFile), ('1' + destinationRank)); //Builds the move string
    }
    return moveNotation; //Returns the move notation
}

int parseMove(char *characterPointer, Board *position){ //Takes a move from the user
    if(characterPointer[0] < 'a' || characterPointer[0] > 'h'){return false;} //Checks if the original file is valid
    if(characterPointer[1] < '1' || characterPointer[1] > '8'){return false;} //Checks if the original rank is valid
    if(characterPointer[2] < 'a' || characterPointer[2] > 'h'){return false;} //Checks if the destination file is valid
    if(characterPointer[3] < '1' || characterPointer[3] > '8'){return false;} //Checks if the destination rank is valid

    int squareOfOrigin = coordinatesTo120ArrayIndex(characterPointer[0] - 'a', characterPointer[1] - '1'); //Converts the input to a 120 array index
    int destinationSquare = coordinatesTo120ArrayIndex(characterPointer[2] - 'a', characterPointer[3] - '1'); //Converts the input to a 120 array index

    ASSERT(isSquareOnTheBoard(squareOfOrigin) && isSquareOnTheBoard(destinationSquare)) //Validates the squares

    MoveList moveList[1]; //Creates a Movelist
    generateAllMoves(position, moveList); //Generates all possible moves

    int move{}; //The move that is going to be returned
    int promotedPiece = emptyPiece; //Special case for promotions

    for(int moveNumber{}; moveNumber < moveList->count; ++moveNumber){ //Loops through all the moves in the movelist
        move = moveList->moves[moveNumber].move; //Gets that specific move
        if(originalSquare(move) == squareOfOrigin && destinationSquare(move) == destinationSquare){ //If that move is the same as the user input
            promotedPiece = piecePromotedTo(move); //Gets the promoted piece from the move
            if(promotedPiece){ //If the promoted piece is not empty
                if(isItARookOrQueen[promotedPiece] && isItABishopOrQueen[promotedPiece] && characterPointer[4] == 'q'){return move;} //Promotes to a queen
                else if(isItARookOrQueen[promotedPiece] && !isItABishopOrQueen[promotedPiece] && characterPointer[4] == 'r'){return move;} //Promotes to a rook
                else if(!isItARookOrQueen[promotedPiece] && isItABishopOrQueen[promotedPiece] && characterPointer[4] == 'b'){return move;} //Promotes to a bishop
                else if(isItAKnight[promotedPiece] && characterPointer[4] == 'n'){return move;} //Promotes to a knight
                continue;
            }
            return move; //return non promotion move
        }
    }
    return false; //if the move is invalid
}

void printMoveList(const MoveList *moveList){ //Prints out the move list
    std::cout << "Move list: " << moveList->count << "\n";

    for(int index{}; index < moveList->count; ++index){ //Loops through the move list
        int move = moveList->moves[index].move; //Gets the move
        int score = moveList->moves[index].postitionScore; //Gets the score of that move

        std::cout << "Move: " << std::setw(2) << index + 1 << ". " << printMove(move) << ", (score: " << score << ")\n";
    }
    std::cout << "Move list total " << moveList->count << " moves\n";
}