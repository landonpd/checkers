#ifndef Square_H
#define Square_H
//#include "Piece.h"
//#include "Pos.h"

class Square{
public:
    Square();  //default constructor
    Square(const bool PIECEPRESENT,const bool COLOR); //overloaded constructor to set all values

    //getters for all
    bool color() const;
    bool piecePresent() const;

    //setters for piecepresent
    void movePiece();
    //need to be able to change color too
    void setColor(const bool NEWCOLOR);//needed if a red moves to where a black was

private:
   
    bool mColor;
    bool mPiecePresent;

};
#endif