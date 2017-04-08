#include<iostream>
#include<stdio.h> // printf and null (probably don't need)
#include<stdlib.h> //srand and rand
#include<time.h> //time

using namespace std;

void displayGameBoard(char[6][7]);
int getPlayersMove(char[6][7]);
int playAutoPiece(int, char[6][7], char);
bool checkForWinner(char[6][7],char, int);
bool checkForDanger(char[6][7], char[6][7], char, int);
bool isThereWinner(char[6][7], int, int, char, int);

bool lookUp(char[6][7], int, int, char, int, int&);
bool lookDown(char[6][7], int, int, char, int, int&);
bool lookRight(char[6][7], int, int, char, int, int&);
bool lookLeft(char[6][7], int, int, char, int, int&);
bool lookNW(char[6][7], int, int, char, int, int&);
bool lookNE(char[6][7], int, int, char, int, int&);
bool lookSW(char[6][7], int, int, char, int, int&);
bool lookSE(char[6][7], int, int, char, int, int&);

int getPossibleMoves(char[6][7],int[7], int[7]);
int randomNumberGenerator(int[], int);

int calculatePathsToDefeat(char[6][7],int[], char);
void drasticMeasures(char[6][7]);
int prediction(char[6][7], int, int[], int);
int checkForSafeMoves(char[6][7], int[7], int[7], int&);
bool searchFor(int[], int, int);
int checkPlayersMove(int[7], int, int);


void main()
{
	char gameBoard[6][7];
	bool isWinner = false; //Is there a winner yet?
	bool playerWins = false;
	bool computerWins = false;
	int safeMovesC[7];
	int safeCount;
	int computersMove;
	int playersMove;
	int possibleC[7];
	int possibleR[7];
	int possibleCount;
	int interestCol[7];
	int interestCount = 0;
	int potentialMove;

	//Populate the game board with '-'s
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 7; j++) {
			gameBoard[i][j] = '-';
		}
	}
	displayGameBoard(gameBoard);
	possibleCount = getPossibleMoves(gameBoard, possibleC, possibleR);

	char response;
	cout << "\nWould you like to go first?(y/n)? ";
	cin >> response;

	if (response == 'n' || response == 'N') {
		cout << "\nSo be it.\n";
		playAutoPiece(3, gameBoard, 'O');

		displayGameBoard(gameBoard);
	}
	else if (response == 'y' || response == 'Y') {
		cout << "\nVery well.\n";
	}
	else {
		cout << "\nInvalid input. I'll assume you want to go first.\n";
	}

	while (isWinner == false && possibleCount > 0) {
		playersMove = getPlayersMove(gameBoard); //Player selects a column
		
		if (checkForWinner(gameBoard, 'X', 4)) {
			isWinner = true;
			playerWins = true;
		}
		if (checkForWinner(gameBoard, 'O', 4)) {
			isWinner = true;
			computerWins = true;
		}

		if (!isWinner) {
			potentialMove = -8;
			interestCount = 0;
			safeCount = checkForSafeMoves(gameBoard, safeMovesC, interestCol, interestCount);

			potentialMove = prediction(gameBoard, 4, safeMovesC, safeCount); //Check to see if there are any mandatory moves (sieze or prevent immediete win)
			if (potentialMove == -8) {
				potentialMove = checkPlayersMove(interestCol, interestCount, playersMove);
				if (potentialMove == -8 || (!searchFor(safeMovesC, safeCount, potentialMove))) { //if there is no suggested move, or there is one but its not safe
					potentialMove = prediction(gameBoard, 3, safeMovesC, safeCount);
				}
			}
			
			if (potentialMove == -8) {
				
				if (safeCount != 0) {
					computersMove = playAutoPiece(randomNumberGenerator(safeMovesC, safeCount), gameBoard, 'O');
				}
				else {
					drasticMeasures(gameBoard);
				}
			}
			else {
				computersMove = playAutoPiece(potentialMove, gameBoard, 'O');
			}

			if (checkForWinner(gameBoard, 'X', 4)) {
				isWinner = true;
				playerWins = true;
			}
			if (checkForWinner(gameBoard, 'O', 4)) {
				isWinner = true;
				computerWins = true;
			}
		
			displayGameBoard(gameBoard);
			cout << "\n\nComputer chose column " << computersMove << endl;
		}
	}

	
	if (getPossibleMoves(gameBoard, possibleC, possibleR)) {
		if (playerWins) {
			displayGameBoard(gameBoard);
			cout << "\nCongratulations, you won!\n";
		}
		else if (computerWins) {
			displayGameBoard(gameBoard);
			cout << "\n\nComputer chose column " << computersMove << endl;

			cout << "\nHA! In yo FACE! I win!!!! Nice try though.\n";
		}
	}
	else {
		displayGameBoard(gameBoard);
		cout << "\nWell great. It's a draw... (this is why we can't have nice things)\n";
	}
	

	cout << endl;
	system("pause");
}

