using namespace std;
//PIECE VALUES
const int pawn   =  100;
const int bishop =  325;
const int knight =  320;
const int rook   =  500;
const int queen  =  975;
const int king   = 32767;
/*-----------------------------------------------*/
const bool white = true ;
const bool black = false;
const int w = 0 ;
const int b = 1;
const int none = 2;
/*-----------------------------------------------*/
int bx[64];
const int startup[64] = { rook, knight, bishop, queen, king, bishop, knight, rook, pawn, pawn,pawn,pawn,pawn,pawn,pawn, pawn, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -pawn, -pawn, -pawn, -pawn, -pawn, -pawn, -pawn, -pawn, -rook, -knight, -bishop, -queen, -king, -bishop, -knight, -rook};
/*------------------------------------------------*/

//Pawns are encouraged to stay in the center and advance forward
short PawnTable[64] =
{
  0,  0,  0,  0,  0,  0,  0,  0,
 50, 50, 50, 50, 50, 50, 50, 50,
 10, 10, 20, 30, 30, 20, 10, 10,
  5,  5, 10, 27, 27, 10,  5,  5,
  0,  0,  0, 25, 25,  0,  0,  0,
  5, -5,-10,  0,  0,-10, -5,  5,
  5, 10, 10,-25,-25, 10, 10,  5,
  0,  0,  0,  0,  0,  0,  0,  0
};

//Knights are encouraged to control the center and stay away from edges to increase mobility
short KnightTable[64] =
{
 -50,-40,-30,-30,-30,-30,-40,-50,
 -40,-20,  0,  0,  0,  0,-20,-40,
 -30,  0, 10, 15, 15, 10,  0,-30,
 -30,  5, 15, 20, 20, 15,  5,-30,
 -30,  0, 15, 20, 20, 15,  0,-30,
 -30,  5, 10, 15, 15, 10,  5,-30,
 -40,-20,  0,  5,  5,  0,-20,-40,
 -50,-40,-20,-30,-30,-20,-40,-50,
};

//Bishops are also encouraged to control the center and stay away from edges and corners
short BishopTable[64] =
{
 -20,-10,-10,-10,-10,-10,-10,-20,
 -10,  0,  0,  0,  0,  0,  0,-10,
 -10,  0,  5, 10, 10,  5,  0,-10,
 -10,  5,  5, 10, 10,  5,  5,-10,
 -10,  0, 10, 10, 10, 10,  0,-10,
 -10, 10, 10, 10, 10, 10, 10,-10,
 -10,  5,  0,  0,  0,  0,  5,-10,
 -20,-10,-40,-10,-10,-40,-10,-20,
};

//Kings have 2 piece square tables, one for the end game and one for the middle game.  
//During the middle game kings are encouraged to stay in the corners, while in the end game kings are encouraged to move towards the center
short KingTable[64] =
{
  -30, -40, -40, -50, -50, -40, -40, -30,
  -30, -40, -40, -50, -50, -40, -40, -30,
  -30, -40, -40, -50, -50, -40, -40, -30,
  -30, -40, -40, -50, -50, -40, -40, -30,
  -20, -30, -30, -40, -40, -30, -30, -20,
  -10, -20, -20, -20, -20, -20, -20, -10,
   20,  20,   0,   0,   0,   0,  20,  20,
   20,  30,  10,   0,   0,  10,  30,  20
};

short KingTableEndGame[64] =
{
 -50,-40,-30,-20,-20,-30,-40,-50,
 -30,-20,-10,  0,  0,-10,-20,-30,
 -30,-10, 20, 30, 30, 20,-10,-30,
 -30,-10, 30, 40, 40, 30,-10,-30,
 -30,-10, 30, 40, 40, 30,-10,-30,
 -30,-10, 20, 30, 30, 20,-10,-30,
 -30,-30,  0,  0,  0,  0,-30,-30,
 -50,-30,-30,-30,-30,-30,-30,-50
};
/*---------------------------------------------------*/
class Board   //Class Board begins here
{
	public:
		int Score;
		bool BlackCheck;
		bool BlackMate;
		bool WhiteCheck;
		bool WhiteMate;
		bool StaleMate;
		bool EndGamePhase;
		bool EndGame;
        bool InsufficientMaterial;
		bool WhoseMove;//It will be true if white is to move
		int square[64];
        //Constructor
		Board()
		{
			Score=0;
			BlackCheck=BlackMate=WhiteCheck=WhiteMate=StaleMate=false;
			EndGamePhase=false;EndGame=false;InsufficientMaterial=false;
			WhoseMove=white;
			for(int k=0;k<64;k++)
			{
				square[k]=startup[k];
			}
		};
		//Prints The Board
		void printboard()
		{
			int a,b;
			string piece;
			cout<<endl<<"\t a  b  c  d  e  f  g  h\n\n";
			for (a = 0; a<8; a++)
			{
				cout << endl;
				cout<<a+1<<"\t";
				for (b = 0; b < 8; b++)
				{
					switch (square[(a*8)+b])
					{
					case 0:
						piece = "-";
						break;
					case pawn:
						piece = "P";
						break;
					case knight:
						piece = "N";
						break;
					case bishop:
						piece = "B";
						break;
					case rook:
						piece = "R";
						break;
					case queen:
						piece = "Q";
						break;
					case king:
						piece = "K";
						break;
					case -pawn:
						piece = "p";
						break;
					case -knight:
						piece = "n";
						break;
					case -bishop:
						piece = "b";
						break;
					case -rook:
						piece = "r";
						break;
					case -queen:
						piece = "q";
						break;
					case -king:
						piece = "k";
						break;
					}
					cout << " " << piece << " ";
				}
			}
			cout << endl << endl;
		};
		
