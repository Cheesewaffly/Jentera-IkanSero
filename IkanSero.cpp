#include "Definitions.h"

int main()
{
    initializeAll();

    Board board[1];
    MoveList moveList[1];
    
    parseFENString(" ", board);
    perftTest(6, board);
    
    return 0;
}