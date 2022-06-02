#include "Definitions.h"

int main(){
    initializeAll();

    Board board[1];

    parseFENString("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", board);
    printBoard(board);
    ASSERT(checkBoard(board))
}