void displayGameBoard(char gameBoard[6][7]) {
	//Display the gameboard
	
	system("cls");
	cout << "   0 1 2 3 4 5 6\n\n";
	for (int i = 0; i < 6; i++) {
		cout << i << "  ";
		for (int j = 0; j < 7; j++) {
			cout << gameBoard[i][j] << " ";
		}
		cout << endl;
	}
}

int getPlayersMove(char gameBoard[6][7]) {
	int playerColumn;

	cout << "\nPlease select a column between 0 and 6: ";
	cin >> playerColumn;

	int i = 5;

	while (gameBoard[i][playerColumn] != '-' && i >= 0) {
		i--;
	}
	if (i < 0) {
		cout << "\nThat is an invalid move. Please try again.";
		playerColumn = getPlayersMove(gameBoard);
	}
	else {
		//gameBoard[i][playerColumn - 1] = 'X';
		gameBoard[i][playerColumn] = 'X';
	}

	return playerColumn;
}

int playAutoPiece(int column, char gameBoard[6][7], char piece) {
	int i = 5;

	while (gameBoard[i][column] != '-' && i >= 0) {
		i--;
	}
	if (i >= 0) { //Which, it better be.
		gameBoard[i][column] = piece;
	}

	return column;
}

int checkPlayersMove(int interestC[7], int interestCount, int playerCol) {
	if (interestC == 0) {
		return -8;
	}
	else {
		if (searchFor(interestC, interestCount, playerCol)) {
			return playerCol;
		}
		else {
			return -8;
		}
	}
}

int randomNumberGenerator(int legalColumns[], int length) {
//http://www.cplusplus.com/reference/cstdlib/srand/

	srand((unsigned int)time(NULL));
	int index = rand() % length;

	return legalColumns[index];
}

bool checkForWinner(char gameBoard[6][7], char potentialWinner, int length) {
	bool winnerExists = false;


//	int space;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 7; j++) {
			winnerExists = isThereWinner(gameBoard, i, j, potentialWinner, length);
			if (winnerExists == true) {
				return true;
			}
		}
	}
	return false;
}


bool checkForDanger(char gameBoard[6][7], char tempBoard[6][7], char potentialWinner, int length) {
	bool theoretical = false;
	
	bool gameBoardDanger = false;
	bool tempBoardDanger = false;


	//	int space;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 7; j++) {
			gameBoardDanger = isThereWinner(gameBoard, i, j, potentialWinner, length);
			tempBoardDanger = isThereWinner(tempBoard, i, j, potentialWinner, length);
			if (!gameBoard && tempBoard) {
				return true;
			}
		}
	}
	return false;
}


