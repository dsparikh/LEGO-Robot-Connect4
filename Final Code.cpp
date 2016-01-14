/*******************************************************************
Names:    Nisarg Bhavsar          ID Numbers : 20624568   
          Jorren Meeussen					   20631571
          Dhrumilkumar Parikh				   20612453	
          Geoff Spielman					   20606326	                     

Final Project Source Code


Date:   November 24, 2015

*******************************************************************/

#include "NXTServo-lib-UW.c"

const int MAXROW = 4;
const int MAXCOL = 7;
const int redVal = 300;
const int stabilize = 800; //time it takes the robot to stop shaking
int GameBoard[MAXROW][MAXCOL] = {
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0} };


void moveHor(int currCol,int disCol);//By: Dhrumilkumar Parikh
int moveVert(int currRow,int disRow);//By: Dhrumilkumar Parikh
int P_moveUpdate(int *emptiesToCheck);//By: Jorren Meeussen
void updateEmpties(int *recEmptiesArray);//By: Geoff Spielman 
void robotPlace(int *recEmptiesArray, int recCurPos);//By: Geoff Spielman 
bool finishHorz();//By: Nisarg Bhavsar
bool finishVerti();//By: Nisarg Bhavsar
bool finishDia();//By: Nisarg Bhavsar
bool isFinish ();//By: Nisarg Bhavsar
void displayGameBoard();//By: Geoff Spielman 
void displayEmpties(int *recEmptiesArray);//By: Geoff Spielman 
void dropPiece();//By: Nisarg Bhavsar
int Strategy();//Group 

task main()
{
	int EmptiesArray[MAXCOL] = {3, 3, 3, 3, 3, 3, 3};
	SensorType[S1] = sensorI2CCustom9V;
	SensorType[S2] = sensorColorNxtRED;
	SensorType[S3] = sensorTouch; //used for recalibration
	SensorType[S4] = sensorSONAR;
	
	
	while(SensorValue(S4) == 255){}
	playSound(soundDownwardTones);
	time1[T1] = 0;

	int curPos = -2;

	motor[motorA] = 10;
	motor[motorB] = 10;
	while(SensorValue[S3] != 1)
	{}
	motor[motorA] = 0;
	motor[motorB] = 0;

	displayGameBoard();
	do
	{
		updateEmpties(EmptiesArray);
		robotPlace(EmptiesArray, curPos);
		if (!isFinish())
		{
			clearTimer(T1);
			while (time1[T1]<5000)
			{
				if (time1[T1]%1000==0)
				{
					playTone (650,50);
					wait1Msec(800);
					clearSounds();
				}	
			}
			playTone (850, 50);
			curPos = P_moveUpdate(EmptiesArray);
		}
	} while(!isFinish());

	playTone(695, 14); while(bSoundActive);
	playTone(695, 14); while(bSoundActive);
	playTone(695, 14); while(bSoundActive);
	playTone(929, 83); while(bSoundActive);
	playTone(1401, 83); while(bSoundActive);
	playTone(1251, 14); while(bSoundActive);
	playTone(1188, 14); while(bSoundActive);
	playTone(1054, 14); while(bSoundActive);
	playTone(1841, 83); while(bSoundActive);
	playTone(1401, 41); while(bSoundActive);
	playTone(1251, 14); while(bSoundActive);
	playTone(1188, 14); while(bSoundActive);
	playTone(1054, 14); while(bSoundActive);
	playTone(1841, 83); while(bSoundActive);
	playTone(1401, 41); while(bSoundActive);
	playTone(1251, 14); while(bSoundActive);
	playTone(1188, 14); while(bSoundActive);
	playTone(1251, 14); while(bSoundActive);
	playTone(1054, 55); while(bSoundActive);
	wait1Msec(280);
	playTone(695, 14); while(bSoundActive);
	playTone(695, 14); while(bSoundActive);
	playTone(695, 14); while(bSoundActive);
	playTone(929, 83); while(bSoundActive);
	playTone(1401, 83); while(bSoundActive);
	playTone(1251, 14); while(bSoundActive);
	playTone(1188, 14); while(bSoundActive);
	playTone(1054, 14); while(bSoundActive);
	playTone(1841, 83); while(bSoundActive);
	playTone(1401, 41); while(bSoundActive);
	playTone(1251, 14); while(bSoundActive);
	playTone(1188, 14); while(bSoundActive);
	playTone(1054, 14); while(bSoundActive);
	playTone(1841, 83); while(bSoundActive);
	playTone(1401, 41); while(bSoundActive);
	playTone(1251, 14); while(bSoundActive);
	playTone(1188, 14); while(bSoundActive);
	playTone(1251, 14); while(bSoundActive);
	playTone(1054, 55); while(bSoundActive);//[3]
	wait1Msec(280);

}
//****************************************************************


