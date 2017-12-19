#include <stdio.h>
#include <windows.h>
#include <time.h>

#define N 1000000000

int func1(int a, int b);
int func2(int a, int b);
int resultOfSum(int a, int b);

int func1(int a, int b) {
	int res = 0;
	//long i;
	//for (i = 0; i < N; i++) {
		res = func2(a, b);
		if (res > 0) return res;
	//}
	return res;
}

int func2(int a, int b) {
	int res = 0;
	long i;
	//for (i = 0; i < N; i++) {
		//Sleep(1);
		//if (i > N-2)
			res = a + b;//resultOfSum(a, b);
		if (res > 0) return res;
	//}
	return res;
}

int resultOfSum(int a, int b) {
	return a + b;
}

void main() {
	time_t start, end;
	int res;
	start = time(0);
	res = func1(5, 10);
	end = time(0);
	printf("%.20f\n", difftime(end, start));
}