bool isThereWinner(char gameBoard[6][7], int row, int column, char potentialWinner, int length) {	

	int space = 0;

	if (gameBoard[row][column] != potentialWinner) {
		return false;
	}

	//Look up
	if (lookUp(gameBoard, row, column, potentialWinner, length, space)) {
		return true;
	}

	//Look down
	if (lookDown(gameBoard, row, column, potentialWinner, length, space)) {
		return true;
	}

	//Look right
	if (lookRight(gameBoard, row, column, potentialWinner, length, space)) {
		return true;
	}
	
	//Look left
	if (lookLeft(gameBoard, row, column, potentialWinner, length, space)) {
		return true;
	}

	//Look NW
	if (lookNW(gameBoard, row, column, potentialWinner, length, space)) {
		return true;
	}

	//Look NE
	if (lookNE(gameBoard, row, column, potentialWinner, length, space)) {
		return true;
	}

	//Look SW
	if (lookSW(gameBoard, row, column, potentialWinner, length, space)) {
		return true;
	}

	//Look SE
	if (lookSE(gameBoard, row, column, potentialWinner, length, space)) {
		return true;
	}

	return false;
}

bool lookUp(char gameBoard[6][7], int row, int column, char potentialWinner, int length, int &space) {
	space = 0;
	bool winner = true;
	int i = 0;



	if (row >= 3) {
		while (winner == true && i < length) {
			if (gameBoard[row][column] == potentialWinner || gameBoard[row][column] == '-') {
				space++;
			}

			if (gameBoard[row][column] != potentialWinner) {
				winner = false;
			}
			row--;
			i++;
		}
	}
	else {
		return false;
	}

	return winner;
}

bool lookDown(char gameBoard[6][7], int row, int column, char potentialWinner, int length, int &space) {
	space = 0;
	bool winner = true;
	int i = 0;


	if (row <= 2) {
		while (winner == true && i < length) {
			if (gameBoard[row][column] == potentialWinner || gameBoard[row][column] == '-') {
				space++;
			}

			if (gameBoard[row][column] != potentialWinner) {
				winner = false;
			}
			row++;
			i++;
		}
	}
	else {
		return false;
	}
	return winner;
}

bool lookLeft(char gameBoard[6][7], int row, int column, char potentialWinner, int length, int &space) {
	space = 0;
	bool winner = true;
	int i = 0;
	
	if (column >= 3) {
		while (winner == true && i < length) {
			if (gameBoard[row][column] == potentialWinner || gameBoard[row][column] == '-') {
				space++;
			}
			if (gameBoard[row][column] != potentialWinner) {
				winner = false;
			}
			column--;
			i++;
		}
	}
	else {
		return false;
	}

	return winner;
}

bool lookRight(char gameBoard[6][7], int row, int column, char potentialWinner, int length, int &space) {
	space = 0;
	bool winner = true;
	int i = 0;


	if (column <= 3) {
		while (winner == true && i < length) {
			if (gameBoard[row][column] == potentialWinner || gameBoard[row][column] == '-') {
				space++;
			}
			if (gameBoard[row][column] != potentialWinner) {
				winner = false;
			}
			column++;
			i++;
		}
	}
	else {
		return false;
	}

	return winner;
}

bool lookNW(char gameBoard[6][7], int row, int column, char potentialWinner, int length, int &space) {
	space = 0;
	bool winner = true;
	int i = 0;
	
	if (row >= 3 && column >= 3) {
		while (winner == true && i < length) {
			if (gameBoard[row][column] == potentialWinner || gameBoard[row][column] == '-') {
				space++;
			}
			if (gameBoard[row][column] != potentialWinner) {
				winner = false;
			}
			row--;
			column--;
			i++;
		}
	}
	else {
		return false;
	}

	return winner;
}

bool lookNE(char gameBoard[6][7], int row, int column, char potentialWinner, int length, int &space) {
	space = 0;
	bool winner = true;
	int i = 0;


	if (row >= 3 && column <= 3) {
		while (winner == true && i < length) {
			if (gameBoard[row][column] == potentialWinner || gameBoard[row][column] == '-') {
				space++;
			}
			if (gameBoard[row][column] != potentialWinner) {
				winner = false;
			}
			row--;
			column++;
			i++;
		}
	}
	else {
		return false;
	}

	return winner;
}

