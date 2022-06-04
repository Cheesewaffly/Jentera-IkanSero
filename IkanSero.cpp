#include "Definitions.h"

int main()
{
    initializeAll();
    int origin = A2;
    int destination = H7;
    int capture = whiteRook;
    int promotion = blackKing;

    int move = origin | (destination << 7) | (capture << 14) | (promotion << 20);
    cout << "Hexadecimal: " << hex << uppercase << move << dec << endl;
    cout << "Decimal: " << move << endl;
    cout << "Binary: " << bitset<28>(move) << endl;
    cout << "Original Square: " << printAlgebraicSquareNotation(origin) << endl;
    cout << "Destination Square: " << printAlgebraicSquareNotation(destination) << endl;
    cout << "Notation: " << printMove(move) << endl;
}