		//Make board
		void makeboard(int ary[64])
		{
			for(int k=0;k<64;k++)
			{
				square[k]=ary[k];
			}
		};
		//Make move on a given board
		void makemove(int movefrom,int moveto)
		{  
			if(checkturn(movefrom))
			{
 				if(checkvalid(movefrom,moveto))
				{
					square[moveto]=square[movefrom];
					square[movefrom]=0;
					WhoseMove=!WhoseMove;
				}
			}
		};
		//Rules For Knights Moves
		int moveknight(int rs,int cs,int rd,int cd)
		{
			int app=0;
			if((rd==rs-1)&&(cd==cs+2))
			{app=1;}
			if((rd==rs+1)&&(cd==cs+2))
			{app=1;}
			if((rd==rs-1)&&(cd==cs-2))
			{app=1;}
			if((rd==rs+1)&&(cd==cs-2))
			{app=1;}
			if((rd==rs-2)&&(cd==cs+1))
			{app=1;}
			if((rd==rs-2)&&(cd==cs-1))
			{app=1;}
			if((rd==rs+2)&&(cd==cs-1))
			{app=1;}
			if((rd==rs+2)&&(cd==cs+1))
			{app=1;}
			return app;
		};
		
		//Rules For Bishops Moves
		int movebishop(int rs,int cs,int rd,int cd)
		{
			int app=0;int ct=0;int nsib=0;int k=0;int newrow=0;int newcol=0;
			if((abs(rd-rs)==abs(cd-cs)))   //ensures diagonal mvmt
			{
				if((rs<rd)&&(cs<cd))      //topdownleftright
				{
					nsib=(rd-rs)-1;
					k=1;
					while(k<=nsib)     //checks if all squares in between are empty
					{
						newrow=rs+k;
						newcol=cs+k;
						if(square[(newrow*8)+newcol]==0)
						{
							app=1;
						}
						else
						{
							app=0;
							break;
						}
						k++;
					} //while ends
					if(nsib==0)
					{
						app=1;
					}
				}
				if((rs<rd)&&(cs>cd))   //topdownrightleft
				{
					nsib=(rd-rs)-1;
					k=1;
					while(k<=nsib)     //checks if all squares in between are empty
					{
						newrow=rs+k;
						newcol=cs-k;
						if(square[(newrow*8)+newcol]==0)
						{
							app=1;
						}
						else
						{
							app=0;
							break;
						}
						k++;
					}//while ends
					if(nsib==0)
					{
						app=1;
					}
				}
				if((rs>rd)&&(cs<cd)) //downupleftright
				{
					nsib=(rs-rd)-1;
					k=1;
					while(k<=nsib)     //checks if all squares in between are empty
					{
						newrow=rs-k;
						newcol=cs+k;
						if(square[(newrow*8)+newcol]==0)
						{
							app=1;
						}
						else
						{
							app=0;
							break;
						}
						k++;
					}//while ends
					if(nsib==0)
					{
						app=1;
					}
				}
				if((rs>rd)&&(cs>cd))   //smaller rows
				{
					nsib=(rs-rd)-1;
					k=1;
					while(k<=nsib)     //checks if all squares in between are empty
					{
						newrow=rs-k;
						newcol=cs-k;
						if(square[(newrow*8)+newcol]==0)
						{
							app=1;
						}
						else
						{
							app=0;
							break;
						}
						k++;
					}//while  ends
					if(nsib==0)
					{
						app=1;
					}
				}
			}		//ensures diagonal mvmt
			return app;
		};
		
