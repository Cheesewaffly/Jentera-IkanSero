#include "definitions.hpp"

int main(){
    initializeAll();

    Board board[1];
    MoveList moveList[1];

    parseFENString("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", board);

    for(int _{1}; _ < 14; ++_){
        printBoard(board);
        perftTest(_, board);
    }
    
    // parsePerft(board, 6);
}