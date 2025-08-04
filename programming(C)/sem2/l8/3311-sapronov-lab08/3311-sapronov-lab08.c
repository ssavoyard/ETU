#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MSIZE 100

typedef struct CITY {
    char name[MSIZE];
    char region[MSIZE];
    int code;
    float latitude;
    float longitude;
    int stats[2];
    struct CITY *next;
} CITY_node;

typedef struct head_cities {
    int cnt;
    CITY_node *first;
    CITY_node *last;
} head;

head *make_head() {
    head *ph = NULL;
    if ((ph = (head*)malloc(sizeof(head))) != NULL) {
        ph->cnt = 0;
        ph->first = NULL;
        ph->last = NULL;
    }
    return ph;
}

CITY_node *create_node(char *name, char *region, int code, float latitude, float longitude, int stat1, int stat2) {
    CITY_node *new_CITY = (CITY_node*)malloc(sizeof(CITY_node));
    if (new_CITY != NULL) {
        strcpy(new_CITY->name, name);
        strcpy(new_CITY->region, region);
        new_CITY->code = code;
        new_CITY->latitude = latitude;
        new_CITY->longitude = longitude;
        new_CITY->stats[0] = stat1;
        new_CITY->stats[1] = stat2;
        new_CITY->next = NULL;
    } else {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return new_CITY;
}

void append(head* list, char *name, char *region, int code, float latitude, float longitude, int stat1, int stat2) {
    CITY_node *new_CITY = create_node(name, region, code, latitude, longitude, stat1, stat2);
    if (list->first == NULL) {
        list->first = new_CITY;
        list->last = new_CITY;
    } else {
        list->last->next = new_CITY;
        list->last = new_CITY;
    }
    list->cnt++;
}

void insert(head* list, int id, char *name, char *region, int code, float latitude, float longitude, int stat1, int stat2) {
    int i;
    CITY_node *target = list->first;
    CITY_node *new_CITY = NULL;
    if (id == list->cnt) {
        append(list, name, region, code, latitude, longitude, stat1, stat2);
    } else {
        new_CITY = create_node(name, region, code, latitude, longitude, stat1, stat2);
        for (i = 0; i < id - 1 && target != NULL; i++) {
            target = target->next;
        }
        new_CITY->next = target->next;
        target->next = new_CITY;
        list->cnt++;
    }
}


void print_header() {
    printf("|%20s|%15s|%12s|%10s|%10s|%10s|%15s|\n", "Name", "Region", "Region code", "Latitude", "Longitude", "Area", "Population");
    printf("+--------------------+---------------+------------+----------+----------+----------+---------------+\n");
}

void print_list(head *list) {
    CITY_node *current = list->first;
    print_header();
    while (current != NULL) {
        printf("|%20s|%15s|%12d|%10.2f|%10.2f|%10d|%15d|\n", current->name, current->region, current->code, current->latitude,
               current->longitude, current->stats[0], current->stats[1]);
        current = current->next;
    }
    printf("\n");
    free(current);
}

void free_list(head *list) {
    CITY_node *current = list->first;
    CITY_node *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
    free(current);
}

void clear_console() {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    head *ph = make_head();
    CITY_node *current = NULL;
    FILE *file;

    int id, code, stat1, stat2, i, answer;
    float latitude, longitude;
    char name[MSIZE], region[MSIZE], name_base[MSIZE][MSIZE];

    if ((file = fopen("cities.csv", "r")) != NULL) {
        i = 0;
        while (fscanf(file, "%[^,],%[^,],%d,%f,%f,%d,%d\n", name, region, &code, &latitude, &longitude, &stat1, &stat2) == 7) {
            append(ph, name, region, code, latitude, longitude, stat1, stat2);
            strcpy(name_base[i], name);
            i++;
        }
        fclose(file);
    } else {
        printf("Can't open the file\n");
        exit(EXIT_FAILURE);
    }
    do {
        clear_console();
        print_list(ph);
        printf("\nEnter '0' if you want to add new city\nEnter '1' to exit program\n");
        scanf(" %d", &answer);
        getchar();
        if (answer == 0) {
            clear_console();
            print_list(ph);
            printf("Enter name of the city, after which the new city will be added: ");
            fgets(name, MSIZE, stdin);
            name[strcspn(name, "\n")] = '\0';
            id = 0;
            for (i = 0; i < ph->cnt; i++) {
                if (strcmp(name_base[i], name) == 0) i = ph->cnt;
                id++;
            }
            printf("Enter name: ");
            fgets(name, MSIZE, stdin);
            name[strcspn(name, "\n")] = '\0';
            printf("Enter region: ");
            fgets(region, MSIZE, stdin);
            region[strcspn(region, "\n")] = '\0';
            printf("Enter region code: ");
            scanf("%d", &code);
            printf("Enter latitude: ");
            scanf("%f", &latitude);
            printf("Enter longitude: ");
            scanf("%f", &longitude);
            printf("Enter area: ");
            scanf("%d", &stat1);
            printf("Enter population: ");
            scanf("%d", &stat2);
            strcpy(name_base[ph->cnt], name);
            insert(ph, id, name, region, code, latitude, longitude, stat1, stat2);
            current = ph->first;
            for(i=0; i < ph->cnt; i++){
                strcpy(name_base[i], current->name);
                current = current->next;
            }
        }
    } while (answer == 0);
    clear_console();
    free_list(ph);
    free(current);
    return 0;
}
