#include <stdio.h>
#define MSIZE 100

int main(){
    int A[MSIZE][MSIZE], N, M, i, j;
    puts("Enter matrix size:");
    scanf("%i %i", &N, &M);
    if(((N + 1) < MSIZE) && ((M + 1) < MSIZE)){
        puts("Enter matrix:");
        for(i = 0; i < N; i++){
            for(j = 0; j < M; j++){
                scanf("%i", &A[i][j]);
                if(A[i][j] == 0){
                    A[i][M]++;
                    A[N][j]++;
                }
            }
        }
        printf("New matrix: \n");
        for(i = 0; i < N + 1; i++){
            for(j = 0; j < M + 1; j++){
                printf("%i ", A[i][j]);
            }
            printf("\n");
        }

    }
    else{
        puts("Invalid matrix size");
    }
    return 0;
}
