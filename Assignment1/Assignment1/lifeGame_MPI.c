#include "mpi.h"

#include <stdio.h>
#include <stdlib.h>

void printLiveCell(int **board, int boardLength){
	/*  Iterations are done; sum the number of live cells */
	int i, j, isum = 0;
	for (i = 1; i <= boardLength; i++){
		for (j = 1; j <= boardLength; j++){
			isum = isum + board[i][j];
		}
	}
	printf("\nNumber of live cells = %d\n", isum);
}

void createRandomBoard(int **board, int **tempBoard, int boardSideLength, int threadBoardSize){
	int i, j, x;

	/* Initialize the boundaries of the life matrix */
	for (j = 0; j < boardSideLength + 2; j++)
		board[0][j] = board[threadBoardSize + 1][j] = tempBoard[0][j] = tempBoard[threadBoardSize + 1][j] = 0;
	for (i = 0; i < threadBoardSize + 2; i++)
		board[i][0] = board[i][boardSideLength + 1] = tempBoard[i][0] = tempBoard[i][boardSideLength + 1] = 0;

	/* Initialize the life matrix */
	for (i = 1; i <= threadBoardSize; i++)  {

		for (j = 1; j <= boardSideLength; j++){
			x = rand() / ((float) RAND_MAX + 1);
			if (x < 0.5){
				board[i][j] = 0;
			}
			else {
				board[i][j] = 1;
			}
		}
	}
}

void runRoundOfLife(int **board, int **tempBoard, int boardSideLength, int threadBoardSize, int previousThreadRank, int nextThreadRank, MPI_Comm comm){
	int i, j, sum;

	MPI_Request      req[4];
	MPI_Status       status[4];

	MPI_Isend(&board[1][0], boardSideLength + 2, MPI_INT, previousThreadRank, 0, comm, req);
	MPI_Irecv(&board[0][0], boardSideLength + 2, MPI_INT, previousThreadRank, 0, comm, req + 1);
	MPI_Isend(&board[threadBoardSize][0], boardSideLength + 2, MPI_INT, nextThreadRank, 0, comm, req + 2);
	MPI_Irecv(&board[threadBoardSize + 1][0], boardSideLength + 2, MPI_INT, nextThreadRank, 0, comm, req + 3);
	MPI_Waitall(4, req, status);


	for (i = 1; i <= threadBoardSize; i++) {
		for (j = 1; j < boardSideLength + 1; j++) {


			sum = board[i - 1][j - 1] + board[i - 1][j] + board[i - 1][j + 1]
				+ board[i][j - 1] + board[i][j + 1]
				+ board[i + 1][j - 1] + board[i + 1][j] + board[i + 1][j + 1];

			if (sum < 2 || sum > 3)
				tempBoard[i][j] = 0;
			else if (sum == 3)
				tempBoard[i][j] = 1;
			else
				tempBoard[i][j] = board[i][j];
		}
	}

	board = tempBoard;
}

void playGameOfLife(int numCycles, int boardSideLength, MPI_Comm comm){


	int **board, **tempBoard;

	int i, j, k, x;

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

	createRandomBoard(board, tempBoard, boardSideLength, threadBoardSize);

	for (k = 0; k < numCycles; k++) {
		runRoundOfLife(board, tempBoard, boardSideLength, threadBoardSize, previousThreadRank, nextThreadRank, comm);
	}
	
	printLiveCell(board, boardSideLength);
}


int main(argc, argv)
int argc;
char *argv[];
{
	int rank, N, iters;
	double time;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	N = 1000;
	iters = 500;

	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&iters, 1, MPI_INT, 0, MPI_COMM_WORLD);

	playGameOfLife(N, iters, MPI_COMM_WORLD);

	MPI_Finalize();

}

