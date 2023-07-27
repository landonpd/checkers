#ifndef Piece_H
#define Piece_H
#include"Pos.h"
class Piece{
public:
Piece(); //default constructor
Piece(const bool COLOR,const Pos POS); //overloaded constructor to put in what info about the piece immediately 
~Piece(); //destructor to delete pieces
Piece& operator=(const Piece* TARGET);

//getters and setters for all variables except team, team does not need a setter since it should never change
bool color() const; //returns team
bool kinged() const; //returns if the piece is a king
Pos pos();
void setPos(const Pos POS);
void kingMe();//changes king to true 

private:
bool mColor;//whether the piece is an x or o
bool mKinged;//whether the piece is a king or not
Pos mPos;//where the piece is on the board, Pos will be a struct to hold coordinates
};
#endif