#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <time.h>

// Boards dimension values
unsigned int width;
unsigned int height;

// Number of threads
unsigned int nthreads;

//Number of rounds
unsigned int rounds;

int **workingBoard;
int **finalBoard;

// Input filename
char *filename;

void initialize_board(int **curptr)
{
	int i, j;

	for (i = 0; i<width; i++) for (j = 0; j<height; j++)
		curptr[i][j] = 0;
}

void read_file(int **curptr, char *name, int height, int width)
{
	FILE *f;
	int	i, j;
	char s[100];

	f = fopen(name, "r");
	for (j = 0; j<height; j++)
	{

		fgets(s, 100, f);

		for (i = 0; i<width; i++)
		{
			curptr[i][j] = s[i] == 'x';
		}
	}
	fclose(f);
}

void copy_region(int **curptr, int **nextptr)
{
	int i, j;
	for (j = 0; j<height; j++)
		for (i = 0; i<width; i++)
			if ((i == 0) || (j == 0) || (j == height - 1) || (i == width - 1))
				nextptr[i][j] = curptr[i][j];
}

int adjacent_to(int **curptr, int i, int j)
{
	int row, col, count;

	count = 0;

	// Examine all the neighbors
	for (row = -1; row <= 1 && count <= 3; row++)
		for (col = -1; col <= 1 && count<=3; col++)
		{
			// exclude current cell from count
			if (row || col)
				if (curptr[i + row][j + col]) count++;
		}
	return count;
}

void play(int **curptr, int **nextptr, int start, int finish)
{
	int i, j, alive;

	for (i = 1; i<width - 1; i++) for (j = start; j<finish; j++)
	{
		alive = adjacent_to(curptr, i, j);
		if (alive == 2) nextptr[i][j] = curptr[i][j];
		if (alive == 3) nextptr[i][j] = 1;
		if (alive < 2) nextptr[i][j] = 0;
		if (alive > 3) nextptr[i][j] = 0;
	}
}

int cmdArgCheck(int argc, char *argv[])
{
	int opt_char;
	if (argc == 1 || argc < 5){
		return -1;
	}
	nthreads = atoi(argv[1]);
	rounds = atoi(argv[2]);
	height = atoi(argv[3]);
	width = atoi(argv[4]);
	filename = argv[5];

	if (height != 0 || width != 0)
	{
		printf("Height or width cannot be 0.");
		return -1;
	}
	return 0;
}

void allocatBoardMemory(int** inputboard){
	int i;

	inputboard = (int **) malloc(width*sizeof(int *));

	for (i = 0; i < width; i++){
		inputboard[i] = (int *) malloc(height*sizeof(int));
	}
}

int main(int argc, char *argv[])
{
	int **curptr, **nextptr, **temp;

	int i, tid;
	unsigned int start, finish;

	height = 0;
	width = 0;
	filename = NULL;
	nthreads = 1;

	if (cmdArgCheck(argc, argv) != 0)
		return -1;

	allocatBoardMemory(workingBoard);
	allocatBoardMemory(finalBoard);

	curptr = workingBoard;
	nextptr = finalBoard;

	initialize_board(curptr);

	read_file(curptr, filename, width, height);

	copy_region(curptr, nextptr);

#pragma omp parallel shared(curptr, nextptr, temp, nthreads) private(tid, i) num_threads(nthreads)
	{
		tid = omp_get_thread_num();

		start = tid * (height / nthreads);
		finish = start + (height / nthreads);

		if (tid == 0) start++;
		if (tid == nthreads - 1) finish = height - 1;

		for (i = 0; i<rounds; i++)
		{
			play(curptr, nextptr, start, finish);

#pragma omp barrier

			if (tid == 0)
			{
					temp = curptr;
					curptr = nextptr;
					nextptr = temp;
			}


#pragma omp barrier

		}
	}

	return 0;
}