void moveHor(int currCol,int disCol)
//Use Motor A and Motor B for Horizontal movement
{
	int const x=52;//counts from one column to another (51 is optimal)
	if (currCol<disCol)//Move up the columns or drive forward 
	{
		nMotorEncoder[motorA]=0;
		int numColMove=disCol-currCol;//Number of columns to be moved 
		motor[motorA]=-10;
		motor[motorB]=-10;
		while(nMotorEncoder[motorA] > -1*numColMove*x)
		{}
		motor[motorA]=0;
		motor[motorB]=0;
  }
  else if (currCol>disCol)//Move down the columns or drive backwards
	{
  		nMotorEncoder[motorA]=0;
 		int numColMove=currCol-disCol;//Number of columns to be moved 
		motor[motorA]=10;
		motor[motorB]=10;
		while(nMotorEncoder[motorA]<numColMove*x)
		{}
		motor[motorA]=0;
		motor[motorB]=0;
	}
}

int moveVert(int currRow,int disRow)
//Use Motor C for Vertical movement
{
	float const x = 3320;//counts from one row to another (3320 is optimal)
	if (currRow<disRow)//Move down
	{
		nMotorEncoder[motorC] = 0;
		int numRowMove=disRow-currRow;
		motor[motorC]=-95;
		while(nMotorEncoder[motorC]>-1*numRowMove*x)
		{}
		motor[motorC]=0;
  }
  if (currRow>disRow)//Move up
	{
  	nMotorEncoder[motorC]=0;
 		int numRowMove=currRow-disRow;
		motor[motorC]=95;
		while(nMotorEncoder[motorC]<numRowMove*x)
		{}
		motor[motorC]=0;
	}
	return SensorValue[S2];
}

int P_moveUpdate(int *emptiesToCheck)
{
	//Assumed home position is at 3,-2
	int curCol = -2;
	int curRow = 3;
	//moves to each column
	for(int i = 0; i < MAXCOL; i ++)
	{
		moveHor(curCol, i);
		curCol = i;
		wait1Msec(stabilize);

	//if it is necessary to check for a piece in this column
		if (emptiesToCheck[i] >= 0)
		{
				moveVert(curRow, emptiesToCheck[i]);
				curRow = emptiesToCheck[i];

			displayString(MAXROW+2, "check Row: %i ", curRow);
			displayString(MAXROW+3, "check Col: %i ", curCol);
		if (SensorRaw(S2) > redVal)
			{
				GameBoard[curRow][curCol] = 1; 
				//updates gameboard that a piece is there
				displayGameBoard();
				updateEmpties(emptiesToCheck);
				displayEmpties(emptiesToCheck);
				playSound(soundBeepBeep); 
				moveVert(curRow, 3); //resets light sensor to bottom
				return curCol;
			}
		}
	}
	moveVert(curRow, 3); //resets light sensor to bottom
	return curCol; //in theory, this will never be required, just for compiling
}


