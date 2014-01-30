#include "mpi.h"

#include <stdio.h>
#include <stdlib.h>


void randomGame(int** matrix, int** temp, int matrix_size, int mysize){
	/* Initialize the boundaries of the life matrix */
	int i, j, x;
	for (j = 0; j < matrix_size + 2; j++)
		matrix[0][j] = matrix[mysize + 1][j] = temp[0][j] = temp[mysize + 1][j] = 0;
	for (i = 0; i < mysize + 2; i++)
		matrix[i][0] = matrix[i][matrix_size + 1] = temp[i][0] = temp[i][matrix_size + 1] = 0;

	/* Initialize the life matrix */
	for (i = 1; i <= mysize; i++)  {
		for (j = 1; j <= matrix_size; j++)
			x = rand() / ((float) RAND_MAX + 1);
			if (x<0.5){
				matrix[i][j] = 0;
			}
			else {
				matrix[i][j] = 1;
			}
	}
}


void playRoundOfLife(int numCycles, int boardSideLength, MPI_Comm comm){

	int **board, **tempBoard;

	int i, j, k;

	int currentThreadRank, previousThreadRank, nextThreadRank, size;
	int threadBoardSize, threadOffset;
	int sum;

	MPI_Comm_size(comm, &size);
	MPI_Comm_rank(comm, &currentThreadRank);

	if (currentThreadRank == 0)
		previousThreadRank = MPI_PROC_NULL;
	else
		previousThreadRank = currentThreadRank - 1;
	if (currentThreadRank == size - 1)
		nextThreadRank = MPI_PROC_NULL;
	else
		nextThreadRank = currentThreadRank + 1;


	if (currentThreadRank < (boardSideLength % size))
		threadBoardSize = boardSideLength / size + 1;
	else
		threadBoardSize = boardSideLength / size + 0;

	threadOffset = currentThreadRank * (boardSideLength / size);

	if (currentThreadRank >(boardSideLength % size))
		threadOffset += (boardSideLength % size);
	else
		threadOffset += currentThreadRank;

	board = (int **) malloc(sizeof(int *) *(threadBoardSize + 2));
	for (i = 0; i < threadBoardSize + 2; i++) {
		board[i] = (int *) malloc(sizeof(int) *(threadBoardSize + 2));
	}

	tempBoard = (int **) malloc(sizeof(int *) *(threadBoardSize + 2));
	for (i = 0; i < threadBoardSize + 2; i++) {
		tempBoard[i] = (int *) malloc(sizeof(int) *(threadBoardSize + 2));
	}


	randomGame(board, tempBoard, boardSideLength, threadBoardSize);

	for (k = 0; k < numCycles; k++) {
		MPI_Request      req[4];
		MPI_Status       status[4];

		/* Send and receive boundary information */
		MPI_Isend(&board[1][0], boardSideLength + 2, MPI_INT, previousThreadRank, 0, comm, req);
		MPI_Irecv(&board[0][0], boardSideLength + 2, MPI_INT, previousThreadRank, 0, comm, req + 1);
		MPI_Isend(&board[threadBoardSize][0], boardSideLength + 2, MPI_INT, nextThreadRank, 0, comm, req + 2);
		MPI_Irecv(&board[threadBoardSize + 1][0], boardSideLength + 2, MPI_INT, nextThreadRank, 0, comm, req + 3);
		MPI_Waitall(4, req, status);

		/* For each element of the matrix ... */
		for (i = 1; i <= threadBoardSize; i++) {
			for (j = 1; j < boardSideLength + 1; j++) {

				/* find out the value of the current cell */
				sum = board[i - 1][j - 1] + board[i - 1][j] + board[i - 1][j + 1]
					+ board[i][j - 1] + board[i][j + 1]
					+ board[i + 1][j - 1] + board[i + 1][j] + board[i + 1][j + 1];

				/* check if the cell dies or life is born */
				if (sum < 2 || sum > 3)
					tempBoard[i][j] = 0;
				else if (sum == 3)
					tempBoard[i][j] = 1;
				else
					tempBoard[i][j] = board[i][j];
			}
		}
		/* Swap the matrices */
		board = tempBoard;
	}


}

void startGame(int iters){
	
}

int main(char** argv, int argc){

	int processRank, boardSideLength, iters;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &processRank);

	if (processRank == 0) {
		printf("Matrix Size : ");
		scanf("%d", &boardSideLength);
		printf("Iterations : ");
		scanf("%d", &iters);
	}

	MPI_Bcast(&boardSideLength, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&iters, 1, MPI_INT, 0, MPI_COMM_WORLD);

}