bool lookSW(char gameBoard[6][7], int row, int column, char potentialWinner, int length, int &space) {
	space = 0;
	bool winner = true;
	int i = 0;

	if (row <= 2 && column >= 3) {
		while (winner == true && i < length) {
			if (gameBoard[row][column] == potentialWinner || gameBoard[row][column] == '-') {
				space++;
			}
			if (gameBoard[row][column] != potentialWinner) {
				winner = false;
			}
			row++;
			column--;
			i++;
		}
	}
	else {
		return false;
	}

	return winner;
}

bool lookSE(char gameBoard[6][7], int row, int column, char potentialWinner, int length, int &space) {
	space = 0;
	bool winner = true;
	int i = 0;

	if (row <= 2 && column <= 3) {
		while (winner == true && i < length) {
			if (gameBoard[row][column] == potentialWinner || gameBoard[row][column] == '-') {
				space++;
			}
			if (gameBoard[row][column] != potentialWinner) {
				winner = false;
			}
			row++;
			column++;
			i++;
		}
	}
	else {
		return false;
	}

	return winner;
}

//Finds all legal moves. Returns length of parellel arrays
int getPossibleMoves(char gameBoard[6][7], int possibleCol[7], int possibleRow[7]) {

	int i = 5;
	int k = 0;

	for (int j = 0; j < 7; j++) {
		i = 5;
		while (gameBoard[i][j] != '-' && i >= 0) {
			i--;
		}
		if (i >= 0) {
			possibleRow[k] = i;
			possibleCol[k] = j;
			k++;
		}
	}

	return k;
}

//Calculates all the "dangerous" spaces the player could potentially use to win
//Also calculates all "stupid" moves
int calculatePathsToDefeat(char gameBoard[6][7], int colOfInterest[], char targetPiece) {

	int possCol[7];
	int possRow[7];
	int possibleLength = getPossibleMoves(gameBoard, possCol, possRow);
	int threatLevel[7];
	int interestCount = 0;
	char otherPiece;

	if (targetPiece == 'X') {
		otherPiece = 'O';
	}
	else {
		otherPiece = 'X';
	}

	//For each possible move, create test board
	//We already know no one can win by playing this peice.
	//Play piece on test board
	//Play player piece on test board
	//Check to see if they won. If they did it is a dangerous piece.
	//If they didn't win test your piece again.
	//Check to see if you won.
		//Check to see if the computer won. If they did it is a stupid piece.
	//Keep doing this until a "winner" is found, or there is no more room in column
	//Keep a counter of how many times it takes for a winner

	bool winnerHasComeForth;
	int row;
	for (int i = 0; i < possibleLength; i++) { //For each possible move
		
		//Create a test board
		char testingBoard[6][7];
		for (int j = 0; j < 6; j++) {
			for (int k = 0; k < 7; k++) {
				testingBoard[j][k] = gameBoard[j][k];
			}
		}

		winnerHasComeForth = false;
		row = possRow[i];
		playAutoPiece(possCol[i], testingBoard, otherPiece);
		row--;
		threatLevel[i] = 0;

		while (!winnerHasComeForth && row >= 0) { //While no winner has been found and there is still room in the column
			playAutoPiece(possCol[i], testingBoard, targetPiece);
			threatLevel[i]++;
			row--;

			winnerHasComeForth = checkForWinner(testingBoard, targetPiece, 4);
			if (winnerHasComeForth) {
				colOfInterest[interestCount] = possCol[i];
				interestCount++;
			}
			else {
				if (row >= 0) {
					playAutoPiece(possCol[i], testingBoard, otherPiece);
					row--;
					threatLevel[i]++;
					winnerHasComeForth = checkForWinner(testingBoard, otherPiece, 4);
				}
			}
		}

	}

	return interestCount;
}