void updateEmpties(int *recEmptiesArray)
{
	//go across columns
	for (int col = 0; col < MAXCOL; col ++)
	{
		//sets entry to -1 incase the column is full
		recEmptiesArray[col] = -1;
		//go down the columns looking for non-zero entries, bottom one is saved
		for (int row = 0; row < MAXROW; row ++)
		{
			if (GameBoard[row][col] == 0)
				recEmptiesArray[col] = row;
		}
	}
}

void robotPlace(int *recEmptiesArray, int recCurPos)
{
	int ChosenColIndex=0;
	//determines which columns are possible (not full) based on empites array
	int PossibleIndices[MAXCOL] = {0, 0, 0, 0, 0, 0, 0};
	int count = 0;
	
	ChosenColIndex = Strategy();
	
	if (ChosenColIndex == -1) 
	{
		for (int col = 0; col < MAXCOL; col ++)
		{
			if (recEmptiesArray[col] >= 0)
			{
				PossibleIndices[count] = col;
				count += 1;
			}
		}
		count -= 1; 
		/*corrects for count going one too far
		at this point, Possible indices contains the column indices of 
		non-full columns and count contains how many not full columns there are
		*/
		ChosenColIndex = PossibleIndices[random[count]];
		// random number between 0 and count
		displayString(4, "ChosenCol: %d", ChosenColIndex);
	}

	//now place piece in the selected column
	GameBoard[recEmptiesArray[ChosenColIndex]][ChosenColIndex] = 2;
	updateEmpties(recEmptiesArray);
	displayEmpties(recEmptiesArray);
	displayGameBoard();
	moveHor(recCurPos, ChosenColIndex);
	//wait to give robot a change to stabilize
	wait1Msec(stabilize);
	dropPiece();
}

bool finishHorz()
{
	bool finishHor = false;
	//Check for horizontal win
	for (int row = MAXROW - 1; row >= 0; row --)
	{
		for (int baseCol = 0; baseCol < MAXCOL - 3; baseCol ++)
		{
				if (GameBoard[row][baseCol] != 0 && (GameBoard[row][baseCol] 
					== GameBoard[row][baseCol + 1]) 
					&& (GameBoard[row][baseCol + 1] 
					== GameBoard[row][baseCol + 2]) 
					&& (GameBoard[row][baseCol+2] 
					== GameBoard[row][baseCol + 3]))
				{
					finishHor = true;
				}
		}//ends basecol for
	}//ends row for
	//displayString(4, "k: %d  i: %d j: %d", k, i, j);
	return finishHor;
}

bool finishVerti()
{
	bool finishVert = false;
	//Check for vertical win
	for (int a=0; a<MAXCOL && !finishVert; a++)
	{
		for (int b=0; b<MAXROW; b++)
		{
			if (GameBoard[b][a]== GameBoard [(1+b)%4][a]&& GameBoard[b][a]!=0 
				&& GameBoard [(1+b)%4][a]!=0)
				finishVert = true;
			else
			{
				finishVert = false;
				b=MAXROW;
			}
		}
	}
	return finishVert;
}


bool finishDia()
{
	bool finishDiagonal = false;
	//Check for diagonal win
	for (int c=0; c< MAXCOL - 3 && !finishDiagonal;c++)
	{
		if (GameBoard[MAXROW - 1][c] != 0 && (GameBoard[MAXROW - 1][c] == 
			GameBoard[MAXROW - 2][c+1]) && (GameBoard[MAXROW - 2][c + 1] == 
			GameBoard[MAXROW - 3][c+2]) && (GameBoard[MAXROW - 3][c + 2] == 
			GameBoard[MAXROW - 4][c + 3]))
			
			finishDiagonal = true;
		else if (GameBoard[MAXROW - 4][c] != 0 && (GameBoard[MAXROW - 4][c] ==
				 GameBoard[MAXROW - 3][c+1]) && 
				(GameBoard[MAXROW - 3][c+1] == GameBoard[MAXROW - 2][c+2]) && 
				(GameBoard[MAXROW - 2][c+2] == GameBoard[MAXROW - 1][c+3]))
				
			finishDiagonal = true;
	}
	return finishDiagonal;
}

