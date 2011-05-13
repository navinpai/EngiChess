class Evaluation  //Class Evaluation begins here
{
public:	
	//FUNCTION one
	int EvaluateBoardScore(Board board1)
	{
		//&&&&&&&&&&&&&&&  Declarations
		board1.Score = 0;
		bool insufficientMaterial = true;
        //&&&&&&&&&&&&&&&  Board conditions
		board1.findBlackMateOrCheck();
        board1.findWhiteMateOrCheck();

		if (board1.StaleMate)
		{	
			return board1.Score;
		}
		if (board1.BlackMate==true)
		{
			board1.Score = 32767;
			return board1.Score;
		}
		if (board1.WhiteMate==true)
		{
				board1.Score = -32767;
				return board1.Score;
		}	
		if (board1.BlackCheck==true)
		{
			board1.Score += 75;
			if (board1.EndGamePhase==true)
				board1.Score += 10;
		}
		if (board1.WhiteCheck==true)
		{
			board1.Score -= 75;
			if (board1.EndGamePhase==true)
				board1.Score -= 10;
		}			
		if (board1.WhoseMove==white)
		{
			board1.Score += 10;//Add a small bonus for tempo (turn)
		}
		else
		{
			board1.Score -= 10;//Add a small bonus for tempo (turn)
		}
		//&&&&&&&&&&&&&&&   Further Declarations inside FUNCTION one
		int blackBishopCount=0;
		int whiteBishopCount=0;
		int knightCount=0;
		int remainingPieces=0;
		int s1=0,s2=0;
		//&&&&&&&&&&&&&&&      LOOP STARTS HERE
		//system("cls");
		//cout<<endl<<"board1.Score1:"<<board1.Score<<endl;
		//getch();
		for (int x = 0; x < 64; x++)
		{
			//cout<<endl<<"x:"<<x<<endl;
			//getch();
			if (board1.square[x]==0)
				continue;			
			remainingPieces++;//Calculate Remaining Material for end game determination
			//cout<<endl<<"remainingPieces:"<<remainingPieces<<endl;
			//getch();
			if (board1.square[x] < 0) //i.e White Piece
			{
				s1=EvaluatePieceScore(board1,x,whiteBishopCount,insufficientMaterial);
				board1.Score += s1;
			}
			else if (board1.square[x] > 0) //i.e Black Piece
			{
				s2=EvaluatePieceScore(board1,x,blackBishopCount,insufficientMaterial);
				board1.Score -= s2;				
			}
			if ((board1.square[x] == knight)||(board1.square[x] == -knight))
			{
				knightCount++;
				if (knightCount > 1)
				{
					insufficientMaterial = false;
				}
			}
			if ((blackBishopCount + whiteBishopCount) > 1)
			{
				insufficientMaterial = false;
			}
		}
		system("cls");
		//cout<<endl<<"board1.Score2:"<<board1.Score<<endl;
		//getch();
		//cout<<endl<<"remainingPieces:"<<remainingPieces<<endl;
		//&&&&&&&&&&&&&&&LOOP ENDS HERE
		if (insufficientMaterial)
		{
			board1.Score = 0;
			board1.StaleMate = true;
			board1.InsufficientMaterial = true;
			return board1.Score;
		}
		if (remainingPieces < 10)
		{
			board1.EndGamePhase = true;
			if (board1.BlackCheck)
			{
				board1.Score += 10;
			}
			else if (board1.WhiteCheck)
			{
				board1.Score -= 10;
			}
		}
		return board1.Score;
	};	
	
	//FUNCTION two
	int EvaluatePieceScore(Board board2,int position,int &bishopCount,bool &insufficientMaterial)
	{
		int score=0;
		int attackedvalue=0,defendedvalue=0;
		int vmoves=0;
		int index = 0; 
		index=position;
		if (board2.square[position] > 0)
		{
			index = (int)(63-position); 
			//cout<<endl<<"index:"<<index<<endl;
			//getch();
		}
		score += abs(board2.square[position]);//ADDING PIECE VALUE of our piece
        //cout<<endl<<"piecevalue:"<<score<<endl;
		defendedvalue=EvalDefend(board2,position);
        //cout<<endl<<"defendedvalue:"<<defendedvalue<<endl;
		score += abs(defendedvalue);//shows how many are defending our piece:sum of their piece values
		attackedvalue=EvalAttack(board2,position);
        //cout<<endl<<"attackedvalue:"<<attackedvalue<<endl;
		//getch();
		score -= abs(attackedvalue);//shows how many pieces are attacking our piece:sum of their piece values
		//getch();
		//Double Penalty for Hanging Pieces
		if (abs(defendedvalue) < abs(attackedvalue))
		{
			score -= ((abs(attackedvalue) - abs(defendedvalue))* 10);
		}
		//cout<<endl<<"score after difference:"<<score<<endl;
		//getch();
		//shows how many valid moves are available to our piece
		vmoves=validmovecounter(board2,position);
		//cout<<endl<<"vmoves:"<<vmoves<<endl;
		//getch();
		score+=vmoves;
		//specific pawn concessions
		if ((board2.square[position] == pawn)||(board2.square[position] == -pawn))
		{
			insufficientMaterial = false;
			if (position % 8 == 0 || position % 8 == 7)
			{
				//Rook Pawns are worth 15% less because they can only attack one way
				score -= 15;		
				//cout<<endl<<"score after rook pawns:"<<score<<endl;
				//getch();
			}
			//Calculate Position Values
			score += PawnTable[index];
			//cout<<endl<<"pawntable value:"<<PawnTable[index]<<endl;		
			//cout<<endl<<"score after pawntable:"<<score<<endl;
			//getch();
		}
		else if ((board2.square[position] == knight)||(board2.square[position] == -knight))
		{        
			score += KnightTable[index];
			//cout<<endl<<"Knighttable value:"<<KnightTable[index]<<endl;		
			//cout<<endl<<"score after Knighttable:"<<score<<endl;
			//getch();
			//In the end game remove a few points for Knights since they are worth less
			if (board2.EndGamePhase)
			{
				score -= 10;				
				//cout<<endl<<"score after knights of endphase:"<<score<<endl;
				//getch();
			}
		}
		else if ((board2.square[position] == bishop)||(board2.square[position] == -bishop))
		{
			bishopCount++;
			if (bishopCount >= 2)
			{
				//2 Bishops receive a bonus
				score += 10;				
				//cout<<endl<<"score after bishops are two:"<<score<<endl;
				//getch();
			}
			//In the end game Bishops are worth more
			if (board2.EndGamePhase)
			{
				score += 10;				
				//cout<<endl<<"score after bishops of endphase:"<<score<<endl;
				//getch();
			}
			score += BishopTable[index];			
			//cout<<endl<<"BishopTable value:"<<BishopTable[index]<<endl;		
			//cout<<endl<<"score after BishopTable:"<<score<<endl;
			//getch();
		}
		else if ((board2.square[position] == rook)||(board2.square[position] == -rook))
		{
			insufficientMaterial = false;
		}
		else if ((board2.square[position] == queen)||(board2.square[position] == -queen))
		{
			insufficientMaterial = false;
		}
		else if ((board2.square[position] == king)||(board2.square[position] == -king))
		{
			if(validmovecounter(board2,position)< 2)
			{
				score -= 5;
				//cout<<endl<<"less than 2 king moves:"<<score<<endl;
				//getch();
			}
			if (board2.EndGamePhase)
			{
				score += KingTableEndGame[index];
				//cout<<endl<<"score after endphase of kings:"<<score<<endl;
				//getch();
			}
			else
			{
				score += KingTable[index];
			}
			//cout<<endl<<" KingTable value:"<<KingTable[index]<<endl;		
			//cout<<endl<<"score after  KingTable:"<<score<<endl;
			//getch();
		}
        //cout<<endl<<"score of our piece:"<<score<<endl;
		//getch();
		return score;
	};
	//FUNCTION three
	int EvalAttack(Board board3,int p)
	{
		int sum=0;
		//cout<<endl<<"i am entering EvalAttack"<<endl;
		//cout<<endl<<"defender is:"<<p<<endl;

		for (int index = 0; index < 64; index++)
		{	
			//cout<<endl<<"defender is:"<<p<<endl;
			//cout<<endl<<"defendervalue is:"<<board3.square[p]<<endl;
			//cout<<endl<<"attacker is:"<<index<<endl;
			//cout<<endl<<"attackervalue is:"<<board3.square[index]<<endl;
			//cout<<endl<<"fun:"<<board3.checkvalid(index,p)<<endl;
			//getch();
			if(board3.checkvalid(index,p))
			{
				sum+=board3.square[index];				
			}

		}
		return sum;
	};
	//FUNCTION four
	int EvalDefend(Board board4,int p)
	{
		int sum=0;
		if(board4.square[p]<0)
			board4.WhoseMove=black;
		if(board4.square[p]>0)
			board4.WhoseMove=white;

		if(EvalAttack(board4,p)==0)
			sum=0;
		else
		{
			for(int k=0;k<64;k++)
			{
				if(board4.checkvalid(k,p))
				{
					board4.makemove(k,p);
					break;
				}
			}
			sum=EvalAttack(board4,p);
		}
		return sum;
	};
	//FUNCTION five
	int validmovecounter(Board board5,int p)
	{
		int count=0;
		for(int k=0;k<64;k++)
		{
			if(board5.checkvalid(p,k))
			{
				count++;
			}
		}
		return count;
	};
};//Class Evaluation ends here
