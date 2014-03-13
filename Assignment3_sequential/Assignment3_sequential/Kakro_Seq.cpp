#include <iostream>
#include <fstream>

struct sumNode {
	int rightSum;
	int downSum;
	bool solLocation;
};

void loadPuzzel(char* fileName);
void createArrays(int width, int height);
void deletArrays();


using namespace std;

sumNode** sums;
int** solution;
int width, height;


int main(int argc, char *argv[]){

	if (argc <= 1)
		return -1;

	loadPuzzel(argv[1]);
	return 0;
}

bool checkCell(int callValue, int currentWidth, int currentHeight){
	int i;
	int count;
	int startWidth;
	int startHeight;
	if (callValue < 1 || callValue > 9)
		return false;
	if (currentWidth > width || currentHeight > height)
		return false;
	for (i = 0; i < currentWidth; i++){
		if ((sums[i][currentHeight].solLocation) == false){
			if (sums[i][currentHeight].rightSum != 0)
				startWidth = i;
		}

	}
	for (i = 0; i < currentHeight; i++){
		if ((sums[currentWidth][i].solLocation) == false){
			if (sums[currentWidth][i].downSum != 0)
				startHeight = i;
		}
	}
	count = callValue;
	for (i = startWidth; i < startWidth + 9; i++){
		if (i == solution[i][currentHeight])
			return false;
		count += solution[i][currentHeight];
	}
	if (count > sums[startWidth][currentHeight].rightSum)
		return false;
	count = callValue;
	for (i = startHeight; i < startHeight + 9; i++){
		if (i == solution[currentWidth][i])
			return false;
		count += solution[i][currentHeight];
	}
	if (count > sums[currentWidth][startHeight].downSum)
		return false;
	return true;
}

void nextEmptyLocation(int* currentWidth, int* currentHeight){
	if (*currentWidth + 1 < width || *currentHeight + 1< height){
		if (*currentWidth - 1 > 0 || *currentHeight - 1 > 0){
			if (sums[*currentWidth + 1][*currentHeight].solLocation == true && solution[*currentWidth + 1][*currentHeight] != 0)
				currentWidth++;
			else if (sums[*currentWidth][*currentHeight + 1].solLocation == true && solution[*currentWidth][*currentHeight + 1] != 0)
				currentHeight++;
			else if (sums[*currentWidth - 1][*currentHeight].solLocation == true && solution[*currentWidth - 1][*currentHeight] != 0)
				currentHeight--;
			else if (sums[*currentWidth][*currentHeight - 1].solLocation == true && solution[*currentWidth][*currentHeight - 1] != 0)
				currentHeight--;
		}
		
	}
}

bool solve(int currentWidth, int currentHeight){
	int i;
	if (currentWidth == width && currentHeight == height){ // puzzle has been solved
		//display();
		return true;
	}
	else {
		for (i = 1; i <= 9; i++)
			if (checkCell(i, currentWidth, currentHeight)){ //meaning that row, col and no duplicates conditions are met
				solution[currentWidth][currentHeight] = i;
				nextEmptyLocation(&currentWidth, &currentWidth);
				return solve(currentWidth, currentHeight);
			}
			else {
				return false;
			}
	}
	return false;
}

void createArrays(int width, int height){
	sums = (sumNode**)malloc(width*sizeof(sumNode*));
	int i, j;
	for (i = 0; i < width; i++)
	{
		sums[i] = (sumNode*)malloc(height*sizeof(sumNode));
	}

	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			sums[i][j].downSum = 0;
			sums[i][j].solLocation = false;
			sums[i][j].rightSum = 0;
		}
	}

	solution = (int**)malloc(width*sizeof(int*));

	for (i = 0; i < width; i++)
	{
		solution[i] = (int*)malloc(height*sizeof(int));
	}

	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			solution[i][j] = 0;
		}
	}
}

void deletArrays(){
	int i;
	int width = sizeof(sums) / sizeof(sumNode);

	for (i = 0; i<width; i++)
	{
		free(sums[i]);
	}

	free(sums);

	width = sizeof(solution) / sizeof(int);

	for (i = 0; i<width; i++)
	{
		free(solution[i]);
	}

	free(solution);
}

void loadPuzzel(char* fileName){

	FILE * input_values;

	input_values = fopen(fileName, "r");

	if (input_values == NULL) {
		fprintf(stderr, "Error! Could not open file.\n");
		return;
	}

	int fileWidth, fileHeight;
	int i = 0, j = 0;

	fscanf(input_values, "%d", &fileWidth);
	fscanf(input_values, "%d", &fileHeight);

	if (fileWidth != 0 && fileHeight != 0){
		width = fileWidth;
		height = fileHeight;
	}
	else
		return;

	createArrays(width, height);

	int sum;

	fscanf(input_values, "%d", &sum);
	while (sum != EOF) {
		if (sum == -1){
			sums[i][j].solLocation = true;
		}
		sums[i][j].downSum = sum;
		fscanf(input_values, "%d", &sum);
		sums[i][j].rightSum = sum;
		if (i++ == (width - 1)){
			i = 0;
			j++;
		}
		if (sums[i][j].downSum == 0 && sums[i][j].rightSum == 0)
			solution[i][j] = -1;

		fscanf(input_values, "%d", &sum);
	}

	fclose(input_values);

}
