#include "Definitions.h"

#define hashPiece(piece, square) (position->positionKey ^= pieceHashKeys[piece][square]) //Hashes the piece in and out of the position key
#define hashCastling (position->positionKey ^= castleHashKey[position->castlePermission]) //Hashes castling in and out of the position key
#define hashSide (position->positionKey ^= sideHashKey) //Hashes the side playing in and out of the position key
#define hashEnPassant (position->positionKey ^= pieceHashKeys[emptyPiece][position->enPassantSquare]) //Hashes the en passant move in and out of the position key


const int castlingPermissionsArray[120] = {     //Used to change the castling permissions
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,     //If the H1 rook moves the castling permission would be 1110 (14)
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,     //If the A1 rook moves the castling permission would be 1101 (13)
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,     //If the white king moves the castling permission would be 1100 (12)
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,     //If the H* rook moves the castling permission would be 1011 (11)
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,     //If the A8 rook moves the castling permission would be 0111 (7)
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,     //If the black king moves the castling permission would be 0011 (3)
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,     //else the castling permission is 1111 (15)
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,     //AND (&) the value of this array with the latest castling permission
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

static void clearPiece(const int square, Board *position){ //Clears a piece from the board
    ASSERT(isSquareOnTheBoard(square)); //Checks if the square is on the board

    int piece = position->boardPieces[square]; //Gets the piece on that square
    ASSERT(isPieceValid(piece)); //Checks if the piece is valid

    int colour = pieceColour[piece]; //Gets the colour of the piece
    int pieceIndex = -1; //To find the piece we are about to clear

    hashPiece(piece, square); //Hashes out the piece from the position key

    position->boardPieces[square] = emptyPiece; //Clears the piece from that square
    position->materialScore[colour] -= pieceValue[piece]; //Substracts the piece value from the material score

    if(bigPieces[piece]){ //If the piece is not a pawn
        position->bigPiecesNumber[colour]--; //Decrements the big pieces number
        if(majorPieces[piece]){ //If the piece is a rook or queen
            position->majorPiecesNumber[colour]--;  //Decrements the major pieces number
        }else{ //If the piece is a bishop or knight
            position->minorPiecesNumber[colour]--; //Decrements the minor pieces number
        }
    }else{ //If the piece is a pawn
        clearBit(position->pawnBitBoards[colour], array120ToArray64[square]); //Clear that colour's pawn bitboard
        clearBit(position->pawnBitBoards[both], array120ToArray64[square]); //Clears the pawn bitboard
    }

    for(int index = 0; index < position->pieceNumber[piece]; ++index){ //Loops through all of that piece's type on the board
        if(position->pieceList[piece][index] == square){ //if its actually the piece we're looking for
            pieceIndex = index; //sets that piece's index to the piece index
            break;
        }
    }

    ASSERT(pieceIndex != -1); //Checks if the piece is real

    position->pieceNumber[piece]--; //Decrement the piece's type total number
    position->pieceList[piece][pieceIndex] = position->pieceList[piece][position->pieceNumber[piece]]; //Changes the value to that empty square to the last one on the list
}

static void addPiece(const int square, Board *position, const int piece){ //Adds a piece from the board
    ASSERT(isSquareOnTheBoard(square)); //Checks if the square is on the board
    ASSERT(isPieceValid(piece)); //Checks if the piece is valid

    int colour = pieceColour[piece]; //Gets the colour of the piece

    hashPiece(piece, square); //Hashes in the piece from the position key

    position->boardPieces[square] = piece; //Adds the piece to that square

    if(bigPieces[piece]){ //If the piece is not a pawn
        position->bigPiecesNumber[colour]++; //increments the big pieces number
        if(majorPieces[piece]){ //If the piece is a rook or queen
            position->majorPiecesNumber[colour]++;  //increments the major pieces number
        }else{ //If the piece is a bishop or knight
            position->minorPiecesNumber[colour]++; //increments the minor pieces number
        }
    }else{ //If the piece is a pawn
        setBit(position->pawnBitBoards[colour], array120ToArray64[square]); //adds a bit to that colour's pawn bitboard
        setBit(position->pawnBitBoards[both], array120ToArray64[square]); //adds a bit to the pawn bitboard
    }

    position->materialScore[colour] += pieceValue[piece]; //Adds the piece value from the material score
    position->pieceList[piece][position->pieceNumber[piece]++] = square; //Changes the value of the square of that piece on the piece list
}

static void movePiece(int originalSquare, int destinationSquare, Board *position){ //Moves the piece on the board
    ASSERT(isSquareOnTheBoard(originalSquare)); //Checks if the original square is valid
    ASSERT(isSquareOnTheBoard(destinationSquare)); //Checks if the destination square is valid
    
    int piece = position->boardPieces[originalSquare]; //Gets the piece on that square
    int colour = pieceColour[piece]; //Gets the colour of the piece

    #ifdef DEBUG //Only in debug mode (for debugging)
        int pieceIndex = false;
    #endif

    hashPiece(piece, originalSquare); //Hashes out the piece from the original square from the position key
    position->boardPieces[originalSquare] = emptyPiece; //Clears the piece from that square

    hashPiece(piece, destinationSquare); //Hashes in the piece to the destination square from the position key
    position->boardPieces[destinationSquare] = piece; //Adds the piece to that square

    if(!bigPieces[piece]){ //If the piece is a pawn
        clearBit(position->pawnBitBoards[colour], array120ToArray64[originalSquare]); //Clear that colour's pawn bitboard
        clearBit(position->pawnBitBoards[both], array120ToArray64[originalSquare]); //Clears the pawn bitboard
        setBit(position->pawnBitBoards[colour], array120ToArray64[destinationSquare]); //adds a bit to that colour's pawn bitboard
        setBit(position->pawnBitBoards[both], array120ToArray64[destinationSquare]); //adds a bit to the pawn bitboard
    }

    for(int index = 0; index < position->pieceNumber[piece]; ++index){ //Loops through all of that piece's type on the board
        if(position->pieceList[piece][index] == originalSquare){ //if its actually the piece we're looking for
            position->pieceList[piece][index] = destinationSquare; //changes the location of the piece

            #ifdef DEBUG
            pieceIndex = true;
            #endif

            break;
        }
    }
    ASSERT(pieceIndex); //for debugging purpossses
}

bool makeMove(Board *position, int move){ //To make the move
    ASSERT(checkBoard(position)); //Checks if the board is in order

    int originalSquare = originalSquare(move); //Gets the original square of the move
    int destinationSquare = destinationSquare(move); //Gets the destination square of the move
    int side = position->side; //Gets the side who is playing

    ASSERT(isSquareOnTheBoard(originalSquare)); //Checks if the original square is valid
    ASSERT(isSquareOnTheBoard(destinationSquare)); //Checks if the destination square is valid
    ASSERT(isASidePlaying(side)) //Checks if the side is valid
    ASSERT(isPieceValid(position->boardPieces[originalSquare])) //Checks if the piece is valid

    position->history[position->historyPly].positionKey = position->positionKey; //Stores the position key in the history

    if(move & enPassantMove){ //If the move is an en passant move
        if(side == white){ //If white to move
            clearPiece(destinationSquare-10, position); //captures a black pawn
        }else{ //If black to move
            clearPiece(destinationSquare+10, position); //captures a white pawn
        }
    }
    
    else if(move & castlingMove){ //if the move is a castling move
        switch(destinationSquare){
            case C1: //if the king moves to C1
                movePiece(A1, D1, position); //Move the rook to D1
                break;
            case C8: //if the king moves to C8
                movePiece(A8, D8, position);//Move the rook to D8
                break;
            case G1: //if the king moves to G1
                movePiece(H1, F1, position); //Move the rook to F1
                break;
            case G8: //if the king moves to G8
                movePiece(H8, F8, position);//Move the rook to F8
                break;
            default: //If its none of the above
                ASSERT(false); //return an error
                break;
        }
    }

    if(position->enPassantSquare != emptySquare){hashEnPassant;} //hashes the en passant square out of the position key (to be replaced, or be set to empty square)
    
    hashCastling; //hashes out the castling permissions out of the position key

    position->history[position->historyPly].move = move; //stores the move
    position->history[position->historyPly].fiftyMoveRule = position->fiftyMoveRule; //stores the fifty move rule
    position->history[position->historyPly].enPassantSquare = position->enPassantSquare; //stores the en passant square
    position->history[position->historyPly].castlePermission = position->castlePermission; //stores the castle permissions
    
    position->castlePermission &= castlingPermissionsArray[originalSquare]; //updates the castling permission
    position->castlePermission &= castlingPermissionsArray[destinationSquare]; //updates the castling permission
    position->enPassantSquare = emptySquare;

    hashCastling;  //hashes in the updated castling permissions in the position key
    
    position->fiftyMoveRule++; //increments the fifty move rule

    int capturedPiece = pieceCaptured(move); //Gets the piece captured
    if(capturedPiece != emptyPiece){ //If the piece captured is not an empty piece
        ASSERT(isPieceValid(capturedPiece)); //Checks if the piece captured is valid
        clearPiece(destinationSquare, position); //Clears the piece on that board
        position->fiftyMoveRule = 0; //Resets the fifty move rule
    }

    position->historyPly++; //Increments the ply history
    position->ply++; //Increments the half-moves that are searched

    if(pawnPieces[position->boardPieces[originalSquare]]){ //If its a pawn move
        position->fiftyMoveRule = 0; //Resets the fifty move rule
        if(move & doublePawnMove){ //If its a double pawn move
            if(side == white){ //If white to move
                position->enPassantSquare = originalSquare + 10; //Sets the en passant square
                ASSERT(indexToRanks[position->enPassantSquare] == thirdRank); //Checks if the en passant square is correct
            }else{ //If black to move
                position->enPassantSquare = originalSquare - 10; //Sets the en passant square
                ASSERT(indexToRanks[position->enPassantSquare] == sixthRank); //Checks if the en passant square is correct
            }
            hashEnPassant; //Hashes the en passant square back in the position key
        }
    }

    movePiece(originalSquare, destinationSquare, position); //Finally moves the piece to the destination square

    int promotedPiece = piecePromotedTo(move); //Gets the piece promoted to
    if(promotedPiece != emptyPiece){
        ASSERT(isPieceValid(promotedPiece) && !pawnPieces[promotedPiece]); //checks if the piece promoted to is valid
        clearPiece(destinationSquare, position); //Clears the promotion square
        addPiece(destinationSquare, position, promotedPiece); //Adds the piece promoted to
    }

    if(kingPieces[position->boardPieces[destinationSquare]]){ //if its a king move
        position->kingSquare[position->side] = destinationSquare; //Updates the position of the king
    }

    position->side ^= 1; //Switches the side (position->side is now enemy side, side is the original move maker's side)
    hashSide; //Hashes the side to the position key

    ASSERT(checkBoard(position)); //Checks if the board is in order

    if(squareUnderAttack(position->kingSquare[side], position->side, position)){ //if the king is in check
        undoMove(position); //undoes the move
        return false;
    }

    return true;
} 

void undoMove(Board *position){ //To undo the move
    ASSERT(checkBoard(position)); //Checks if the board is in order

    position->historyPly--; //Decrements the ply history
    position->ply--; //Decrements the half-moves that are searched

    int move = position->history[position->historyPly].move; //Gets the move from history
    int originalSquare = originalSquare(move); //Gets the original square of the move
    int destinationSquare = destinationSquare(move); //Gets the destination square of the move

    ASSERT(isSquareOnTheBoard(originalSquare)); //Checks if the original square is valid
    ASSERT(isSquareOnTheBoard(destinationSquare)); //Checks if the destination square is valid

    if(position->enPassantSquare != emptySquare){hashEnPassant;} //hashes the en passant square out of the position key (to be replaced, or be set to empty square)
    hashCastling; //hashes out the castling permissions out of the position key
    
    position->fiftyMoveRule = position->history[position->historyPly].fiftyMoveRule; //stores the fifty move rule
    position->enPassantSquare = position->history[position->historyPly].enPassantSquare; //stores the en passant square
    position->castlePermission = position->history[position->historyPly].castlePermission; //stores the castle permissions

    if(position->enPassantSquare != emptySquare){hashEnPassant;} //hashes the en passant square out of the position key (to be replaced, or be set to empty square)
    hashCastling; //hashes out the castling permissions out of the position key

    position->side ^= 1; //Switches the side
    hashSide; //Hashes the side to the position key

    if(move & enPassantMove){ //If the move is an en passant move
        if(position->side == white){ //If white to move
            addPiece(destinationSquare-10, position, blackPawn); //Readds a black pawn
        }else{ //If black to move
            addPiece(destinationSquare+10, position, whitePawn); //Readds a white pawn
        }
    }
    
    else if(move & castlingMove){ //if the move is a castling move
        switch(destinationSquare){
            case C1: //if the king moves to C1
                movePiece(D1, A1, position); //Move the rook back to A1
                break;
            case C8: //if the king moves to C8
                movePiece(D8, A8, position);//Move the rook back to A8
                break;
            case G1: //if the king moves to G1
                movePiece(F1, H1, position); //Move the rook back to H1
                break;
            case G8: //if the king moves to G8
                movePiece(F8, H8, position);//Move the rook back to H8
                break;
            default: //If its none of the above
                ASSERT(false); //return an error
                break;
        }
    }

    movePiece(destinationSquare, originalSquare, position); //Finally moves the piece to the original square

    if(kingPieces[position->boardPieces[originalSquare]]){ //if its a king move
        position->kingSquare[position->side] = originalSquare; //Updates the position of the king
    }

    int capturedPiece = pieceCaptured(move); //Gets the piece captured
    if(capturedPiece != emptyPiece){ //If the piece captured is not an empty piece
        ASSERT(isPieceValid(capturedPiece)); //Checks if the piece captured is valid
        addPiece(destinationSquare, position, capturedPiece); //Clears the piece on that board
    }

    int promotedPiece = piecePromotedTo(move); //Gets the piece promoted to
    if(promotedPiece != emptyPiece){
        ASSERT(isPieceValid(promotedPiece) && !pawnPieces[promotedPiece]); //checks if the piece promoted to is valid
        clearPiece(originalSquare, position); //Clears the piece promoted to
        addPiece(originalSquare, position, pieceColour[promotedPiece] == white ? whitePawn:blackPawn); //Adds the pawn back to
    }

    ASSERT(checkBoard(position)); //Checks if the board is in order
}