int checkForSafeMoves(char gameBoard[6][7], int safeCol[7], int interestCol[7], int &interestCount) {

	//For each possible column
	//Search dangerous list
	//Seatrch stupid list
	//If it cannot be found on either it is safe, add it to the list
	//If the safe list is empty, try agian but with only the dangerous list

	int possCol[7];
	int possRow[7];
	int possCount = getPossibleMoves(gameBoard, possCol, possRow);
	int dangerCol[7];
	int dangerCount = calculatePathsToDefeat(gameBoard, dangerCol,'X');
	int stupidCol[7];
	int stupidCount = calculatePathsToDefeat(gameBoard, stupidCol, 'O');
	int safeCount = 0;

	for (int i = 0; i < possCount; i++) {

		if (!searchFor(dangerCol, dangerCount, possCol[i]) && !searchFor(stupidCol,stupidCount,possCol[i])) {
			//Add to safeList
			safeCol[safeCount] = possCol[i];
			safeCount++;
		}
		else {
			//Add to interestList
			interestCol[interestCount] = possCol[i];
			interestCount++;
		}
	}

	if (safeCount == 0) {
		for (int i = 0; i < possCount; i++) {

			if (!searchFor(dangerCol, dangerCount, possCol[i])) {
				//Add to safeList
				safeCol[safeCount] = possCol[i];
				safeCount++;
			}
		}
	}

	return safeCount;
}

//I believe this is a linear search. Not very efficent in most cases, but we are dealing with very small lists here.
bool searchFor(int list[], int length, int subject) {
	for (int i = 0; i < length; i++) {
		if (list[i] == subject) {
			return true;
		}
	}
	return false;
}

void resetArray(int arr[], int length) {
	for (int i = 0; i < length; i++) {
		arr[i];
	}
}

void drasticMeasures(char gameBoard[6][7]) {
	int possR[7];
	int possC[7];

	int length = getPossibleMoves(gameBoard,possC, possR);

	if (length == 0) { //Somehow, no possible moves.
		cout << "Somehow... there are no moves left. Must be a draw. Likley due to an error on your part.\n";
		system("pause");
	}
	else if (length == 1) {
		playAutoPiece(possC[0], gameBoard,'O');
	}
	else {
		cout << "YOU NEED TO FIX THIS" << endl;
		system("pause");
	}

}

//Checks to see if there are any nessesary moves to make (seize the win, or prevent it)
int prediction(char gameBoard[6][7], int length, int safeMoves[], int safeCount) {
	char testingBoard[6][7];

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 7; j++) {
			testingBoard[i][j] = gameBoard[i][j];
		}
	}

	int testPossR[7];
	int testPossC[7];

	int testPossCount = getPossibleMoves(testingBoard, testPossC, testPossR);
	bool didTheyWin = false;
	bool safe = true;

	//Run computer simulation
	for (int k = 0; k < testPossCount; k++) {
		//Copy over the board
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 7; j++) {
				testingBoard[i][j] = gameBoard[i][j];
			}
		}
		if (length == 4) {
			//Play theoretical piece.
			playAutoPiece(testPossC[k], testingBoard, 'O');
			//Check for winner
			didTheyWin = checkForWinner(testingBoard, 'O', length);
		}
		else {
			safe = searchFor(safeMoves, safeCount, testPossC[k]);
			//Play theoretical piece.
			playAutoPiece(testPossC[k], testingBoard, 'X');
			//Check for winner
			didTheyWin = checkForDanger(gameBoard, testingBoard, 'X', length);
		}

		//If there is a winner, return the winning column
		if (didTheyWin && safe) {
			return testPossC[k];
		}
	}

	//Run player simulation
	for (int h = 0; h < testPossCount; h++) {
		//Copy over the board
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 7; j++) {
				testingBoard[i][j] = gameBoard[i][j];
			}
		}
		
		if (length == 4) {
			//Play theoretical piece.
			playAutoPiece(testPossC[h], testingBoard, 'X');
			//Check for winner
			didTheyWin = checkForWinner(testingBoard, 'X', length);
		}
		else {
			safe = searchFor(safeMoves, safeCount, testPossC[h]);
			//Play theoretical piece.
			playAutoPiece(testPossC[h], testingBoard, 'O');
			//Check for winner
			didTheyWin = checkForDanger(gameBoard, testingBoard, 'O', length);
		}


		if (didTheyWin && safe) {
			return testPossC[h];
		}
	}
	
	return -8;
}