		//Rules For Rooks Moves
		int moverook(int rs,int cs,int rd,int cd)
		{
			int app=0;int t1=0,t2=0;
			if((((rd==(rs+1))||(rd==(rs-1)))&&(cd==cs))||(((cd==(cs-1))||(cd==(cs+1)))&&(rd==rs)))
			{
				app=1;
			}
			if((rs==rd)&&(cs!=cd))
			{
				if(cd>cs)
				{
					t1=cs;
					t2=cd;
				}
				else
				{
					t1=cd;
					t2=cs;
				}
				for(int i=t2-1;i>t1;i--)
				{
					if(square[(rs*8)+i]!=0)
					{
						app=0;
						break;
					}
					else
					{
						app=1;
					}
				}
			}
			if((rs!=rd)&&(cs==cd))
			{
				if(rd>rs)
				{
					t1=rs;
					t2=rd;
				}
				else
				{
					t1=rd;
					t2=rs;
				}
				for(int i=t2-1;i>t1;i--)
				{
					if(square[(i*8)+cs]!=0)
					{
						app=0;
						break;
					}
					else
					{
						app=1;
					}
				}
			}
			return app;
		};
		
		
		//check validity of our move
		int checkvalid(int movefrom,int moveto)
		{
			int app=0;
			int rs=0,cs=0,rd=0,cd=0;
			rs=(int)(movefrom/8);
			cs=(int)(movefrom%8);
			rd=(int)(moveto/8);
			cd=(int)(moveto%8);
			if((square[movefrom]<0&&square[moveto]<0)||(square[movefrom]>0&&square[moveto]>0))
				return 0;
			switch (square[movefrom])
			{
			case pawn:
				if(square[moveto]==0)
				{
					if( (rd==(rs+1))&&(cd==cs))
						app=1;
					if((rs==1)&&(rd==(rs+2))&&(cd==cs))
						app=1;
				}
				else
				{
					if((rd==rs+1)&&((cd==cs+1)||(cd==cs-1)))
						return 1;
					else
						return 0;
				}
				break;
			case -pawn:
				if(square[moveto]==0)
				{
					if( (rd==(rs-1))&&(cd==cs))
						app=1;
					if((rs==6)&&(rd==(rs-2))&&(cd==cs))
						app=1;
				}
				else
				{
					if((rd==rs-1)&&((cd==cs+1)||(cd==cs-1)))
						return 1;
					else
						return 0;
				}
				break;
			case -rook:
			case  rook:
					app=moverook(rs,cs,rd,cd);
					break;
			case -knight:
			case  knight:
					app=moveknight(rs,cs,rd,cd);
					break;
			case -king:
			case king:
					if((((rd==(rs+1))||(rd==(rs-1))) &&(cd==cs))||(((cd==(cs-1))||(cd==(cs+1)))&&(rd==rs)))
						app=1;
					if(((rd==rs+1)&&((cd==cs-1)||(cd==cs+1)))||((rd==rs-1)&&((cd==cs+1)||(cd==cs-1))))
						app=1;
					break;
			case -bishop:
			case bishop:
					app=movebishop(rs,cs,rd,cd);
					break;
			case -queen:
			case queen:
					app=(movebishop(rs,cs,rd,cd)||moverook(rs,cs,rd,cd));
					break;
			}
			return app;
		};
		
		
		//check turnwise if our move is legal
		int checkturn(int movefrom)
		{	
			if((WhoseMove==white)&&(square[movefrom]<0))
			return 1;
			else if((WhoseMove==black)&&(square[movefrom]>0))
				return 1;
			else
				return 0;
		};
		//Search for Black mate or Black check
		void findBlackMateOrCheck()
		{	int k;	
			int mfrom=0,mto=0;
			int bkdanger=0;				
			for(k = 0; k < 64; k++)
			{		if(square[k]==-king)
					break;  //Find the Black King
			}
			if(k==64)	
			{	BlackMate=true;	EndGame=true;	return;	}
			mto=k;
			for (k = 0; k < 64; k++)
			{
				if(square[k]>0)
				{
					mfrom=k;
					if(checkvalid(mfrom,mto))
					{
						bkdanger++;
					}
				}
			}
			if(bkdanger==1)
			{
				//cout<<"Black king is in check";
			    BlackCheck=true;
			}
			if(bkdanger>1)
			{
				//cout<<"Black king is checkmated";
				BlackMate=true;EndGame=true;
			}
		};

		//Search for White mate or White check
		void findWhiteMateOrCheck()
		{	int k;
			int mfrom=0,mto=0;	
			int wkdanger=0;		
			for (k = 0; k < 64; k++)
			{
				if(square[k]==king)
					break;  //Find the White King
			}
			if(k==64)	
			{	WhiteMate=true;	EndGame=true;	return;	}
			mto=k;
			for (k = 0; k < 64; k++)
			{
				if(square[k]<0)
				{
					mfrom=k;
					if(checkvalid(mfrom,mto))
					{
						wkdanger++;
					}
				}
			}
			if(wkdanger==1)
			{
				//cout<<"White king is in check";
			    WhiteCheck=true;
			}
			if(wkdanger>1)
			{
				//cout<<"White king is checkmated";
				WhiteMate=true;EndGame=true;
			}
		};


		bool get_EndGame()	{	return EndGame;	};
		bool get_BlackMate()	{	return BlackMate;	};
		bool get_WhiteMate()	{	return WhiteMate;	};


};   //Class Board ends here
