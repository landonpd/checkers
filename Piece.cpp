#include "Piece.h"
#include"Pos.h"
Piece::Piece(){
    mPos.x=0;
    mPos.y=0;
    mKinged=false;
    mColor=false;
} //default constructor
Piece::Piece(const bool COLOR,const Pos POS){
    mColor=COLOR;
    mPos=POS;
    mKinged=false;
} //overloaded constructor to put in what info about the piece immediately 
Piece::~Piece(){

}//destructor to delete pieces, is it needed

Piece& Piece::operator=(const Piece* TARGET){
    this->mPos=TARGET->mPos;
    this->mKinged=TARGET->mKinged;
    this->mColor=TARGET->mColor;
    return *this;
}

//getters and setters for all variables except team, team does not need a setter since it should never change
bool Piece::color() const{
    return mColor;
} //returns team
bool Piece::kinged() const{
    return mKinged;
} //returns if the piece is a king
Pos Piece::pos(){
    return mPos;
}

void Piece::setPos(const Pos POS){
    mPos=POS;
}

void Piece::kingMe(){
    mKinged=true;
}//changes king to true 