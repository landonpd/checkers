#include "Square.h"
#include "Pos.h"
#include "Piece.h"

Square::Square(){ //default constructor
    
    mColor=true;
    mPiecePresent=false;
    
}
Square::Square(const bool COLOR,const bool PIECEPRESENT){ //overloaded constructor to set all values
    mColor=COLOR;
    mPiecePresent=PIECEPRESENT;

} 
bool Square::color()const{
    return mColor;
}
bool Square::piecePresent()const{
    return mPiecePresent;
}

//setters for piecepresent
void Square::movePiece(){
    mPiecePresent=!mPiecePresent;
}

void Square::setColor(const bool NEWCOLOR){
    mColor=NEWCOLOR;
}

