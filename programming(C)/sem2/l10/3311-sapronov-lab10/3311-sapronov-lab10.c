#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MSIZE 100

typedef struct city_regions
{
    int id;
    char name[MSIZE];
    struct city_regions* next;
    struct city_regions* prev;
} region_node;

typedef struct head_city_regions
{
    int cnt;
    struct city_regions* first;
    struct city_regions* last;
} regions_head;

typedef struct city {
    char *name;
    region_node *region;
    int code;
    float latitude;
    float longitude;
    int stats[2];
    struct city *next;
    struct city *prev;
} city_node;

typedef struct head_cities {
    int cnt;
    city_node *first;
    city_node *last;
} cities_head;

regions_head* make_regions_head() {
    regions_head* head = NULL;
    head = (regions_head*)malloc(sizeof(regions_head));
    if (head != NULL) {
        head->cnt = 0;
        head->first = NULL;
        head->last = NULL;
    }
    return head;
}

cities_head* make_cities_head() {
    cities_head* head = NULL;
    head = (cities_head*)malloc(sizeof(cities_head));
    if (head != NULL) {
        head->cnt = 0;
        head->first = NULL;
        head->last = NULL;
    }
    return head;
}

region_node* make_region_node(char name[MSIZE], int id) {
    region_node* region = NULL;
    region = (region_node*)malloc(sizeof(region_node));
    if (region != NULL) {
        region->id = id;
        strcpy(region->name, name);
        region->next = NULL;
        region->prev = NULL;
    }
    return region;
}

region_node* find_region(regions_head* r_head, char str[MSIZE]) {
    int i = 0;
    region_node* current = NULL;
    current = r_head->first;
    while (i < r_head->cnt && strcmp(current->name, str) != 0) {
        current = current->next;
        i++;
    }
    if (i == r_head->cnt) current = NULL;
    return current;
}

city_node* make_city_node(regions_head* r_Head, char **str) {
    city_node* node = NULL;
    node = (city_node*)malloc(sizeof(city_node));
    if (node != NULL) {
        node->name = str[0];
        node->name[MSIZE - 1] = '\0';
        node->region = find_region(r_Head, str[1]);
        node->code = atoi(str[2]);
        node->latitude = atof(str[3]);
        node->longitude = atof(str[4]);
        node->stats[0] = atoi(str[5]);
        node->stats[1] = atoi(str[6]);
        free(str);
        node->next = NULL;
        node->prev = NULL;
    }
    return node;
}

void add_regions_node(regions_head* r_head, region_node* region) {
    r_head->cnt++;
    if (r_head->first == NULL) {
        r_head->first = region;
        r_head->last = region;
    } else {
        region->prev = r_head->last;
        r_head->last->next = region;
        r_head->last = region;
    }
}

void add_city_node(cities_head* head, city_node* node) {
    head->cnt++;
    if (head->first == NULL) {
        head->first = node;
        head->last = node;
    } else {
        node->prev = head->last;
        head->last->next = node;
        head->last = node;
    }
}

void free_regions_list(regions_head* r_head) {
    region_node *f_p, *s_p;
    f_p = r_head->first;

    while (f_p != r_head->last) {
        s_p = f_p->next;
        free(f_p);
        f_p = s_p;
    }
    free(f_p);
    free(r_head);
}

void free_city_node(city_node* node) {
    if (node != NULL) {
        free(node->name);
        node->name = NULL;
        free(node);
    }
}

void free_city_list(cities_head* head) {
    city_node *f_p, *s_p;
    f_p = head->first;

    while (f_p != NULL) {
        s_p = f_p->next;
        free_city_node(f_p);
        f_p = s_p;
    }

    free(head);
}

void trim(char str[MSIZE]) {
    int i, flag = 0;
    str[MSIZE - 1] = '\0';

    for (i = 0; str[i] != '\0' && !flag; i++) {
        if (str[i] == '\n' || str[i] == '\r') {
            str[i] = '\0';
            flag = 1;
        }
    }
}

char **split(char *str, int slen, char sep) {
    int count = 0;
    int i = 0;
    int start = 0;
    int j = 0;
    int wordlen = 0;
    char **result = NULL;
    char *new_str = NULL;
    int allocError = 0;
    for (i = 0; i < slen; i++) {
        if (str[i] == sep) count++;
    }
    count++;
    result = (char**)malloc(count * sizeof(char*));
    if (result == NULL) {
        perror("Memory allocation failed");
    } else {
        for (i = 0; i < slen; i++) {
            if (str[i] == sep || str[i] == '\0') {
                wordlen = i - start;
                new_str = (char*)malloc((wordlen + 1) * sizeof(char));
                if (new_str == NULL) {
                    perror("Memory allocation failed");
                    allocError = 1;
                    i = slen;
                } else {
                    strncpy(new_str, str + start, wordlen);
                    new_str[wordlen] = '\0';
                    result[j++] = new_str;
                    start = i + 1;
                }
            }
        }

        if (allocError) {
            for (i = 0; i < j; i++) {
                free(result[i]);
            }
            free(result);
            result = NULL;
        }
    }

    return result;
}