bool isFinish ()
{
	bool a = finishHorz();
	bool b = finishVerti();
	bool c = finishDia();
	displayString(4, "a: %d, b: %d. c: %d", a, b, c);
	wait1Msec(2000);
	return (a||b||c);
}

void displayGameBoard()
{
	for (int row = 0; row < MAXROW; row ++)
	{
		//must be hardcoded due to displaystring overwriting each line
		displayString(row, "%d %d %d %d %d %d %d", GameBoard[row][0],
		GameBoard[row][1], GameBoard[row][2], GameBoard[row][3],
		GameBoard[row][4], GameBoard[row][5], GameBoard[row][6]);
	}
}

void displayEmpties(int *recEmptiesArray)
{
	//must be hardcoded due to displaystring overwriting each line
	displayString(MAXROW + 1, "E: %d %d %d %d %d %d %d", recEmptiesArray[0], 
				  recEmptiesArray[1], recEmptiesArray[2], recEmptiesArray[3], 
				  recEmptiesArray[4], recEmptiesArray[5], recEmptiesArray[6]);
}

void dropPiece ()
{
	playSound(soundDownwardTones);
	setGripperPosition(S1, 5, 36);
	wait1Msec(93);
	setGripperPosition(S1, 5, 20);
	wait1Msec(stabilize);
	
	motor[motorA]=10;
	motor[motorB]=10;
	while(SensorValue[S3] == 0)
	{}
	motor[motorA] = 0;
	motor[motorB] = 0;
}

int Strategy()
{
	int Tacticol = -1;
	
	for (int row = MAXROW - 1; row >= 0; row --)
	{
		for (int baseCol = 0; baseCol < MAXCOL - 3; baseCol ++)
		{
			if (row == 3)
			{
				if (GameBoard[row][baseCol] != 0 && (GameBoard[row][baseCol] == 
					GameBoard[row][baseCol + 1]) && (GameBoard[row][baseCol + 1]
					== GameBoard[row][baseCol + 2]))
				{
					if (GameBoard[row][baseCol+ 3] == 0 && (baseCol + 3 < 7))
						return baseCol + 3;
					else if ((baseCol - 1 >= 0) && 
							GameBoard[row][baseCol - 1] == 0)
						return baseCol - 1;
				}
			}
			else //row not 3
			{
				if (GameBoard[row][baseCol] != 0 && (GameBoard[row][baseCol] == 
					GameBoard[row][baseCol + 1]) && (GameBoard[row][baseCol + 1]
					 == GameBoard[row][baseCol + 2]))
				{
					if (GameBoard[row][baseCol+ 3] == 0 && 
						GameBoard[row + 1][baseCol+ 3] != 0 && 
						(baseCol + 3 < 7))
						return baseCol + 3;
					else if ((baseCol - 1 >= 0) && GameBoard[row][baseCol - 1]
							 == 0 && GameBoard[row + 1][baseCol - 1] != 0)
						return baseCol - 1;
				}
				
			}//ends else
		}//ends basecol for
	}//ends row for
	
	for (int col = 0; col < MAXCOL; col ++)
	{
			if (GameBoard[MAXROW - 1][col]!=0 && GameBoard[MAXROW - 1][col]==
				GameBoard[MAXROW -2][col] && GameBoard[MAXROW - 2][col]==
				GameBoard[MAXROW - 3][col] && GameBoard[MAXROW - 4][col] == 0)
				//Vert check
			{
				return col;
			}
	}
	return Tacticol; //if not strategic move
}


/*********************************************************************************************************************
  Copyright(c) 2015 Nisarg Bhavsar,Jorren Meeussen,Dhrumilkumar Parikh,Geoff Spielman.  Students, staff, and faculty
  members at the University of Waterloo are granted a non-exclusive right
  to copy, modify, or use this software for non-commercial teaching,
  learning, and research purposes provided the author(s) are acknowledged
  except for as noted below.
***********************************************************************************************************************/		



