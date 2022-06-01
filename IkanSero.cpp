#include "Initialization.h"

int main(){
    initializeAll();

    Unsigned64 firstBitBoard = 0ULL;

    firstBitBoard |= (1ULL << array120ToArray64[A2]);
    firstBitBoard |= (1ULL << array120ToArray64[B2]);
    firstBitBoard |= (1ULL << array120ToArray64[C2]);

    printBitBoard(firstBitBoard);
    cout << countBit(firstBitBoard) << endl;
    cout << "==============" << endl;
    cout << popBit(&firstBitBoard) << endl;
    printBitBoard(firstBitBoard);
    cout << countBit(firstBitBoard) << endl;
    cout << "==============" << endl;
    cout << popBit(&firstBitBoard) << endl;
    printBitBoard(firstBitBoard);
    cout << countBit(firstBitBoard) << endl;
    cout << "==============" << endl;
    cout << popBit(&firstBitBoard) << endl;
    printBitBoard(firstBitBoard);
}