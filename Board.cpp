#include "Definitions.h"

int checkBoard(const Board *position){ //A function to crosscheck wether the information on the board is correct
    int checkPieceTypeNumber[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //Sets up to check the piece number
    int checkBigPiecesnumber[2] = {0, 0}; //Sets up to check the big piece number
    int checkMajorPiecesnumber[2] = {0, 0}; //Sets up to check the major piece number
    int checkMinorPiecesnumber[2] = {0, 0}; //Sets up to check the minor piece number
    int checkMaterialScore[2] = {0, 0};//Sets up to check the material value on the board
    int checkSquare120Array, checkSquare64Array, colour, checkPieces, checkPieceNumber, checkPawnCount; //To check between the arrays, check pieces and piece numbers alongside their colour
    
    U64 checkPawnBitBoards[3] = {0ULL, 0ULL, 0ULL}; //Sets up to check the pawn bitboards

    checkPawnBitBoards[white] = position->pawnBitBoards[white]; //Sets up to check the white pawn bitboards
    checkPawnBitBoards[black] = position->pawnBitBoards[black]; //Sets up to check the black pawn bitboards
    checkPawnBitBoards[both] = position->pawnBitBoards[both]; //Sets up to check the pawn bitboards

    for(checkPieces = whitePawn; checkPieces <= blackKing; ++checkPieces){ //Loops through all piece types
        for(checkPieceNumber = 0; checkPieceNumber < position->pieceNumber[checkPieces]; ++checkPieceNumber){ //Checks the amount of that piece type that is on the board
            checkSquare120Array = position->pieceList[checkPieces][checkPieceNumber]; //Gets that type of piece an its amount to be checked
            ASSERT(position->boardPieces[checkSquare120Array] == checkPieces) //Checks if the piece being checked is actually there
        }
    }

    for(checkSquare64Array = 0; checkSquare64Array < 64; ++checkSquare64Array){ //Check piece number and other counters
        checkSquare120Array = array64ToArray120[checkSquare64Array]; //convert the 64 array index to a 120 array index
        checkPieces = position->boardPieces[checkSquare120Array]; //Looks at a piece on a square
        checkPieceTypeNumber[checkPieces]++; //Adds that piece to the previous total of that piece's type on the board
        colour = pieceColour[checkPieces]; //Gets the colour of that piece

        if(bigPieces[checkPieces]){checkBigPiecesnumber[colour]++;} //Adds a big piece to that color
        if(minorPieces[checkPieces]){checkMinorPiecesnumber[colour]++;} //Adds a minor piece to that color
        if(majorPieces[checkPieces]){checkMajorPiecesnumber[colour]++;} //Adds a major piece to that color

        checkMaterialScore[colour] += pieceValue[checkPieces]; //Adds that piece's value to the total material score on each color
    }

    for(checkPieces = whitePawn; checkPieces <= blackKing; ++checkPieces){ //Loops through all piece types
        ASSERT(checkPieceTypeNumber[checkPieces] == position->pieceNumber[checkPieces]) //Checks if the amount of that piece type is the same
    }

    checkPawnCount = countBit(checkPawnBitBoards[white]); //Gets the number of white pawns
    ASSERT(checkPawnCount == position->pieceNumber[whitePawn]) //Makes sure the number of white pawns is the same
    checkPawnCount = countBit(checkPawnBitBoards[black]); //Gets the number of black pawns
    ASSERT(checkPawnCount == position->pieceNumber[blackPawn]) //Makes sure the number of black pawns is the same
    checkPawnCount = countBit(checkPawnBitBoards[both]); //Gets the number of pawns
    ASSERT(checkPawnCount == position->pieceNumber[whitePawn] + position->pieceNumber[blackPawn]) //Makes sure the number of pawns is the same

    while(checkPawnBitBoards[white]){
        checkSquare64Array = popBit(&checkPawnBitBoards[white]); //Gets the bitboard index for that pawn (its location)
        ASSERT(position->boardPieces[array64ToArray120[checkSquare64Array]] == whitePawn); //Check if there is indeed a white pawn there
    }
    while(checkPawnBitBoards[black]){
        checkSquare64Array = popBit(&checkPawnBitBoards[black]); //Gets the bitboard index for that pawn (its location)
        ASSERT(position->boardPieces[array64ToArray120[checkSquare64Array]] == blackPawn); //Check if there is indeed a white pawn there
    }
    while(checkPawnBitBoards[both]){
        checkSquare64Array = popBit(&checkPawnBitBoards[both]); //Gets the bitboard index for that pawn (its location)
        ASSERT(position->boardPieces[array64ToArray120[checkSquare64Array]] == blackPawn || position->boardPieces[array64ToArray120[checkSquare64Array]] == whitePawn); //Check the pawn location
    }

    ASSERT(checkMaterialScore[white] == position->materialScore[white] && checkMaterialScore[black] == position->materialScore[black]); //Check if the material score is the same
    ASSERT(checkBigPiecesnumber[white] == position->bigPiecesNumber[white] && checkBigPiecesnumber[black] == position->bigPiecesNumber[black]); //Check if the big piece number is the same
    ASSERT(checkMinorPiecesnumber[white] == position->minorPiecesNumber[white] && checkMinorPiecesnumber[black] == position->minorPiecesNumber[black]); //More checks for minor pieces
    ASSERT(checkMajorPiecesnumber[white] == position->majorPiecesNumber[white] && checkMajorPiecesnumber[black] == position->majorPiecesNumber[black]); //More checks for major pieces
    ASSERT(white == position->side || black == position->side); //Check if a side is actually playing
    ASSERT(generatePositionKeys(position) == position->positionKey); //Checks if the position key is the same
    ASSERT(position->enPassantSquare == emptySquare || (indexToRanks[position->enPassantSquare] == sixthRank && position->side == white) || (indexToRanks[position->enPassantSquare] == thirdRank && position->side == black)) //The en passant square
    ASSERT(position->boardPieces[position->kingSquare[white]] == whiteKing) //Checks if the white king is in place
    ASSERT(position->boardPieces[position->kingSquare[black]] == blackKing) //Checks if the black king is in place

    return 1;
}

void updateMaterialList(Board *position){ //counts the number of pieces and their classifications
    for(int index = 0; index < boardSquareNumber; ++index){ //Loops through all of the squares in the 120 array board
        int square120Array = index; //Sets a 120 array index
        int piece = position->boardPieces[index]; //Takes the piece on that square

        if(piece != offBoardSquare && piece != emptyPiece){
            int colour = pieceColour[piece]; //Specifies the color of the pieces

            if(bigPieces[piece]){position->bigPiecesNumber[colour]++;} //Adds a big piece to that color
            if(minorPieces[piece]){position->minorPiecesNumber[colour]++;} //Adds a minor piece to that color
            if(majorPieces[piece]){position->majorPiecesNumber[colour]++;} //Adds a major piece to that color
            if(piece == whiteKing){position->kingSquare[white] = square120Array;} //Saves the white king's position
            if(piece == blackKing){position->kingSquare[black] = square120Array;} //Saves the black king's position
            if(piece == whitePawn){ //Checks if that piece is a white pawn
                setBitBoard(position->pawnBitBoards[white], array120ToArray64[square120Array]); //sets that index's bit in the white pawn bitboard as 1
                setBitBoard(position->pawnBitBoards[both], array120ToArray64[square120Array]); //sets that index's bit in the pawn bitboard as 1
            }
            else if(piece == blackPawn){//Checks if that piece is a black pawn
                setBitBoard(position->pawnBitBoards[black], array120ToArray64[square120Array]); //sets that index's bit in the black pawn bitboard as 1
                setBitBoard(position->pawnBitBoards[both], array120ToArray64[square120Array]); //sets that index's bit in the pawn bitboard as 1
            }

            position->materialScore[colour] += pieceValue[piece]; //Adds that piece's value to the total material score on each color
            position->pieceList[piece][position->pieceNumber[piece]] = index; //Stores the types of pieces and their position on the board
            position->pieceNumber[piece]++; //Adds that piece to the previous total of that piece's type on the board
        }
    }
}

void parseFENString(const char *FENString, Board *position){ //To set up the board using a FEN string

    ASSERT(FENString != NULL); //Makes sure there is actually a valid string
    ASSERT(position != NULL); //Makes sure there is actually a valid position

    int piece; //A placeholder for the pieces
    int rank = eighthRank; //rank variable initializer
    int file = aFile; //initializes this variable so it remains consistent througout this function
    int count; //To initialize the count variable


    resetBoard(position); //Resets the position

    while((rank >= firstRank) && *FENString){ //Goes throug the FEN string, reading each character it points to, starting with the top-most rank
        count = 1;
        switch(*FENString){ //Assign each character with a different condition
            case 'p': piece = blackPawn; break; //Basically sets the character into a piece in this case a black pawn
            case 'n': piece = blackKnight; break; //Sets up a black knight
            case 'b': piece = blackBishop; break; //Sets up a black bishop
            case 'r': piece = blackRook; break; //Sets up a black rook
            case 'q': piece = blackQueen; break; //Sets up a black queen
            case 'k': piece = blackKing; break; //Sets up a black king
            case 'P': piece = whitePawn; break; //Sets up a white pawn
            case 'N': piece = whiteKnight; break; //Sets up a white knight
            case 'B': piece = whiteBishop; break; //Sets up a white bishop
            case 'R': piece = whiteRook; break; //Sets up a white rook
            case 'Q': piece = whiteQueen; break; //Sets up a white queen
            case 'K': piece = whiteKing; break; //Sets up a white king

            case '1': //In the case of 1 empty square after the last piece
            case '2': //In the case of 2 empty square after the last piece
            case '3': //In the case of 3 empty square after the last piece
            case '4': //In the case of 4 empty square after the last piece
            case '5': //In the case of 5 empty square after the last piece
            case '6': //In the case of 6 empty square after the last piece
            case '7': //In the case of 7 empty square after the last piece
            case '8': //In the case of 8 empty square after the last piece
                piece = emptyPiece; //Sets that number of squares into empty squares
                count = *FENString - '0'; //substracts the ASCII value so that it returns an int
                break;
            
            case '/': //Sets up the next rank
            case ' ': //End of the pieces initialization
                rank--; //Move on to the next rank
                file = aFile; //Starts from the first file
                FENString++; //move along to the next character
                continue; //Continues the while loop
            
            default: //Everything else will come out as an error
                cout << "FEN error" << endl;
        }

        for(int _ = 0; _ < count; _++){ //Actually putting the pieces on the board
            int square64Index = rank * 8 + file; //Gets the 64 array index
            int square120Index = array64ToArray120[square64Index]; //Gets the 120 array index
            if(piece != emptyPiece){ //Checks if the square is empty
                position->boardPieces[square120Index] = piece; //If its not empty then theres a piece there
            }

            file++; //Move along to the next file
        }

        FENString++; //move along to the next character
    }

    ASSERT(*FENString == 'w' || *FENString == 'b'); //Makes sure we are on the side to play indicator character
    position->side = (*FENString == 'w') ? white:black; //Checks if that character is white to play or else its black to play
    FENString += 2; //Moves on to the castling permission indicator characters

    for(int _ = 0; _ < 4; _++){ //Loops through the castling permission indicator which consists of 1-4 characters
        if(*FENString == ' '){break;} //Breaks the loop if it hits a blank space

        switch(*FENString){ //if its not a blank space then it will combine the permissions together (using the OR operator)
            case 'K': position->castlePermission |= whiteKingsideCastling; //White kingside castling (0001)
            case 'Q': position->castlePermission |= whiteQueensideCastling; //White queenside castling (0010)
            case 'k': position->castlePermission |= blackKingsideCastling; //Black kingside castling (0100)
            case 'q': position->castlePermission |= blackQueensideCastling; //Black queenside castling (1000)
            default: break; //If there is a '-' then break the loop
        }

        FENString++; //move on to the next character
    }

    FENString++;
    ASSERT(position->castlePermission >= 0 && position->castlePermission <= 15) //Makes sure the castle permission value is within the defined range

    if(*FENString != '-'){ //If its not a '-' then there is an en passant square
        file = FENString[0] - 'a'; //the first character of that square is the file, this returns the numerical value of that letter (the file number)
        rank = FENString[1] - '1'; //the first character of that square is the rank, this returns the numerical value of that number (the rank number)

        ASSERT(file >= aFile && file <=hFile); //Makes sure the file is in the range defined
        ASSERT(rank >= firstRank && rank<=eighthRank); //Makes sure the rank is in the range defined

        position->enPassantSquare = coordinatesTo120ArrayIndex(file, rank); //assigns the en passant square to a 120 array index
    }

    position->positionKey = generatePositionKeys(position); //This generates the position key
    updateMaterialList(position);
}

void resetBoard(Board *position){ //A function to reset basically everything the board
    for(int index = 0; index < boardSquareNumber; ++index){ //Loops through all the squares on the 120 array board
        position->boardPieces[index] = offBoardSquare; //Sets all these squares to an off-board square, which is basically the same as an empty square i don't know why we're differentiating
    }

    for(int index = 0; index < 64; ++index){ //Loops through all the squares on the 64 array board
        position->boardPieces[array64ToArray120[index]] = emptyPiece; //Sets all these squares to an empty square
    }

    for(int index = 0; index < 3; ++index){ //Loops through all the squares on the board for the classifications of the pieces
        position->bigPiecesNumber[index] = 0; //Resets the number of big pieces on the board
        position->minorPiecesNumber[index] = 0; //Resets the number of minor pieces on the board
        position->majorPiecesNumber[index] = 0; //Resets the number of major pieces on the board
        position->materialScore[index] = 0; //Resets the material score on the board
        position->pawnBitBoards[index] = 0ULL; //Resets the number of pawns on the board;
    }

    for(int index = 0; index < 13; ++index){ //Loops through all the squares on the board for the number of the types of pieces
        position->pieceNumber[index] = 0; //Resets the number of pieces on the board
    }

    position->kingSquare[white] = position->kingSquare[black] = emptySquare; //Sets the king squares to empty squares
    position->side = both; //Sets the side to play to both (neither white nor black)
    position->enPassantSquare = emptySquare; //Sets ne en passant square to an empty square
    position->fiftyMoveRule = 0; //Resets the fifty move counter
    position->castlePermission = 0; //Resets castling permissions
    position->ply = 0; //Resets the half-move count
    position->hisPly = 0; //Resets the half-move history
    position->positionKey = 0ULL; //Resets the current position key
}

void printBoard(const Board *position){ //To print the board on the console
    cout << "Game Board: " << endl;

    for(int rank = eighthRank; rank >= firstRank; rank--){ //Loops through every rank on the board
        cout << rank + 1 << " "; //Prints the rank number
        for(int file = aFile; file <= hFile; file++){ //Loops through every file on the board
            int square120Index = coordinatesTo120ArrayIndex(file, rank); //Gets the 120 array index
            int piece = position->boardPieces[square120Index]; //Gets the piece
            cout << " " << pieceCharacter[piece] << " "; //Prints the piece
        }
        cout << endl;
    }
    cout << "  "; //So it alligns
    for(int file = aFile; file <= hFile; file++){ //Loops through every file on the board
        cout << " " << char(file + 'a') << " "; //Prints the file letter
    }
    cout << endl;
    cout << "Side to play: " << char(sideCharacter[position->side]) << endl; //Prints which side is it to play
    cout << "En passant square: " << dec << position->enPassantSquare << endl; //Prints the en passant square as a 120 square index
    cout << char(position->castlePermission & whiteKingsideCastling ? 'K':'-'); //prints if white can castle kingside
    cout << char(position->castlePermission & whiteQueensideCastling ? 'Q':'-'); //prints if white can castle queenside
    cout << char(position->castlePermission & blackKingsideCastling ? 'k':'-'); //prints if black can castle kingside
    cout << char(position->castlePermission & blackQueensideCastling ? 'q':'-') << endl; //prints if black can castle queenkingside
    cout << "Position key: " << hex << uppercase << position->positionKey << dec << endl << endl; //Prints the current position key
}