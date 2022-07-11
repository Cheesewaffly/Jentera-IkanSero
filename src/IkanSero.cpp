#include "definitions.hpp"

int main(){
    initializeAll();

    Board board[1];
    MoveList moveList[1];

    parseFENString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", board);

    char input[6];
    int move{};

    while(true){
        printBoard(board);
        std::cout << "Your move: ";
        std::cin >> input;

        if(input[0] == 'q'){break;}
        else if(input[0] == 't'){undoMove(board);}
        else{
            move = parseMove(input, board);
            if(move){makeMove(board, move);}
            else{std::cout << "Move is invalid and/or impossible\n";}
        }
    }
}