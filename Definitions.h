#pragma once
#include <cstdlib> // The C standard library for some useful functions
#include <iostream>
#include <bitset>

using namespace std;

#define NAME "Jentera IkanSero v1.0" //This just replaces every NAME with "Jentera IkanSero v1.0" in the code
#define boardSquareNumber 120 //We are using a 12x10 board representation
#define maxGameMoves 2048 //The maximum number of moves in a game, to save memory
#define maxPositionMoves 256 //The maximum number of moves in a given position
#define coordinatesTo120ArrayIndex(files, ranks) ((21 + files) + (10 * ranks)) //Converts the coordinates of a square to the index of the 120 array
#define clearBitBoard(bitBoard, index) ((bitBoard) &= clearBitBoardMask[index]) //Clears the bitboard
#define setBitBoard(bitBoard, index) ((bitBoard) |= setBitBoardMask[index]) //Sets the bitboard
#define ASSERT(error) if(!(error)){cout << #error << " - Failed in file " << __FILE__ << " at line " << __LINE__; exit(1);} // Error detection
#define originalSquare(square) (square & 0x7F) //Used to make a move, the origin square is represented by the first 7 bits of a 28 bit number   (0000 0000 0000 0000 0000 0111 1111)
#define destinationSquare(square) ((square >> 7) & 0x7F) //The destination square is represented by the next 7 bits of a 28 bit number          (0000 0000 0000 0011 1111 1000 0000)
#define pieceCaptured(piece) ((piece >> 14) & 0xF)  //The piece captured is represented by the next 4 bits of a 28 bit number                   (0000 0000 0011 1100 0000 0000 0000)
#define enPassantMove 0x40000 //The en passant move is represented by the next bit of a 28 bit number                                           (0000 0000 0100 0000 0000 0000 0000)
#define doublePawnMove 0x80000 //The double pawn move is represented by the next bit of a 28 bit number                                         (0000 0000 1000 0000 0000 0000 0000)
#define piecePromotedTo(piece) ((piece >> 20) & 0xF)  //The piece promoted to is represented by the next 4 bits of a 28 bit number              (0000 1111 0000 0000 0000 0000 0000)
#define castlingMove 0x1000000 //The castling move is represented by the next bit of a 28 bit number                                            (0001 0000 0000 0000 0000 0000 0000) 
#define isCapture  0x7C000 //Checks if a move is a capture
#define isPromotion  0xF00000 //Checks if a move is a promotion

typedef unsigned long long U64; //U64 is a 64-bit number, one bit for each square of the board

enum{emptyPiece, whitePawn, whiteKnight, whiteBishop, whiteRook, whiteQueen, whiteKing, blackPawn, blackKnight, blackBishop, blackRook, blackQueen, blackKing}; //Each piece is assigned a number
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
    A8=91, B8, C8, D8, E8, F8, G8, H8, emptySquare, offBoardSquare};    //Each square on the board is assigned a number
enum{
    whiteKingsideCastling=1, whiteQueensideCastling=2,  //Each castling configuration on the board is assigned a number binarily
    blackKingsideCastling=4, blackQueensideCastling=8}; //e.g. 0101 = white and black can castle queenside, 1100 = white can castle both side, black cant castle either side, etc.

struct Move{
    int move;
    int postitionScore;
};

struct MoveList{ //All the possible moves given a position
    Move moves[maxPositionMoves]; //The possible move list in a given position
    int count; //To count how many moves in a position
};

struct UndoMove{
    int move; //Stores the number of moves that has been played
    int enPassant; //Stores en passant squares
    int fiftyMoveRule; //Stores the fifty move counter
    int castlePermission; // Stores the castling permission

    U64 positionKey; //Stores the positions
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
    int bigPiecesNumber[2]; //Stores how many big pieces (not pawns) are on each side
    int majorPiecesNumber[2]; //Stores how many major pieces (rooks and queens) are on each side, used to evaluate endgames
    int minorPiecesNumber[2]; //Stores how many minor pieces (bishops and knights) are on each side, used to evaluate endgames
    int castlePermission; // Stores the castling permission
    int pieceList[13][10]; //List of each piece type on the board
    int materialScore[2]; //The material score for each side

    U64 pawnBitBoards[3]; //Bitboard representation for pawns
    U64 positionKey; //Stores the positions

    UndoMove history[maxGameMoves]; //Stores the UndoMove class info
};

extern void initializeAll(); //Function to initialize everything
extern void printBitBoard(U64 bitBoard); //Function to print bitboard
extern void resetBoard(Board *position); //A function to reset basically everything the board
extern void printBoard(const Board *position); //To print the board on the console
extern void parseFENString(const char *FENString, Board *position); //To set up the board using a FEN string
extern void updateMaterialList(Board *position); //counts the number of pieces and their classifications

extern int array120ToArray64[boardSquareNumber]; //To convert the 12x10 index to a 8x8 index (BAD PRACTICE TO DEFINE GLOBAL VARIABLES!!)
extern int array64ToArray120[64]; //To convert the 8x8 index to a 12x10 index
extern int popBit(U64 *bitBoardAddress); //This basically pops out (removes) 1 bit from the board (Don't ask how)
extern int countBit(U64 bitBoard); //Takes a bitboard and counts the number of bits inside it
extern int bigPieces[13]; //Specifies all non-pawn pieces
extern int majorPieces[13]; //Specifies the rook and the queen
extern int minorPieces[13]; //Specifies the bishop and the knight
extern int pieceColour[13]; //Specifies the piece color
extern int pieceValue[13]; //Specifies the value of each piece
extern int indexToFiles[boardSquareNumber]; //Converts the index to a file letter
extern int indexToRanks[boardSquareNumber]; //Converts the index to a rank number
extern int checkBoard(const Board *position);  //A function to crosscheck wether the information on the board is correct
extern int isItAKnight[13]; //A silly way to check if a piece is a knight
extern int isItAKing[13]; //A silly way to check if a piece is a king
extern int isItABishopOrQueen[13]; //A silly way to check if a piece is a bishop or queen
extern int isItARookOrQueen[13]; //A silly way to check if a piece is a rook or queen

extern U64 setBitBoardMask[64]; // The set bitboard mask array
extern U64 clearBitBoardMask[64]; // The clear bitboard mask array
extern U64 pieceHashKeys[13][boardSquareNumber]; //Hashes (stores) the pieces and their positions
extern U64 sideHashKey; // Hashes which side is it to move
extern U64 castleHashKey[16]; // Hashes the current castling permissions
extern U64 generatePositionKeys(const Board *position); //Generates a unique key for each position

extern char pieceCharacter[]; //the piece symbols
extern char sideCharacter[]; //the side symbols
extern char rankCharacter[]; //the rank symbols
extern char fileCharacter[]; //the file symbols
extern char *printAlgebraicSquareNotation(const int square); //Converts the square index to an algebraic notation
extern char *printMove(const int move); //Prints the move notation

extern bool squareUnderAttack(const int square, const int attackingSide, const Board *position); //Determines if a square is under attack
extern bool isSquareOnTheBoard(const int square); //checks if the square is on the board
extern bool isASidePlaying(const int side); //checks a side is playing
extern bool isFileOrRankValid(const int fileOrRank); //checks if the square is valid
extern bool isPieceValidOrEmpty(const int piece); //checks if the piece is valid or empty
extern bool isPieceValid(const int piece); //checks if the piece is valid