#include "queens.h"

int main(int argc, char* argv[]) {
    if (argc != 2)
        return 1;

    if (!isNumber(argv[1]))
        return 2;

    int N = atoi(argv[1]);
    printf("c\tN queens problem\nc\tN = %d\nc\n", N);
    int clausules_count = bishopClausulesCount(N) + N * N * (N - 1) + N;

    printf("p cnf %d %d\n", N * N, clausules_count);
    printf("c --------------------- ROWS\n");

    int len = N + 1;
    for (int i = 0; i < N; i++) {
        int a = i * N;
        for (int j = 1; j < len; j++)
            printf("%d ", j + a);
        printf("0\n");
    }

    printf("c --------------------- SHOULDERING\n");
    len = N * N;
    for (int i = 0; i < len; i++) {
        printf("c ## %d ##\n", i);
        rookClausules(i, N);
        bishopClausules(i, N);
    }
}

int bishopClausulesCount(int N) {
    int A = N - 1;
    int B = N - 1;
    int sum = 0;
    while (A >= 0) {
        if (A == 0) {
            sum += B / 2;
            break;
        }
        sum += A * B * 2;
        A -= 2;
        B += 2;
    }
    return sum;
}

void rookClausules(int place, int N) {
    int print_place = place + 1;
    int len = place - place % N + N + 1;
    for (int i = print_place + 1; i < len; i++)
        printf("-%d -%d 0\n", print_place, i);
    len = N * N - N + place % N + 2;
    for (int i = print_place + N; i < len; i += N)
        printf("-%d -%d 0\n", print_place, i);
}

void bishopClausules(int place, int N) {
    int print_place = place + 1;
    int max = N * N;
    int len = print_place + (N - place % N - 1) * (N + 1);
    int step = N + 1;
    for (int i = print_place + N + 1;i <= len;i += step) {
        if (i > max)  break;
        printf("-%d -%d 0\n", print_place, i);
    }
    len = print_place + (place % N) * (N - 1) + 1;
    step = N - 1;
    if (step == 0)return;
    for (int i = print_place + N - 1; i <= len; i += step) {
        if (i > max)  break;
        printf("-%d -%d 0\n", print_place, i);
    }
}

int isNumber(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++)
        if (!isdigit(str[i]))
            return 0;
    return 1;
}
