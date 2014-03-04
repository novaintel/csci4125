#ifndef Kakro_Seq
#define Kakro_Seq

#include <iostream>
#include <fstream>

struct sumNode {
	int rightSum;
	int downSum;
	int isSum;
};

class Kakro_Seq{

public:
	int main(char** args,int argc);

private:
	void loadPuzzel(char* fileName);
	void createArrays(int width, int height);
	void deletArrays();
	sumNode** sums;
	int** solution;
};

#endif