#include <time.h>
#include <stdio.h>
#include <malloc.h>

#define N 10000

void main() {
	int i, j;
	time_t start, end;

	int **a = (int **)malloc(N*sizeof(int *));
	for(i = 0; i < N; i++) {
		a[i] = (int *)malloc(N*sizeof(int));
	}

	start = time(0);

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (i > 0 && j > 0) a[i][j] = a[i-1][j-1] + 1;
			a[i][j]++;
		}
	}

	end = time(0);

	for(i = 0; i < N; i++) {
		free(a[i]);
	}
	free(a);

	printf("%.20f\n", difftime(end, start));
}