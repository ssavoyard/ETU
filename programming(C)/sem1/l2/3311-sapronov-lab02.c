#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
int main(){
    int *a, *b;
    int i, m, k, s, n;
    printf("Enter list size: ");
    scanf("%d", &n);
    a = (int*)malloc(n * sizeof(int));
    if (a==NULL){
        printf("Memory not allocated\n");
        free(a);
        return 0;
    }
    else b = (int*)malloc(n * sizeof(int));
    printf("Enter m: ");
    scanf("%d", &m);
    printf("Enter k: ");
    scanf("%d", &k);
    printf("Enter s: ");
    scanf("%d", &s);
    for (i = 0; i<n; i++){
        printf("a[%d] = ", i);
        scanf("%d", &a[i]);
    }
    if (s>k){
        for (i=0; i<n; i++){
            if ((i>=s)&&(i<s+m)) b[i]=a[k+i-s];
            else if ((i>=k)&&(i<s)) b[i]=a[i+m];
            else b[i]=a[i];
        }
    }
    else {
        for (i=0; i<n; i++){
            if ((i>=s)&&(i<s+m)) b[i]=a[k+i-s];
            else if ((i>=s)&&(i<k+m)) b[i]=a[i-m];
            else b[i]=a[i];
        }
    }
    for (i = 0; i<n; i++) printf("%d ", b[i]);
    free(a);
    free(b);
    return 0;
}
