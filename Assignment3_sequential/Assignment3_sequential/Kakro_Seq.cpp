#include "Kakro.h"

using namespace std;

sumNode** sums;
int** solution;
int width, height;


int main(char** args, int c){

	
	return 0;
}

bool checkCell(int i, int currentWidth, int currentHeight){

	return true;
}

void nextEmptyLocation(int* currentWidth, int* currentHeight){
	if (*currentWidth + 1 < width || *currentHeight + 1< height){
		if (*currentWidth - 1 >= 0 || *currentHeight - 1 >= 0){
			if (*currentWidth + 1 )
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
}

void createArrays(int width, int height){
	sums = (sumNode**)malloc(width*sizeof(sumNode*));
	int i, j;
	for (i = 0; i < width; i++)
	{
		sums[i] = (sumNode*)malloc(height*sizeof(sumNode));
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

	input_values = fopen("text.txt", "r");

	if (input_values == NULL) {
		fprintf(stderr, "Error! Could not open file.\n");
	}

	int fileWidth, fileHeight;
	int i, j;

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

//	fscanf(input_values, "%d", &sum);
	while (sum != EOF) {
		fscanf(input_values, "%d", &sum);
		sums[i][j].rightSum = sum;
		fscanf(input_values, "%d", &sum);
		sums[i][j].downSum = sum;
		if (i++ == (width - 1)){
			i = 0;
			j++;
		}																																
	}

	fclose(input_values);

}
