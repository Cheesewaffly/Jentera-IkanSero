#include "Definitions.h"

char *printAlgebraicSquareNotation(const int square){ //Converts the square index to an algebraic notation
    static char squareNotation[3]; //needs 3 characters e.g e8q (that q is a queen defining a pawn promotion)

    int file = indexToFiles[square]; //Gets the file from the board
    int rank = indexToRanks[square]; //Gets the rank from the board

    sprintf(squareNotation, "%c%c", ('a' + file), ('1' + rank)); //Generates the algebraic notation

    return squareNotation; //Returns the algebraic notation
}

char *printMove(const int move){ //Prints the move notation
    static char moveNotation[6]; //needs 6 characters e.g a1re8q (that q is a queen and r is a rook defining a pawn promotion)

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

void printMoveList(const MoveList *moveList){ //Prints out the move list
    cout << "Move list: " << moveList->count << endl;

    for(int index = 0; index < moveList->count; ++index){ //Loops through the move list
        int move = moveList->moves[index].move; //Gets the move
        int score = moveList->moves[index].postitionScore; //Gets the score of that move

        cout << "Move: " << setw(2) << index + 1 << ". " << printMove(move) << ", (score: " << score << ")" << endl;
    }
    cout << "Move list total " << moveList->count << " moves" << endl;
}