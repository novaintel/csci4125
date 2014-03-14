#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct board {
	int rightSum;
	int downSum;
	int cellValue;
	bool noValue;
};

struct location{
	int width;
	int height;
};

int width, height;

location Start;

bool solve(location currentLocation, vector<vector<board>> currentBoard);
vector<vector<board>> loadFile(char* fileName);

int main(int argc, char *argv[]){
	width = 0;
	height = 0;

	vector<vector<board>> currentBoard;

	if (argc <= 1)
		return -1;

	currentBoard = loadFile(argv[1]);
	solve(Start, currentBoard);

	return 0;

}

location nextEmptyLocation(location currentLocation, vector<vector<board>> currentBoard){
	location newLocation;
	int cWidth = currentLocation.width;
	int cHeight = currentLocation.height;

	if (cWidth + 1 != width){
		if (cHeight + 1 != height){
			if (currentBoard[cWidth + 1][cHeight].noValue || currentBoard[cWidth + 1][cHeight].cellValue != 0){
				newLocation.width = cWidth + 1;
				newLocation.height = cHeight;
				return newLocation;
			}
			else if (currentBoard[cWidth][cHeight + 1].noValue || currentBoard[cWidth][cHeight + 1].cellValue != 0){
				newLocation.width = cWidth;
				newLocation.height = cHeight + 1;
				return newLocation;
			}
			else if (cWidth - 1 >= 0){
				if (currentBoard[cWidth - 1][cHeight].noValue || currentBoard[cWidth - 1][cHeight].cellValue != 0){
					newLocation.width = cWidth - 1;
					newLocation.height = cHeight;
					return newLocation;
				}
			}
			else if (cHeight - 1 >= 0){
				if (currentBoard[cWidth][cHeight - 1].noValue || currentBoard[cWidth][cHeight - 1].cellValue != 0){
					newLocation.width = cWidth;
					newLocation.height = cHeight - 1;
					return newLocation;
				}
			}
		}
	}
	return currentLocation;
}

bool checkCell(int val, location currentLocation, vector<vector<board>> currentBoard){
	int startWidth = 0;
	int startHeight = 0;
	int sum = 0;
	
	for (int i = currentLocation.width; i >= 0; i--){
		if (currentBoard[i][currentLocation.height].rightSum != 0)
			startWidth = i;
	}
	for (int i = currentLocation.height; i >= 0; i--){
		if (currentBoard[currentLocation.width][i].downSum != 0)
			startHeight = i;
	}

	for (int i = startWidth + 1; i <= startWidth + 9; i++){
		if (currentBoard[i][currentLocation.height].cellValue == val)
			return false;
		sum += currentBoard[i][currentLocation.height].cellValue;
	}
	if (sum + val > currentBoard[startWidth][currentLocation.height].rightSum)
		return false;

	sum = 0;
	for (int i = startHeight + 1; i <= startHeight + 9; i++){
		if (currentBoard[currentLocation.width][i].cellValue == val)
			return false;
		sum += currentBoard[currentLocation.width][i].cellValue;
	}
	if (sum + val > currentBoard[currentLocation.width][startHeight].downSum)
		return false;

	return true;
}

bool solve(location currentLocation, vector<vector<board>> currentBoard){
	if (currentLocation.width == (width - 1) && currentLocation.height == (height - 1)){  // puzzle has been solved
		//display();
		return true;
	}
	else{
		for (int i = 1; i <= 9; i++){
			if (checkCell(i, currentLocation, currentBoard)){ //meaning that row, col and no duplicates conditions are met
				currentBoard[currentLocation.width][currentLocation.height].cellValue = 0;
				return solve(nextEmptyLocation(currentLocation, currentBoard), currentBoard);
			}
			else return false;
		}
	}
	return false;
}

vector<vector<board>> loadFile(char* fileName){
	vector<vector<board>> fileBoard;
	int number;
	int i = 0, j = 0;

	bool boardWidth = true;
	bool boardSize = true;
	bool b1 = true;
	bool firstSport;

	ifstream file_handler(fileName);

	while (file_handler >> number) {
		if (number != 0 && boardSize){
			if (boardWidth){
				width = number;
				boardWidth = false;
				continue;
			}
			else{
				height = number;
				fileBoard.resize(width);
				for (int i = 0; i < width; ++i)
					fileBoard[i].resize(height);
				boardSize = false;
				continue;
			}
		}

		if (number == -1){
			if (firstSport){
				Start.width = i;
				Start.height = j;
				firstSport = false;
			}
			fileBoard[i][j].rightSum = 0;
			fileBoard[i][j].downSum = 0;
			fileBoard[i][j].cellValue = 0;
			fileBoard[i][j].noValue = false;
		}
		else if (b1){
			fileBoard[i][j].downSum = number;
			fileBoard[i][j].cellValue = 0;
			fileBoard[i][j].noValue = true;
			b1 = false;
			continue;
		}
		else if (!b1){
			fileBoard[i][j].rightSum = number;
			fileBoard[i][j].cellValue = 0;
			fileBoard[i][j].noValue = true;
			b1 = true;
		}

		if (j++ == (width - 1)){
			j = 0;
			i++;
		}
	}

	return fileBoard;
}