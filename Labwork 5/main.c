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

	for (i = 1; i < N; i++) {
		for (j = 1; j < N; j++) {
			a[j][i] = a[j-1][i-1] + 1;
		}
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			a[j][i]++;
		}
	}

	end = time(0);

	for(i = 0; i < N; i++) {
		free(a[i]);
	}
	free(a);

	printf("%.20f\n", difftime(end, start));
}