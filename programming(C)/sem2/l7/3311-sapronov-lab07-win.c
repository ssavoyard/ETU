#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MSIZE 100

struct CITY {
    char name[MSIZE];
    char region[MSIZE];
    int code;
    float latitude;
    float longitude;
    int stats[2];
};

void clearConsole() {
    system("cls");
}

void print_header() {
    printf("|%20s|%15s|%12s|%10s|%10s|%10s|%15s|\n", "Name", "Region", "Region code", "Latitude", "Longitude", "Area", "Population");
    printf("+--------------------+---------------+------------+----------+----------+----------+---------------+\n");
}

void addCityToFile(const struct CITY *City, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Unable to open the file");
        exit(1);
    }
    fprintf(file, "\n%s,%s,%d,%.2f,%.2f,%d,%d", City->name, City->region, City->code, City->latitude, City->longitude, City->stats[0], City->stats[1]);
    fclose(file);
}

void sort_structs(struct CITY *sarray, int n) {
    struct CITY tmp;
    int i, j;
    for(i = 0; i < n; i++) {
        tmp = sarray[i];
        for(j = i - 1; (j >= 0) && (sarray[j].stats[1] < tmp.stats[1]); j--) sarray[j + 1] = sarray[j];
        sarray[j + 1] = tmp;
    }
}

int main() {
    FILE *file;
    struct CITY *cities = NULL;
    struct CITY newCity;
    int size = 0, exitFlag = 0, i, mx, mn, format, flag = 1;
    char line[MSIZE], trigger[MSIZE];
    file = fopen("cities.csv", "r");
    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) size++;
        rewind(file);
        cities = (struct CITY *)malloc(size * sizeof(struct CITY));
        if (cities) {
            for(i = 0; i < size && flag; i++) {
                fgets(line, sizeof(line), file);
                if (sscanf(line, "%[^,],%[^,],%d,%f,%f,%d,%d", cities[i].name, cities[i].region, &cities[i].code, &cities[i].latitude, &cities[i].longitude, &cities[i].stats[0], &cities[i].stats[1]) != 7) {
                    fprintf(stderr, "Error reading line: %s\n", line);
                    flag = 0;
                }
            }
            fclose(file);
            while (!exitFlag) {
                printf("To display all cities, enter '0'.\nTo sort cities, enter '1'.\nTo add a new city to the list, enter 'add'.\nTo exit, enter 'exit'\n\n");
                fgets(trigger, MSIZE, stdin);
                clearConsole();
                if (strstr(trigger, "exit")) {
                    exitFlag = 1;
                    clearConsole();
                }
                else if (strstr(trigger, "0")) {
                    clearConsole();
                    printf("List of cities:\n\n");
                    print_header();
                    for (i = 0; i < size; i++) {
                        printf("|%20s|%15s|%12d|%10.2f|%10.2f|%10d|%15d|\n", cities[i].name, cities[i].region, cities[i].code, cities[i].latitude, cities[i].longitude, cities[i].stats[0], cities[i].stats[1]);
                    }
                    printf("\n");
                }
                else if (strstr(trigger, "1")) {
                    printf("Enter the number for sorting\n 0. By area \n 1. By population \n\n ");
                    scanf("%d", &format);
                    sort_structs(cities, size);
                    puts("\nEnter with a space min and max values\n");
                    scanf("%d %d", &mn, &mx);
                    getchar();
                    clearConsole();
                    print_header();
                    for (i = 0; i < size; i++) {
                        if (mn <= cities[i].stats[format] && mx >= cities[i].stats[format]) {
                            printf("|%20s|%15s|%12d|%10.2f|%10.2f|%10d|%15d|\n", cities[i].name, cities[i].region, cities[i].code, cities[i].latitude, cities[i].longitude, cities[i].stats[0], cities[i].stats[1]);
                        }
                    }
                    printf("\n");
                    if (i == 0) puts("Not found elements with the specified characteristics\n");
                }
                else if (strstr(trigger, "add")) {
                    printf("Enter name: ");
                    scanf("%s", newCity.name);
                    printf("Enter region: ");
                    scanf("%s", newCity.region);
                    printf("Enter code: ");
                    scanf("%d", &newCity.code);
                    printf("Enter latitude: ");
                    scanf("%f", &newCity.latitude);
                    printf("Enter longitude: ");
                    scanf("%f", &newCity.longitude);
                    printf("Enter area: ");
                    scanf("%d", &newCity.stats[0]);
                    printf("Enter population: ");
                    scanf("%d", &newCity.stats[1]);
                    printf("\n");
                    addCityToFile(&newCity, "cities.csv");
                    getchar();
                    printf("New City added successfully! To update the database you need to restart the program.\n\n");
                }
            }
            free(cities);
        } else puts("Out of memory");
    } else perror("Unable to open the file");
    return 0;
}
