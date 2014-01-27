/***********************

Conway Game of Life

************************/

#include <stdio.h>

#include <stdlib.h>

#define NI 1000        /* array sizes */

#define NJ 1000

#define NSTEPS 500    /* number of time steps */

int i, j, n, im, ip, jm, jp, ni, nj, nsum, isum;
int **old, **new;
float x;
FILE *outFile;

void checkCorner(){
	/* corner boundary conditions */
	old[0][0] = old[NI][NJ];
	old[0][NJ + 1] = old[NI][1];
	old[NI + 1][NJ + 1] = old[1][1];
	old[NI + 1][0] = old[1][NJ];
}

void checkLeftRight(){
	for (i = 1; i <= NI; i++){
		old[i][0] = old[i][NJ];
		old[i][NJ + 1] = old[i][1];
	}

}

void checkTopBottom(){
	for (j = 1; j <= NJ; j++){
		old[0][j] = old[NI][j];
		old[NI + 1][j] = old[1][j];
	}
}

void checkSection(){
	for (i = 1; i <= NI; i++){
		for (j = 1; j <= NJ; j++){
			im = i - 1;
			ip = i + 1;
			jm = j - 1;
			jp = j + 1;

			nsum = old[im][jp] + old[i][jp] + old[ip][jp]
				+ old[im][j] + old[ip][j]
				+ old[im][jm] + old[i][jm] + old[ip][jm];

			switch (nsum){

			case 3:
				new[i][j] = 1;
				break;

			case 2:
				new[i][j] = old[i][j];
				break;

			default:
				new[i][j] = 0;
			}
		}
	}
}

void copyBoard(){
	for (i = 1; i <= NI; i++){
		for (j = 1; j <= NJ; j++){
			old[i][j] = new[i][j];
		}
	}
}

void doTimeStep(int n){

	/* corner boundary conditions */
	checkCorner();

	/* left-right boundary conditions */

	checkLeftRight();

	/* top-bottom boundary conditions */

	checkTopBottom();

	checkSection();

	/* copy new state into old state */
	copyBoard();
	
}

void init(){
	/* allocate arrays */

	ni = NI + 2;  /* add 2 for left and right ghost cells */
	nj = NJ + 2;
	old = malloc(ni*sizeof(int*));
	new = malloc(ni*sizeof(int*));

	for (i = 0; i<ni; i++){
		old[i] = malloc(nj*sizeof(int));
		new[i] = malloc(nj*sizeof(int));
	}

	/*  initialize elements of old to 0 or 1 */

	for (i = 1; i <= NI; i++){
		for (j = 1; j <= NJ; j++){
			x = rand() / ((float) RAND_MAX + 1);
			if (x<0.5){
				old[i][j] = 0;
			}
			else {
				old[i][j] = 1;
			}
		}
	}

}

int main(int argc, char *argv[]) {

	init();

	/*  time steps */
	for (n = 0; n<NSTEPS; n++){
		doTimeStep(n);
	}

	/*  Iterations are done; sum the number of live cells */
	isum = 0;
	for (i = 1; i <= NI; i++){
		for (j = 1; j <= NJ; j++){
			isum = isum + new[i][j];
		}
	}
	printf("\nNumber of live cells = %d\n", isum);

	//getchar(); /*To get the console window to stay open!*/

	return 0;
}