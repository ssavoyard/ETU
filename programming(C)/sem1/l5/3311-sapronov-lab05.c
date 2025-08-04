#include <stdio.h>
#include <string.h>
#define size 100

int sepcheck(char ch, char *seps){
    return strchr(seps, ch) != NULL;
}

void addwordstofile(char *str, const char *filename, char *seps) {
   char line[size];
   int index = 0, i;
   FILE *file = fopen(filename, "w");
   if (file==NULL) printf("Couldn't open file");
   else{
       for (i = 0; i < strlen(str); i++){
           if (!sepcheck(str[i], seps)){
               line[index++] = str[i];
           }
           else{
               line[index] = '\0';
               if (index > 0) {
                  fprintf(file, "%s\n", line);
               }
               index = 0;
           }
       }
       line[index] = '\0';
       if (index > 0) {
           fprintf(file, "%s\n", line);
       }
       fclose(file);
       printf("Words successfully added to file!\n");
   }
}

int countwords(const char *filename, char *seps, int lim){
    int count = 0;
    char line[size];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Couldn't open file\n");
        count=-1;
    }
    else{
        while (fgets(line, sizeof(line), file) != NULL) {
            if (strlen(line)>lim+1) count++;
        }
        fclose(file);
    }
    return count;
}

int main(){
    char seps[50],text[1000];
    int lim;
    const char *filename = "words.txt";
    printf("Enter separators: ");
    fgets(seps, size, stdin);
    seps[strlen(seps)-1] = '\0';
    printf("Enter the text: ");
    fgets(text, size, stdin);
    text[strlen(text)-1] = '\0';
    printf("Enter the length limit: ");
    scanf("%d",&lim);
    printf("Adding words to file...\n");
    addwordstofile(text, filename, seps);
    printf("Counting words...\n");
    printf("The result is: %d",countwords(filename, seps, lim));
    return 0;
}
