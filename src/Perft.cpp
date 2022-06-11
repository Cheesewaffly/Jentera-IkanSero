#include "Definitions.h"
#include <fstream>
#include <sstream>

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

    cout << endl << "Starting test to depth " << depth << endl;
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

        cout << "Move " << printMove(move) << ": " << oldLeafNodes << endl;
    }
    cout << "Complete: " << leafNodes << " nodes visited" << endl;

    return;
}

void parsePerft(Board *position, int depth){ //Automatically parse the file to test
    int positionNumber = 1; //Used to count how many position is checked

    ifstream file("./res/perft.txt"); //Gets the file
    if(file.is_open()){ //Opens the file
        string line; //Gets the line in the file

        while(getline(file, line)){ //While the file is open
            const char *perftString = line.c_str();

            ostringstream FENString, depthNodesString[depth]; //Gets the FEN string from the file
            U64 depthNodesInt[depth]; //Stores the leaf nodes in the form of a number

            while(*perftString != ';'){ //Gets the FEN string from the file
                FENString << *perftString; //inserts the character into the string
                perftString++;// moves on to the next character
            }

            cout << endl << "Position " << positionNumber << ": " << FENString.str().c_str() << endl << endl;
            parseFENString(FENString.str().c_str(), position); //Parses the fenstring to the board
            printBoard(position); //Prints the board

            for(int leafDepth = 0; leafDepth < depth; leafDepth++){
                perftString += 4; //Moves on to the leaf nodes number indicator
                
                while(*perftString != ';'){ //Gets the FEN string from the file
                    depthNodesString[leafDepth] << *perftString; //Gets the real leaf nodes to a atring
                    perftString++; //Moves on to the next character
                }
                
                depthNodesInt[leafDepth] = stoull(depthNodesString[leafDepth].str()); //truns the string into a number

                perftTest(leafDepth + 1, position); //Executes the perft test on that depth

                if(leafNodes == depthNodesInt[leafDepth]){ //Checks if the number of leaf nodes is the same o the file
                    cout << "Perft test successful on depth " << leafDepth + 1 << endl;
                }else{
                    cout << "SIKE, THATS THE WRONG NUMBER" << endl;
                    ASSERT(false);
                    system("pause");
                }
            }
            positionNumber++; //Increments the number of positions checked
        }
        file.close(); //Closes the file
        cout << endl << "PERFT TESTING FINISHED, ALL IS IN ORDER" << endl;
    }

}