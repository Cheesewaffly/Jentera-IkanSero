#pragma once
#include <stdlib.h>
#include <iostream>

using namespace std;

#define NAME "Jentera IkanSero v1.0" //This just replaces every NAME with "Jentera IkanSero v1.0" in the code
#define boardSquareNumber 120 //We are using a 12x10 board representation
#define maxGameMoves //The maximum number of moves in a game, to save memory
#define coordinatesTo120ArrayIndex(files, ranks) ((21 + files) + (10 * ranks)) //converts the coordinates of a square to the index of the 120 array
#define ASSERT(n) if(!(n)){cout << #n << " - Failed in file " << __FILE__ << " at line " << __LINE__; exit(1);} // Error detection


typedef unsigned long long Unsigned64; //U64 is a 64-bit number, one bit for each square of the board

enum{empty, wP, wN, wB, wR, wQ, wK, bP, bN, bR, bQ, bK}; //Each piece is assigned a number (1=white pawn, 2=white knight, etc.) 
enum{aFile, bFile, cFile, dFile, eFile, fFile, gFile, hFile, emptyFile}; //Each files (columns) of the chess board is assigned a number
enum{firstRank, secondRank, thirdRank, fourthRank, fifthRank, sixthRank, seventhRank, eighthRank, emptyRank}; //Each ranks (lines) of the chess board is assigned a number
enum{white, black, both}; //Each sides is assigned a number
enum{
    A1=21, B1, C1, D1, E1, F1, G1, H1,                  //the 12x10 board consists of 2 lines as an upper and lower border, and a column for the right and left border
    A2=31, B2, C2, D2, E2, F2, G2, H2,                  //which leaves us with an 8x8 board, this makes it easier to generate moves
    A3=41, B3, C3, D3, E3, F3, G3, H3,
    A4=51, B4, C4, D4, E4, F4, G4, H4,
    A5=61, B5, C5, D5, E5, F5, G5, H5,
    A6=71, B6, C6, D6, E6, F6, G6, H6,
    A7=81, B7, C7, D7, E7, F7, G7, H7,
    A8=91, B8, C8, D8, E8, F8, G8, H8, emptySquare};    //Each square on the board is assigned a number
enum{
    whiteKingsideCastling=1, whiteQueensideCastling=2,  //Each castling configuration on the board is assigned a number binarily
    blackKingsideCastling=4, blackQueensideCastling=8}; //e.g. 0101 = white and black can castle queenside, 1100 = white can castle both side, black cant castle either side, etc.

extern int array120ToArray64[boardSquareNumber]; //To convert the 12x10 index to a 8x8 index (BAD PRACTICE TO DEFINE GLOBAL VARIABLE!!)
extern int array64ToArray120[64]; //To convert the 8x8 index to a 12x10 index
extern void initializeAll(); //Function to initialize everything
extern void printBitBoard(Unsigned64 bitBoard); //Function to print bitboard
extern int popBit(Unsigned64 *bitBoardAddress); //This basically pops out (removes) 1 bit from the board (Don't ask how)
int countBit(Unsigned64 bitBoard); //Takes a bitboard and counts the number of bits inside it

struct UndoMove{
    int move; //Stores the number of moves that has been played
    int enPassant; //Stores en passant squares
    int fiftyMoveRule; //Stores the fifty move counter
    int castlePermission; // Stores the castling permission

    Unsigned64 positionKey; //Stores the positions
};

struct Board{
    int boardPieces[boardSquareNumber]; //An array of 120 to represent the board
    int kingSquare[2]; //Stores where the king's squares
    int side; //Stores who's turn is it to move
    int enPassant; //Stores en passant squares
    int fiftyMoveRule; //Stores the fifty move counter
    int ply; //Stores how many half-moves are searched
    int hisPly; //Stores ply history
    int pieceNumber[13]; //Stores the number of the 13 types (including the empty square) of pieces are on the board
    int bigPiecenumber[3]; //Stores how many big pieces (not pawns) are on each side
    int majorPiecesNumber[3]; //Stores how many major pieces (rooks and queens) are on each side, used to evaluate endgames
    int minorPiecesNumber[3]; //Stores how many minor pieces (bishops and knights) are on each side, used to evaluate endgames
    int castlePermission; // Stores the castling permission
    int pieceList[13][10]; // list of each piece type on the board

    Unsigned64 pawns[3]; //Bitboard representation for pawns
    Unsigned64 positionKey; //Stores the positions

    UndoMove history[maxGameMoves]; //Stores the UndoMove class info
};