void read_regions(regions_head* r_head) {
    FILE *file;
    region_node* region;
    int n, count, i;
    char temp[MSIZE];
    region = NULL;
    n = count = 0;
    file = fopen("regions.csv", "r");
    if (file != NULL) {
        while(fgets(temp, MSIZE, file) != NULL) n++;
        rewind(file);
        for (i = 0; i < n; i++) {
            fgets(temp, MSIZE, file);
            trim(temp);
            region = make_region_node(temp, i+1);
            if (region != NULL) {
                add_regions_node(r_head, region);
                count++;
            }
        }
        fclose(file);
    } else {
        perror("Failed to open file");
    }

    if (count != n) {
        perror("Failed to read from file");
        free_regions_list(r_head);
    }
}

void read_cities(cities_head* head, regions_head* r_head) {
    FILE* file;
    city_node* node;
    int n, count, i, slen;
    char **splitArray;
    char temp[MSIZE];
    node = NULL;
    n = count = 0;
    file = fopen("cities.csv", "r");
    if (file != NULL) {
        while (fgets(temp, MSIZE, file) != NULL) n++;
        rewind(file);
        for (i = 0; i < n; i++, count++) {
            fgets(temp, MSIZE, file);
            slen = strlen(temp);
            trim(temp);
            splitArray = split(temp, slen, ',');
            if (splitArray != NULL) {
                node = make_city_node(r_head, splitArray);
                if (node != NULL) {
                    add_city_node(head, node);
                }
            }
        }
        fclose(file);
    } else {
        perror("Failed to open file");
    }
    if (count != n) {
        perror("Failed to read from file");
        free_city_list(head);
    }
}

void append_city(regions_head* r_head, cities_head* head) {
    char** splitArray;
    city_node* node = NULL;
    char data_str[MSIZE] = {0};
    char temp[MSIZE];
    int slen;

    printf("Enter city name: ");
    fgets(temp, MSIZE, stdin);
    trim(temp);
    strcat(data_str, temp);
    strcat(data_str, ",");

    printf("Enter region: ");
    fgets(temp, MSIZE, stdin);
    trim(temp);
    strcat(data_str, temp);
    strcat(data_str, ",");

    printf("Enter region code: ");
    fgets(temp, MSIZE, stdin);
    trim(temp);
    strcat(data_str, temp);
    strcat(data_str, ",");

    printf("Enter latitude: ");
    fgets(temp, MSIZE, stdin);
    trim(temp);
    strcat(data_str, temp);
    strcat(data_str, ",");

    printf("Enter longitude: ");
    fgets(temp, MSIZE, stdin);
    trim(temp);
    strcat(data_str, temp);
    strcat(data_str, ",");

    printf("Enter area: ");
    fgets(temp, MSIZE, stdin);
    trim(temp);
    strcat(data_str, temp);
    strcat(data_str, ",");

    printf("Enter population: ");
    fgets(temp, MSIZE, stdin);
    trim(temp);
    strcat(data_str, temp);
    trim(data_str);

    slen = strlen(data_str);
    splitArray = split(data_str, slen + 1, ',');
    if (splitArray != NULL) {
        node = make_city_node(r_head, splitArray);
        add_city_node(head, node);
    }
}

void move_regions_head(regions_head* r_head){
    region_node* current = r_head->first;
    int id;
    printf("\nEnter element id (head will be moved before this element): ");
    scanf("%d", &id);
    getchar();
    while (current->id != id && current != r_head->last){
        current = current->next;
    }
    r_head->first = current;
    r_head->last = current->prev;
}

void print_regions_list(regions_head* r_head){
    region_node* current;
    int i;
    printf("|%3s|%15s|\n", "Id", "Region name");
    printf("+---+---------------+\n");
    current = r_head->first;
    for(i = 0; i < r_head->cnt; i++) {
        printf("|%3d|%15s|\n", current->id, current->name);
        current = current->next;
    }
    printf("+---+---------------+\n\n");
}

void print_city_node(city_node* node) {
    char temp[MSIZE] = "undefined";
    if (node->region != NULL) {
        strcpy(temp, node->region->name);
    }
    printf("|%20s|%15s|%12d|%10.1f|%10.1f|%10d|%15d|\n", node->name, temp, node->code, node->latitude, node->longitude, node->stats[0], node->stats[1]);
}

void print_cities_list(cities_head* head) {
    city_node* current;
    printf("|%20s|%15s|%12s|%10s|%10s|%10s|%15s|\n", "City name", "Region", "Region code", "Latitude", "Longitude", "Area", "Population");
    printf("+--------------------+---------------+------------+----------+----------+----------+---------------+\n");
    current = head->first;
    while (current != NULL) {
        print_city_node(current);
        current = current->next;
    }
    printf("+--------------------+---------------+------------+----------+----------+----------+---------------+\n");
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
    int answer;
    regions_head* r_head = NULL;
    cities_head* head = NULL;

    r_head = make_regions_head();
    head = make_cities_head();

    read_regions(r_head);
    read_cities(head, r_head);

    r_head->first->prev = r_head->last;
    r_head->last->next = r_head->first;
    do {
        clear_console();
        print_regions_list(r_head);
        print_cities_list(head);
        printf("\nEnter '1' to add new city\nEnter '2' to move regions' head\nEnter '0' to exit program\n");
        scanf("%d", &answer);
        getchar();
        clear_console();
        if (answer == 1) {
            print_cities_list(head);
            append_city(r_head, head);
        } else if (answer == 2){
            print_regions_list(r_head);
            move_regions_head(r_head);
        }
    } while (answer != 0);
    free_regions_list(r_head);
    free_city_list(head);
    return 0;
}
