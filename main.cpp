/* CSCI 261 FP: Checkers
 *
 * Author: Landon Dixon
 * 
 *Resources-Some quetions for Dr. Paone in class
 *
 * description-creates checkers played with two people with no chaining jumps, also creates a file with stats from the game when it is over
 */

#include <iostream> 
#include <SFML/Graphics.hpp>
#include<vector>
#include<string>
#include<fstream>
#include "Pos.h"
#include"Piece.h"
#include"Square.h"
using namespace std;
using namespace sf;

//function prototypes

//draws the squares on the board at a position and of a certain color
void drawRect(const Pos POS,const bool COLOR,RenderTarget& window);

//draws a piece onto the screen, also checks if it needs to draw a crown for kinged pieces
void drawPiece(Piece& PIECE,RenderTarget& window);

//draws crown on top of kinged pieces
void drawCrown(const Pos POS,RenderTarget& window);

//converts the actual position of the mouse to just 0-7 to match the positions of the pieces
Pos convertPos(const Vector2i REALPOS);

int main() {
    //all of the variables that need to be made outside of the loop, all the bools and ints get updated to keep track of a stat or the state of a moving piece
    bool turn=true,correctTeam=false,emptySquare=true,jumping=false,landed=false,kinged=false,finished=false; //true is black false is red
    int clicks=0,turns=0,jumpedIndex,pieceIndex,redPieces=8,blackPieces=8,blackKings=0,redKings=0;
    Pos piecePos,jumpedPos;
    string winner;

    // create a window
    RenderWindow window( VideoMode(800,800), "Checkers");


    //Creating all of the pieces and setting them to their initial positions, and keeping them in a list
    vector<Piece> pieces;
    for (int i=0;i<2;i++){
        for (int j=0;j<8;j+=2){
            Pos piecePos(j,i);
            if (i==1){
                piecePos.x+=1;
            }
            Piece piece(true,piecePos);
            pieces.push_back(piece);
        }
    }
    //these are the red piece
    for (int i=7;i>5;i--){
        for (int j=0;j<8;j+=2){
            Pos piecePos(j,i);
            if (i==7){
                piecePos.x+=1;
            }
            Piece piece(false,piecePos);
            pieces.push_back(piece);
        }
    }

    //creating all squares to help keep track of where pieces are
    vector<vector<Square>> squares;
    for (int i=0;i<8;i++){
        vector<Square> squareRow;
        for (int j=0;j<8;j++){
            if(i==0){
                if(j%2==0){
                    Square square(true,true);
                    squareRow.push_back(square);
                }else{
                    Square square;
                    squareRow.push_back(square);
                }
            }else if(i==1){
                if(j%2==1){
                    Square square(true,true);
                    squareRow.push_back(square);
                }else{
                    Square square;
                    squareRow.push_back(square);
                }
            }else if(i==6){
                if(j%2==0){
                    Square square(false,true);
                    squareRow.push_back(square);
                }else{
                    Square square;
                    squareRow.push_back(square);
                }
            }else if(i==7){
                if(j%2==1){
                    Square square(false,true);
                    squareRow.push_back(square);
                }else{
                    Square square;
                    squareRow.push_back(square);
                }
            }else{
                Square square;
                squareRow.push_back(square);
            }
        }
        squares.push_back(squareRow);
    }
   
    //  END  ANY FILE LOADING
    /////////////////////////////////////

    // create an event object once to store future events
    Event event;
    
    
    // while the window is open
    while( window.isOpen() ) {
        // clear any existing contents
        window.clear();

        /////////////////////////////////////    
        //Drawing the board
        int counter=0;
        for (int i=0;i<800;i+=100){
            for (int j=0;j<800;j+=100){
                Pos sqrPos(i,j);
                bool color;
                if (counter%2==0){
                    color=true;
                }else{
                    color=false;
                }
                drawRect(sqrPos,color,window);
                counter++;
            }
            counter++;
        }

        

        //drawing all pieces onto the board, might need to do this at a later spot in the loop
        for(unsigned int i=0;i<pieces.size();++i){
            drawPiece(pieces[i],window);
        }
        // display the current contents of the window
        window.display();

        /////////////////////////////////////
        // BEGIN EVENT HANDLING HERE
        // check if any events happened since the last time checked
        while( window.pollEvent(event) ) {
            

            // if event type corresponds to pressing window X
            if(event.type == Event::Closed) {
                // tell the window to close
                window.close();
            }
            

            if (Mouse::isButtonPressed(Mouse::Left)){//maybe do this first
                Pos mousePos=convertPos(Mouse::getPosition(window));
                //checking if the click was in bounds
                if (mousePos.x<8&&mousePos.x>=0&&mousePos.y<8&&mousePos.y>=0){
                    //checking if this is the first click of the turn
                    if (clicks%2==0){
                        if(squares[mousePos.y][mousePos.x].piecePresent()&&squares[mousePos.y][mousePos.x].color()==turn){
                            clicks++;
                            piecePos=mousePos;
                            for (unsigned int i=0; i<pieces.size();i++){
                                if(pieces[i].pos().x==piecePos.x&&pieces[i].pos().y==piecePos.y){
                                    pieceIndex=i;
                                    break;
                                }
                            }
                        }

                    }else if(clicks%2==1){
                        //checking if the piece that is going to move is kinged
                        if(pieces[pieceIndex].pos().x==piecePos.x&&pieces[pieceIndex].pos().y==piecePos.y&&pieces[pieceIndex].kinged()==true){
                            kinged=true;
                        }
                        
                        //checks if they are red or black to know which way then can move
                        if (turn&&!kinged){
                            if(mousePos.y-1==piecePos.y){
                                correctTeam=true;
                            //checking if the player is trying to jump another player
                            }else if(mousePos.y-2==piecePos.y){
                                correctTeam=true;
                                jumping=true; 
                            }

                        //accounts for kinged pieces being able to move either way
                        }else if(kinged){
                            if(mousePos.y-1==piecePos.y||mousePos.y+1==piecePos.y){
                                correctTeam=true;
                            //checking if the player is trying to jump another player
                            }else if(mousePos.y-2==piecePos.y||mousePos.y+2==piecePos.y){
                                correctTeam=true;
                                jumping=true;
                            }
                        //accounts for red team and not kinged
                        }else{
                            if(mousePos.y+1==piecePos.y){
                                correctTeam=true; 
                            }else if(mousePos.y+2==piecePos.y){
                                correctTeam=true;
                                jumping=true;
                            }
                        }
                        //logic to check if there is a piece of the opposite team being jumped, first finding if there is a piece then saving its position and index
                        if (correctTeam&&jumping){

                            //checks if the position is correct on the x-axis, if going, first all checks for when it is going to the right
                            if(mousePos.x-2==piecePos.x){
                                if(turn&&!kinged){
                                    if(squares[mousePos.y-1][mousePos.x-1].piecePresent()&&squares[mousePos.y-1][mousePos.x-1].color()!=turn){
                                        if(mousePos.y-2==piecePos.y){//needed?
                                            landed=true;
                                            for (unsigned int i=0; i<pieces.size();i++){
                                                if(pieces[i].pos().y==mousePos.y-1&&pieces[i].pos().x==mousePos.x-1){
                                                    jumpedPos=pieces[i].pos();
                                                    jumpedIndex=i;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }else if(kinged){
                                    if(mousePos.y-2==piecePos.y){ //going down, already know we are going left
                                        if(squares[mousePos.y-1][mousePos.x-1].piecePresent()&&squares[mousePos.y-1][mousePos.x-1].color()!=turn){
                                            landed=true;
                                            for (unsigned int i=0; i<pieces.size();i++){
                                                if(pieces[i].pos().y==mousePos.y-1&&pieces[i].pos().x==mousePos.x-1){
                                                    jumpedPos=pieces[i].pos();
                                                    jumpedIndex=i;
                                                    break;
                                                }
                                            }
                                        }
                                    }else if(mousePos.y+2==piecePos.y){
                                        if(squares[mousePos.y+1][mousePos.x-1].piecePresent()&&squares[mousePos.y+1][mousePos.x-1].color()!=turn){
                                            landed=true;
                                            for (unsigned int i=0; i<pieces.size();i++){
                                                if(pieces[i].pos().y==mousePos.y+1&&pieces[i].pos().x==mousePos.x-1){
                                                    jumpedPos=pieces[i].pos();
                                                    jumpedIndex=i;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    //for red
                                }else{
                                    if(squares[mousePos.y+1][mousePos.x-1].piecePresent()&&squares[mousePos.y+1][mousePos.x-1].color()!=turn){
                                        if(mousePos.y+2==piecePos.y){//needed?
                                            landed=true;
                                            for (unsigned int i=0; i<pieces.size();i++){
                                                if(pieces[i].pos().y==mousePos.y+1&&pieces[i].pos().x==mousePos.x-1){
                                                    jumpedPos=pieces[i].pos();
                                                    jumpedIndex=i;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }else if(mousePos.x+2==piecePos.x){
                                if(turn&&!kinged){
                                    if(squares[mousePos.y-1][mousePos.x+1].piecePresent()&&squares[mousePos.y-1][mousePos.x+1].color()!=turn){
                                        if(mousePos.y-2==piecePos.y){//needed?
                                            landed=true;
                                            for (unsigned int i=0; i<pieces.size();i++){
                                                if(pieces[i].pos().y==mousePos.y-1&&pieces[i].pos().x==mousePos.x+1){
                                                    jumpedPos=pieces[i].pos();
                                                    jumpedIndex=i;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }else if(kinged){
                                    if(mousePos.y-2==piecePos.y){ //going down, already know we are going left
                                        if(squares[mousePos.y-1][mousePos.x+1].piecePresent()&&squares[mousePos.y-1][mousePos.x+1].color()!=turn){
                                            landed=true;
                                            for (unsigned int i=0; i<pieces.size();i++){
                                                if(pieces[i].pos().y==mousePos.y-1&&pieces[i].pos().x==mousePos.x+1){
                                                    jumpedPos=pieces[i].pos();
                                                    jumpedIndex=i;
                                                    break;
                                                }
                                            }
                                        }
                                    }else if(mousePos.y+2==piecePos.y){
                                        if(squares[mousePos.y+1][mousePos.x+1].piecePresent()&&squares[mousePos.y+1][mousePos.x+1].color()!=turn){
                                            landed=true;
                                            for (unsigned int i=0; i<pieces.size();i++){
                                                if(pieces[i].pos().y==mousePos.y+1&&pieces[i].pos().x==mousePos.x+1){
                                                    jumpedPos=pieces[i].pos();
                                                    jumpedIndex=i;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                //for red team
                                }else{
                                    if(squares[mousePos.y+1][mousePos.x+1].piecePresent()&&squares[mousePos.y+1][mousePos.x+1].color()!=turn){
                                        if(mousePos.y+2==piecePos.y){//needed?
                                            landed=true;
                                            for (unsigned int i=0; i<pieces.size();i++){
                                                if(pieces[i].pos().y==mousePos.y+1&&pieces[i].pos().x==mousePos.x+1){
                                                    jumpedPos=pieces[i].pos();
                                                    cout<<mousePos.x<<" "<<mousePos.y<<endl;
                                                    cout<<i<<" "<<jumpedPos.x<<" "<<jumpedPos.y<<endl;
                                                    jumpedIndex=i;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                        //when there is no jumping, just moving a piece, makes sure it is goin to a viable spot
                        }else if(correctTeam){
                            if(mousePos.x-1==piecePos.x||mousePos.x+1==piecePos.x){
                                landed=true;
                            }
                        }
                        //You are going to a spot that you can land at
                        if(landed){ 
                            //making sure where you are landing is empty
                            if(squares[mousePos.y][mousePos.x].piecePresent()){
                                emptySquare=false;
                            }
                        }else{
                            emptySquare=false;
                        }
                        //Now know for sure that the piece can move to where it wants to go, handles moving the piece and updating squares and kinged ect.
                        if (emptySquare){ //now know what the index of the moving piece is o use that to not use for loop, come back and double check
                            for (unsigned int i=0; i<pieces.size();i++){
                                //cout<<i<<" "<<piecePos.x<<" "<<piecePos.y<<endl;
                                if(pieces[i].pos().x==piecePos.x&&pieces[i].pos().y==piecePos.y){
                                    pieces[i].setPos(mousePos);
                                    if (pieces[i].color()==true && pieces[i].pos().y==7){
                                        if(!pieces[i].kinged()){
                                            pieces[i].kingMe();
                                            blackKings++;
                                        }
                                        
                                       
                                    }else if(pieces[i].color()==false && pieces[i].pos().y==0){
                                        if(!pieces[i].kinged()){
                                            pieces[i].kingMe();
                                            redKings++;
                                        }
                                    }
                                    squares[mousePos.y][mousePos.x].movePiece();
                                    squares[mousePos.y][mousePos.x].setColor(pieces[i].color());
                                    squares[piecePos.y][piecePos.x].movePiece();

                                    //handles removing piece that has been jumped
                                    if(jumping){
                                        squares[jumpedPos.y][jumpedPos.x].movePiece();
                                        if(pieces[jumpedIndex].color()){
                                            blackPieces--;
                                        }else if(!pieces[jumpedIndex].color()){
                                            redPieces--;
                                        }
                                        pieces.erase(pieces.begin()+jumpedIndex);
                                    }
                                    break;
                                }
                            }

                            //updating bools and ints that are keeping track of stats
                            clicks++; 
                            turn=!turn;
                            turns++;
                        }
                        
                    }

                    //reset all of the bools used to check back to default
                    kinged=false; //make sure this is needed
                    jumping=false;
                    landed=false;
                    correctTeam=false;
                    emptySquare=true;
                }
                
            }

            //checking if the game is over
            if(blackPieces==0||redPieces==0){
                finished=true;
                if (turn){
                    winner="Red";
                }else{
                    winner="Black";
                }
                window.close();
            }
        }
    }
    if(finished){
        ofstream outputFile("Stats.txt"); 
        if (outputFile.fail()){
            cout<<"Could not open fileOut, shutting down"<<endl;
            return -1;
        }
        outputFile<<"game over. The winner is "<<winner<<"! Thanks for playing"<<endl;
        outputFile<<"number of turns: "<<turns<<endl;
        outputFile<<"Pieces remaining: "<<pieces.size()<<endl;
        outputFile<<"number of black pieces kinged: "<<blackKings<<endl;
        outputFile<<"number of red pieces kinged: "<<redKings<<endl;

    }
    return 0; 
}


// function definitions


void drawRect(const Pos POS,const bool COLOR,RenderTarget& window){
    RectangleShape square;
    square.setSize(Vector2f(100,100));
    square.setPosition(POS.x,POS.y);
    if(COLOR){
        square.setFillColor(Color(120,60,0));
    }else{
        square.setFillColor(Color(255,255,255));
    }
    window.draw(square);//window.draw(square);
}

void drawPiece(Piece& piece,RenderTarget& window){ //maybe just pass a piece object which has all of that info
    CircleShape drawnPiece;
    int x,y;
    drawnPiece.setRadius(40);
    drawnPiece.setOrigin(40,40);
    x=piece.pos().x*100+50;
    y=piece.pos().y*100+50;
    drawnPiece.setPosition(x,y);
    if(piece.color()){
        drawnPiece.setFillColor(Color(0,0,0));
    }else{
        drawnPiece.setFillColor(Color(255,0,0));
    } 
    window.draw(drawnPiece);
    if(piece.kinged()){
        drawCrown(piece.pos(),window); 
    }
}
void drawCrown(const Pos POS,RenderTarget& window){ //fix so that it can be filled in
    ConvexShape prong1,prong2,prong3;
    RectangleShape base;
    prong1.setPointCount(3);
    prong1.setPoint(0,Vector2f(0, 0));
    prong1.setPoint(1,Vector2f(25,50));
    prong1.setPoint(2,Vector2f(0,50));
    prong2.setPointCount(3);
    prong2.setPoint(0,Vector2f(25,50));
    prong2.setPoint(1,Vector2f(50,0));
    prong2.setPoint(2,Vector2f(75,50));
    prong3.setPointCount(3);
    prong3.setPoint(0,Vector2f(75,50));
    prong3.setPoint(1,Vector2f(100,0));
    prong3.setPoint(2,Vector2f(100,50));
    base.setSize(Vector2f(50,5));
    
    prong1.setFillColor(Color::Yellow);
    prong2.setFillColor(Color::Yellow);
    prong3.setFillColor(Color::Yellow);
    base.setFillColor(Color::Yellow);
    prong1.setScale(Vector2f(.5,.5));
    prong2.setScale(Vector2f(.5,.5));
    prong3.setScale(Vector2f(.5,.5));
    int x,y;
    x=POS.x*100+25;
    y=POS.y*100+30;
    prong1.setPosition(x,y); //take coord and do lots to it,
    prong2.setPosition(x,y);
    prong3.setPosition(x,y);
    base.setPosition(x,y+25);
    window.draw(prong1);
    window.draw(prong2);
    window.draw(prong3);
    window.draw(base);
}

Pos convertPos(const Vector2i REALPOS){
    Pos changedPos;
    changedPos.x=REALPOS.x/100;
    changedPos.y=REALPOS.y/100;
    return changedPos;
}