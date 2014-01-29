#include "mpi.h"

#include <stdio.h>
#include <stdlib.h>

void initProcess(MPI_Comm comm){

}

void createTestBoard(int **board){
	
	int boardHeight = sizeof(*board) / (sizeof(int *));
	int boardWidth = sizeof(**board) / (sizeof(int *));

	for (int i = 1; i <= boardHeight; i++){
		for (int j = 1; j <= boardWidth; j++){
			int x = rand() / ((float) RAND_MAX + 1);
			if (x<0.5){
				board[i][j] = 0;
			}
			else {
				board[i][j] = 1;
			}
		}
	}

}

void initBoard(int** board, int boardHeight, int boardWidth){
	board = malloc(boardHeight*sizeof(int*));

	for (int i = 0; i<boardHeight; i++){
		board[i] = malloc(boardWidth*sizeof(int));
	}

	createTestBoard(board);
}

int main(char** argv, int argc){

	int processRank, boardHeight, boardWidth, iters;
	int **board;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &processRank);

	if (processRank == 0) {
		printf("Matrix Size : ");
		scanf("%d", &boardHeight);
		scanf("%d", &boardWidth);
		printf("Iterations : ");
		scanf("%d", &iters);
	}

	initBoard(board, boardHeight, boardWidth);

	MPI_Bcast(&boardHeight, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&boardWidth, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&iters, 1, MPI_INT, 0, MPI_COMM_WORLD);

	


}