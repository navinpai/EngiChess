#include<iostream>
#include<string>
#include<stdlib.h>
#include<iomanip>
#include<windows.h>
#include<math.h>
#include<conio.h>
#include "board.h"
#include "eval.h"
#include "gametree.h"
#define GAMEDEPTH 2

int main (void)
{
	Board obj1;
	int move=0;
	Evaluation obj2;
	string passd;
	bool turn=1;
	cout<<"\n\nWelcome To EngiChess\n\nEnter Moves in string fashion Eg <a1b7>\nEnter x to Exit.\n\n";
	obj1.printboard();
	int ct=0;
	while(obj1.EndGame==false&& obj1.get_EndGame()==false)
	{			if(turn)
				{	/*obj1=nextmove(obj1,GAMEDEPTH);
							obj1.printboard();
							//_getch();
							turn=0;
							ct++;
							*/
                    cout<<endl<<"Enter Move:";
					cin>>passd;
					if (passd.substr(0, 1) >= "a" && passd.substr(0, 1) <= "h" && passd.substr(1, 1) >= "1" && passd.substr(1, 1) <= "8" && passd.substr(2, 1) >= "a" && passd.substr(2, 1) <= "h" && passd.substr(3, 1) >= "1" && passd.substr(3, 1) <= "8")
					{	int a,b,c,d;
						a = passd[0] - 'a';
						b = passd[1] - '1';
						c = passd[2] - 'a';
						d = passd[3] - '1';

						if(obj1.checkvalid((b*8)+a,(d*8)+c)==0)
						{	cout<<"This move is Illegal"<<endl;
							obj1.printboard();
						}
						else
						{	obj1.makemove((b*8)+a,(d*8)+c);
							cout<<"\nScore:"<<obj2.EvaluateBoardScore(obj1);
							obj1.printboard();
							getch();
							turn=0;
							ct++;
						}
					}
					if(passd.substr(0,1)=="x")
						break;
				}
				if(!turn)
				{		obj1=nextmove(obj1,GAMEDEPTH);
						cout<<"\nScore:"<<obj2.EvaluateBoardScore(obj1);
						obj1.printboard();
						move=move+2;
						//cout<<"\nMove: "<<move;
						turn=1;
						ct++;
				}
				obj1.findBlackMateOrCheck();
				obj1.findWhiteMateOrCheck();
	}



}

