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

void checkBoarder(int** inputArray){

	int height = sizeof(inputArray) / sizeof(inputArray[0]);
	int width = sizeof(inputArray[0]) / sizeof(inputArray[0][0]);

	/* corner boundary conditions */
	inputArray[0][0] = inputArray[height][width];
	inputArray[0][height + 1] = inputArray[height][1];
	inputArray[height + 1][width + 1] = inputArray[1][1];
	inputArray[height + 1][0] = inputArray[1][width];

	/* left-right boundary conditions */

	for (int i = 1; i <= size; i++){
		inputArray[i][0] = inputArray[i][width];
		inputArray[i][height + 1] = inputArray[i][1];
	}

	/* top-bottom boundary conditions */

	for (int j = 1; j <= size; j++){
		inputArray[0][j] = inputArray[height][j];
		inputArray[height + 1][j] = inputArray[1][j];
	}
}

void lifeGame(int size, int gens, string *rs){

	int n, im, ip, jm, jp, ni, nj, nsum, isum;
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

	MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&gens, 1, MPI_INT, 0, MPI_COMM_WORLD);

	ni = size + 2;  /* add 2 for left and right ghost cells */
	nj = size + 2;
	oldd = (int **) malloc(ni*sizeof(int*));
	newd = (int **) malloc(ni*sizeof(int*));

	for (int i = 0; i<ni; i++){
		oldd[i] = (int *) malloc(nj*sizeof(int));
		newd[i] = (int *) malloc(nj*sizeof(int));
	}


	finish = clock() - start;
	interval = finish / (double) CLOCKS_PER_SEC;
	intm = to_string(interval);
	*rs = *rs + "," + intm;


	/*  initialize elements of old to 0 or 1 */
	start = clock();

	for (int i = 1; i <= size; i++){
		for (int j = 1; j <= size; j++){
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


	/*  time steps */
	start = clock();

	for (n = 0; n<gens; n++){

		checkBoarder(oldd);

		for (int i = 1; i <= size; i++){
			for (int j = 1; j <= size; j++){
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

		/* copy new state into old state */
		copyArray(oldd, newd){

		}

	}



	finish = clock() - start;
	interval = finish / (double) CLOCKS_PER_SEC;
	intm = to_string(interval);
	*rs = *rs + "," + intm;


	/*  Iterations are done; sum the number of live cells */
	start = clock();

	isum = 0;
	for (int i = 1; i <= size; i++){
		for (int j = 1; j <= size; j++){
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

void copyArray(int **inputArray, int **outputArray){

	int height = sizeof(inputArray) / sizeof(inputArray[0]);
	int width = sizeof(inputArray[0]) / sizeof(inputArray[0][0]);

	for (int i = 1; i <= height; i++){
		for (int j = 1; j <= width; j++){
			inputArray[i][j] = outputArray[i][j];
		}
	}
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
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	return TimingF();
}
