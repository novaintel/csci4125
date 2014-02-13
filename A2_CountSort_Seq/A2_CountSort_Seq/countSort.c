struct Key
{
	int key;
};

int main(int argc, char *argv []){
	return 0;
}

void countingsort(struct Key* A, struct Key* B, int k){
	int i,j;
	int* C;

	C = (int *) malloc(k*sizeof(int *));

	for (i = 1; i < k; i++){
		C[i] = 0;
	}

	for (j = 1; j < length(A);j++){
		C[A[j].key] = C[A[j].key] + 1;
	}

	for (i = 1; i < k;i++){
		C[i] = C[i] + C[i - 1];
	}

	for (j = 1; j < length(A); j++){
		B[C[A[j].key]] = A[j];
		C[A[j].key] = C[A[j].key] - 1;
	}
}