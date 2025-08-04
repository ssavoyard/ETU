#include <stdio.h>
#include <stdlib.h>
#define size 100

void scanline(int m[size][size],int rowm,int cols){
    int i;
    for (i=0;i<cols;i++){
        scanf("%d",&m[rowm][i]);
    }
}

int checkline(int m[size][size],int n[size][size], int rowm, int rown, int cols, int a, int b){
    int i,check=0,res;
    if ((m[rowm][0]<a)||(m[rowm][cols-1]>b)){
        check++;
    }
    else {
        for (i=0;i<(cols-1);i++){
            if (m[rowm][i]>m[rowm][i+1]){
                check++;
            }
        }
    }
    if (check==0){
        for (i=0;i<cols;i++){
            n[rown][i]=m[rowm][i];
        }
        res=1;
    }
    else res=0;
    return res;
}

int main() {
    int m[size][size],n[size][size],cols,a,b,min,i,j,rown=0;
    printf("Enter number of columns: ");
    scanf("%d",&cols);
    printf("Enter interval: ");
    scanf("%d %d",&a,&b);
    puts("Enter matrix: ");
    scanline(m,0,cols);
     min=abs(m[0][0]);
    for (i=1;i<cols;i++){
        if (min>abs(m[0][i])){
            min=abs(m[0][i]);
        }
    }
    for (i=1;i<min;i++){
        scanline(m,i,cols);
    }
    printf("New matrix:");
    for (i=0;i<min;i++){
        rown=rown+checkline(m,n,i,rown,cols,a,b);
    }
    for (i=0;i<rown;i++){
        printf("\n");
        for(j=0;j<cols;j++){
            printf("%d ",n[i][j]);
        }
    }
    return 0;
}
