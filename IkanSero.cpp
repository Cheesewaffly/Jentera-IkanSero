#include "Definitions.h"

int main(){
    initializeAll();

    Board board[1];
    //parseFENString("rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1", board);
    parseFENString("rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b - e3 0 1", board);
    printBoard(board);
    MoveList moveList[1];
    generateAllMoves(board, moveList);
    printMoveList(moveList);

    system("pause");
}