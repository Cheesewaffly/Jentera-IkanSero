#include "Definitions.h"

U64 leafNodes; //the number of leaf nodes

void perft(int depth, Board *position){ //individual perft testing
    ASSERT(checkBoard(position)); //Checks if the board is in order

    if(depth == 0){ //If we have reached the bottom of the tree
        leafNodes++; //Increment the number of leaf nodes
        return;
    }

    MoveList moveList[1]; //otherwise, make a movelist
    generateAllMoves(position, moveList); //generate all moves

    for(int moveNumber = 0; moveNumber < moveList->count; ++moveNumber){ //Loop through all the moves in the movelist
        if(!makeMove(position, moveList->moves[moveNumber].move)){ //ignore all illegal moves
            continue;
        }
        perft(depth - 1, position); //Iterate this function until depth = 0
        undoMove(position); //Undoes the move
    }
}

void perftTest(int depth, Board *position){ //Main perft tester
    ASSERT(checkBoard(position)); //Checks if the board is in order

    cout << endl << "Starting test to depth: " << depth << endl;
    leafNodes = 0; //resets the number of leaf nodes

    MoveList moveList[1]; //otherwise, make a movelist
    generateAllMoves(position, moveList); //generate all moves

    for(int moveNumber = 0; moveNumber < moveList->count; ++moveNumber){ //Loop through all the moves in the movelist
        int move = moveList->moves[moveNumber].move; //Gets the move from the move list
        if(!makeMove(position, moveList->moves[moveNumber].move)){ //ignore all illegal moves
            continue;
        }
        U64 cummulativeLeafNodes = leafNodes; //To calculate the old leaf nodes
        
        perft(depth - 1, position); //Iterate this function until depth = 0
        undoMove(position); //Undoes the move
        
        U64 oldLeafNodes = leafNodes - cummulativeLeafNodes; //The amoung of leafnodes for each root move

        cout << "move " << setw(2) << moveNumber + 1 << ". " << printMove(move) << " : " << oldLeafNodes << endl;
    }
    cout << "Complete: " << leafNodes << " nodes visited" << endl;

    return;
}