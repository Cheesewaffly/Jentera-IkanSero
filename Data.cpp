#include "Definitions.h"

int bigPieces[13] = {false, false, true, true, true, true, true, false, true, true, true, true, true}; //Specifies all non-pawn pieces
int majorPieces[13] = {false, false, false, false, true, true, true, false, false, false, true, true, true}; //Specifies the rook and the queen
int minorPieces[13] = {false, false, true, true, false, false, false, false, true, true, false, false, false}; //Specifies the bishop and the knight
int pieceValue[13] = {0, 100, 300, 325, 550, 1000, 50000, 100, 300, 325, 550, 1000, 50000}; //Specifies the piece color
int pieceColour[13] = {both, white, white, white, white, white, white, black, black, black, black, black, black}; //Specifies the value of each piece
int isItAKnight[13] = {false, false, true, false, false, false, false, false, true, false, false, false, false}; //A silly way to check if a piece is a knight
int isItAKing[13] = {false, false, false, false, false, false, true, false, false, false, false, false, true}; //A silly way to check if a piece is a king
int isItABishopOrQueen[13] = {false, false, false, true, false, true, false, false, false, true, false, true, false}; //A silly way to check if a piece is a bishop or queen
int isItARookOrQueen[13] = {false, false, false, false, true, true, false, false, false, false, true, true, false}; //A silly way to check if a piece is a rook or queen

char pieceCharacter[] = ".PNBRQKpnbrqk"; //the piece symbols
char sideCharacter[] = "wb-"; //the side to play symbols
char rankCharacter[] = "12345678"; //the rank symbols
char fileCharacter[] = "abcdefgh"; //the file symbols;