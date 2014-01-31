/***********************

Conway Game of Life

************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void checkBoardBoundary(int **oldd, int size){
	int i, j;

	/* corner boundary conditions */
	oldd[0][0] = oldd[size][size];
	oldd[0][size + 1] = oldd[size][1];
	oldd[size + 1][size + 1] = oldd[1][1];
	oldd[size + 1][0] = oldd[1][size];

	/* left-right boundary conditions */

	for (i = 1; i <= size; i++){
		oldd[i][0] = oldd[i][size];
		oldd[i][size + 1] = oldd[i][1];
	}

	/* top-bottom boundary conditions */

	for (j = 1; j <= size; j++){
		oldd[0][j] = oldd[size][j];
		oldd[size + 1][j] = oldd[1][j];
	}


}

void checkBoardCenter(int **oldd, int **newd, int size){
	int i, j, im, ip, jm, jp, nsum;

	for (i = 1; i <= size; i++){
		for (j = 1; j <= size; j++){
			im = i - 1;
			ip = i + 1;
			jm = j - 1;
			jp = j + 1;

			nsum = oldd[im][jp] + oldd[i][jp] + oldd[ip][jp]
				+ oldd[im][j] + oldd[ip][j]
				+ oldd[im][jm] + oldd[i][jm] + oldd[ip][jm];

			switch (nsum){

			case 3:
				newd[i][j] = 1;
				break;

			case 2:
				newd[i][j] = oldd[i][j];
				break;

			default:
				newd[i][j] = 0;
			}
		}
	}

}

void copyArray(int **oldd, int **newd, int size){
	int i, j;

	for (i = 1; i <= size; i++){
		for (j = 1; j <= size; j++){
			oldd[i][j] = newd[i][j];
		}
	}

}

void lifeGame(int size, int gens, string *rs){

	int i, j, n, ni, nj, isum;
	int copyArrayTime, checkBoardCenterTime, checkBoardBoundaryTime;
	int **oldd, **newd;
	float x;
	FILE *outFile;

	clock_t start;
	clock_t finish;
	string ptime = "";
	string intm = "";
	double interval = 0;


	/* allocate arrays */
	start = clock();

	ni = size + 2;  /* add 2 for left and right ghost cells */
	nj = size + 2;
	oldd = (int **) malloc(ni*sizeof(int*));
	newd = (int **) malloc(ni*sizeof(int*));

	for (i = 0; i<ni; i++){
		oldd[i] = (int *) malloc(nj*sizeof(int));
		newd[i] = (int *) malloc(nj*sizeof(int));
	}


	finish = clock() - start;
	interval = finish / (double) CLOCKS_PER_SEC;
	intm = to_string(interval);
	*rs = *rs + "," + intm;


	/*  initialize elements of old to 0 or 1 */
	start = clock();

	for (i = 1; i <= size; i++){
		for (j = 1; j <= size; j++){
			x = rand() / ((float) RAND_MAX + 1);
			if (x<0.5){
				oldd[i][j] = 0;
			}
			else {
				oldd[i][j] = 1;
			}
		}
	}

	finish = clock() - start;
	interval = finish / (double) CLOCKS_PER_SEC;
	intm = to_string(interval);
	*rs = *rs + "," + intm;

	for (n = 0; n<gens; n++){

		start = clock();

		checkBoardBoundary(oldd, size);

		finish = clock() - start;
		interval = finish / (double) CLOCKS_PER_SEC;
		checkBoardBoundaryTime += interval;

		start = clock();

		checkBoardCenter(oldd, newd, size);

		finish = clock() - start;
		interval = finish / (double) CLOCKS_PER_SEC;
		checkBoardCenterTime += interval;


		start = clock();

		copyArray(oldd, newd, size);
		
		finish = clock() - start;
		interval = finish / (double) CLOCKS_PER_SEC;
		copyArrayTime += interval;
		
	}

	intm = to_string(checkBoardBoundaryTime);
	*rs = *rs + "," + intm;

	intm = to_string(checkBoardCenterTime);
	*rs = *rs + "," + intm;

	intm = to_string(copyArrayTime);
	*rs = *rs + "," + intm;

	/*  Iterations are done; sum the number of live cells */
	start = clock();

	isum = 0;
	for (i = 1; i <= size; i++){
		for (j = 1; j <= size; j++){
			isum = isum + newd[i][j];
		}
	}

	finish = clock() - start;
	interval = finish / (double) CLOCKS_PER_SEC;
	intm = to_string(interval);
	*rs = *rs + "," + intm;


	printf("\nNumber of live cells = %d\n", isum);

	return;
}

int TimingF(){
	int bdsize = 200;
	int gens = 500;
	int rounds = 10;
	string file = "";

	cout << "Please input board size:";
	cin >> bdsize;
	cout << endl;

	cout << "Please input number of generations:";
	cin >> gens;
	cout << endl;

	cout << "Please input number of rounds:";
	cin >> rounds;
	cout << endl;

	cout << "Please input file to store result:";
	getline(cin, file);
	getline(cin, file);
	cout << endl;

	fstream fs;
	fs.open(file, fstream::in | fstream::out | fstream::app);
	fs << "\n";

	for (int i = 0; i < rounds; i++){
		string re = "";
		re = "S:" + to_string(bdsize) + "G:" + to_string(gens) + "R:" + to_string(i);
		lifeGame(bdsize, gens, &re);
		re = re + "\n";
		fs << re;
	}

	fs.close();

	return 0;
}

int main(int argc, char *argv[]){
	return TimingF();
}