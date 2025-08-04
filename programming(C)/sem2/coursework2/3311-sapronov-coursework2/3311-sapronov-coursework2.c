#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MSIZE 256

typedef struct city_regions{
    int id;
    char name[MSIZE];
    struct city_regions* next;
    struct city_regions* prev;
} region_node;

typedef struct head_city_regions{
    int count;
    struct city_regions* first;
    struct city_regions* last;
} region_head;

typedef struct city {
    int id;
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
    int count;
    city_node *first;
    city_node *last;
} city_head;

region_head* make_region_head();
region_node* make_region_node(char name[MSIZE]);
city_head* make_city_head();
city_node* make_city_node();

void menu(region_head* cHead, city_head* rHead);
int menuInput();
void options(region_head* rHead, city_head* cHead, int option);
int startsWithIgnoreCase(const char *str, const char *prefix);
void pressEnterToContinue();
void clearStdin();
void clearConsole();

void printAllRegions(region_head* rHead);
void printAllCities(city_head* cHead);
void printRegion(region_node *region);
void printCity(city_node *city);
void trimForDisplay(char *output, const char *input, int mlen);
void nullString(char str[MSIZE]);
void trim(char str[MSIZE]);
char **split(char *str, int length, char sep);

void readRegionsFile(char* filename, region_head* rHead);
void readCitiesFile(char* filename, city_head* cHead, region_head* rHead);
void writeRegionsToFile(region_head* rHead, const char* filename);
void writeCitiesToFile(city_head* cHead, const char* filename);
void freeRegionsList(region_head* rHead);
void freeCity(city_node* city);
void freeCitiesList(city_head* cHead);

void appendRegionNode(region_head* r_head, region_node* region);
void appendCityNode(city_head* cHead, city_node* city);
void deleteRegionNode(region_head* rHead, city_head* cHead, region_node* region);
void deleteCityNode(city_head* cHead, city_node* city);
void deleteRegionGUI(region_head* rHead, city_head* cHead);
void deleteCityGUI(city_head* cHead);
void addRegionGUI(region_head* rHead);
void addCityGUI(region_head* rHead, city_head* cHead);
void fillCityNode(region_head* rHead, city_head* cHead, city_node* city, char** str);
void clearRegionListGUI(region_head* rHead, city_head* cHead);
void clearCityListGUI(city_head* cHead);
void clearCityRegionById(city_head* cHead, int id);
city_node* findCityById(city_head* cHead, int id);
void filterCitiesGUI(region_head* rHead, city_head* cHead);
void filterCitiesByName(city_head* cHead);
void filterCitiesByRegionId(city_head* cHead, region_head* rHead);
void filterCitiesByCode(city_head* cHead);
void filterCitiesByLatitude(city_head* cHead);
void filterCitiesByLongitude(city_head* cHead);
void filterCitiesByArea(city_head* cHead);
void filterCitiesByPopulation(city_head* cHead);
void sortCitiesGUI(city_head* cHead);
void sortCities(city_head* cHead, int option);
int compareCities(city_node* a, city_node* b, int option);
region_node* findRegionByName(region_head* rHead, char name[MSIZE]);
region_node* findRegionById(region_head* rHead, int id);
void updateCityDataGUI(region_head* rHead, city_head* cHead);
void editCityName(city_node* city);
void editCityRegion(region_head* rHead, city_node* city);
void editCityCode(city_node* city);
void editCityLatitude(city_node* city);
void editCityLongitude(city_node* city);
void editCityArea(city_node* city);
void editCityPopulation(city_node* city);

int main() {
    city_head* cHead = NULL;
    region_head* rHead = NULL;
    cHead = make_city_head();
    rHead = make_region_head();
    /*start if heads created successfully*/
    if (cHead != NULL && rHead != NULL) {
        menu(rHead, cHead);
    } else {
        puts("Memory allocation error\n");
    }
    return 0;
}

region_head* make_region_head() {
    region_head* rHead = NULL;
    rHead = (region_head*)malloc(sizeof(region_head));
    /*memory allocation check*/
    if (rHead != NULL) {
        rHead->count = 0;
        rHead->first = NULL;
        rHead->last = NULL;
    } else {
        perror("Memory allocation failed");
    }

    return rHead;
}

region_node* make_region_node(char name[MSIZE]) {
    region_node* region = NULL;
    region = (region_node*)malloc(sizeof(region_node));
    /*memory allocation check*/
    if (region != NULL) {
        region->id = 0;
        strcpy(region->name, name);
        region->next = NULL;
        region->prev = NULL;
    }

    return region;
}

city_head* make_city_head() {
    city_head* cHead = NULL;
    cHead = (city_head*)malloc(sizeof(city_head));
    /*memory allocation check*/
    if (cHead != NULL) {
        cHead->count = 0;
        cHead->first = NULL;
        cHead->last = NULL;
    } else {
        perror("Memory allocation failed");
    }

    return cHead;
}

city_node* make_city_node() {
    city_node* city = NULL;
    city = (city_node*)malloc(sizeof(city_node));
    /*memory allocation check*/
    if (city != NULL) {
        city->id = 0;
        city->name = NULL;
        city->region = NULL;
        city->code = 0;
        city->latitude = 0;
        city->longitude = 0;
        city->stats[0] = 0;
        city->stats[1] = 0;
        city->next = NULL;
        city->prev = NULL;
    }
    return city;
}

void menu(region_head* rHead, city_head* cHead) {
    int option, saveOption;
    /*fill lists from files*/
    readRegionsFile("regions.csv", rHead);
    readCitiesFile("cities.csv", cHead, rHead);
    /*print menu and get input until exit option is chosen*/
    do {
        clearConsole();
        option = menuInput();
        if (option != 0) {
            /*print menu for chosen option*/
            options(rHead, cHead, option);
        } else {
            /*user chooses to save changes or not*/
            saveOption = -1;
            printf("\nDo you want to save changes? (1 - yes, 0 - no): ");
            scanf("%d", &saveOption);
            clearStdin();
            if (saveOption == 1) {
                /*rewrite starting files*/
                writeCitiesToFile(cHead, "cities.csv");
                writeRegionsToFile(rHead, "regions.csv");
                puts("\nChanges successfully saved\n");
            } else if (saveOption != 0) {
                option = -1;
                puts("Wrong input - must be 0 or 1.\n");
            }
            pressEnterToContinue();
            clearConsole();
        }
    } while (option != 0);
    /*free allocated memory*/
    freeRegionsList(rHead);
    freeCitiesList(cHead);
}

int menuInput() {
    int answer;
    puts("Choose an option:");
    puts("1.  Print all regions");
    puts("2.  Print all cities");
    puts("3.  Add new region");
    puts("4.  Add new city");
    puts("5.  Update city data");
    puts("6.  Filter cities");
    puts("7.  Sort cities");
    puts("8.  Delete region");
    puts("9.  Delete city");
    puts("10. Clear regions list");
    puts("11. Clear cities list");
    puts("0.  Exit");
    printf("> ");
    /*get user answer and return it*/
    scanf("%d", &answer);
    clearStdin();
    return answer;
}

void options(region_head* rHead, city_head* cHead, int option) {
    clearConsole();
    /*call function based on user choice*/
    switch (option) {
        case 1:
            puts("1.  Print all regions\n");
            printAllRegions(rHead);
            break;
        case 2:
            puts("2.  Print all cities\n");
            printAllCities(cHead);
            break;
        case 3:
            puts("3.  Add new region\n");
            addRegionGUI(rHead);
            break;
        case 4:
            puts("4.  Add new city\n");
            addCityGUI(rHead, cHead);
            break;
        case 5:
            puts("5.  Update city data\n");
            updateCityDataGUI(rHead, cHead);
            break;
        case 6:
            puts("6.  Filter cities\n");
            filterCitiesGUI(rHead, cHead);
            break;
        case 7:
            puts("7.  Sort cities\n");
            sortCitiesGUI(cHead);
            break;
        case 8:
            puts("8.  Delete region\n");
            deleteRegionGUI(rHead, cHead);
            break;
        case 9:
            puts("9.  Delete city\n");
            deleteCityGUI(cHead);
            break;
        case 10:
            puts("10. Clear region list\n");
            clearRegionListGUI(rHead, cHead);
            break;
        case 11:
            puts("11. Clear city list\n");
            clearCityListGUI(cHead);
            break;
        default:
            clearConsole();
            puts("Invalid option.");
            break;
    }
    pressEnterToContinue();
}

void pressEnterToContinue() {
    /*convenient transition between menus*/
    printf("\nPress ENTER to continue ");
    clearStdin();
    clearConsole();
}

int startsWithIgnoreCase(const char *str, const char *prefix) {
    /*initialize flag for prefix check*/
    int isPrefix = 1;
    /*loop through characters of both the string and the prefix until either the end of one is reached or mismatch found*/
    while (*str && *prefix && isPrefix) {
        if (tolower(*str) != tolower(*prefix)) {
            /* If characters don't match, set flag to 0*/
            isPrefix = 0;
        }
        /*move to next character*/
        str++;
        prefix++;
    }
    /*check if end of prefix reached*/
    if (*prefix != '\0') {
        isPrefix = 0;
    }
    /*return 1 if string starts with prefix, 0 if not*/
    return isPrefix;
}

void clearStdin() {
    int c;
    /*clear input buffer*/
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void clearConsole() {
    /*clear screen depending on operation system*/
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

void printAllRegions(region_head* r_head) {
    region_node *current;
    /*print header*/
    printf("|%3s|%15s|\n", "Id", "Region name");
    puts("+---+---------------+");

    current = r_head->first;
    /*print regions until end of list reached*/
    while (current != NULL) {
        printRegion(current);
        current = current->next;
    }
    puts("+---+---------------+\n");
}

void printAllCities(city_head* cHead) {
    city_node *current;
    /*print header*/
    printf("|%5s|%20s|%15s|%12s|%10s|%10s|%10s|%15s|\n", "ID", "City name", "Region", "Region code", "Latitude", "Longitude", "Area", "Population");
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");

    current = cHead->first;
    /*print regions until end of list reached*/
    while (current != NULL) {
        printCity(current);
        current = current->next;
    }
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
}

void printRegion(region_node *region) {
    char trimmedName[15];
    /*shorten name to fit in the table*/
    trimForDisplay(trimmedName, region->name, 15);
    printf("| %-1d | %-13s |\n", region->id, trimmedName);
}

void printCity(city_node *city) {
    char region[MSIZE] = "undefined";
    char trimmedName[23], trimmedRegion[17];
    /*shorten names to fit in the table*/
    if (city->region != NULL) {
        trimForDisplay(region, city->region->name, sizeof(region));
    }
    trimForDisplay(trimmedName, city->name, 22);
    trimForDisplay(trimmedRegion, region, 16);

    printf("| %-3d | %-18s | %-13s | %-10d | %-8.2f | %-8.2f | %-8d | %-13d |\n",
           city->id, trimmedName, trimmedRegion, city->code, city->latitude, city->longitude, city->stats[0], city->stats[1]);
}

void trimForDisplay(char *output, const char *input, int mlen) {
    /* compare input string length to length allowed for display*/
    if (strlen(input) > mlen) {
        /*trim string and put ... in the end*/
        strncpy(output, input, mlen - 3);
        output[mlen - 3] = '\0';
        strcat(output, "...");
    } else {
        /*string remains the same*/
        strcpy(output, input);
    }
}

void nullString(char str[MSIZE]) {
    int i;
    /*nullify every string character*/
    for (i = 0; i < MSIZE; i++) {
        str[i] = '\0';
    }
}

void trim(char str[MSIZE]) {
    int i, flag = 0;
    str[MSIZE - 1] = '\0';
    /*check if end of string reached*/
    for (i = 0; str[i] != '\0' && !flag; i++) {
        /*if end of string found, set flag to leave loop*/
        if (str[i] == '\n' || str[i] == '\r') {
            str[i] = '\0';
            flag = 1;
        }
    }
}

char **split(char *str, int length, char sep) {
    int count = 0;
    int i = 0;
    int start = 0;
    int j = 0;
    int wordLen = 0;
    char **result = NULL;
    char *newStr = NULL;
    int allocError = 0;
    /*count number of words*/
    for (i = 0; i < length; i++) {
        if (str[i] == sep) count++;
    }
    count++;
    /*allocate memory and check for success*/
    result = malloc(count * sizeof(char *));
    if (result == NULL) {
        perror("Memory allocation failed");
    } else {
        /*loop through string*/
        for (i = 0; i < length; i++) {
            /*check for separator or end of string*/
            if (str[i] == ';' || str[i] == '\0') {
                /*calculate word length, allocate memory and check for success*/
                wordLen = i - start;
                newStr = malloc((wordLen + 1) * sizeof(char));
                if (newStr == NULL) {
                    perror("Memory allocation failed");
                    allocError = 1;
                    i = length;
                } else {
                    /*save string in word list*/
                    strncpy(newStr, str + start, wordLen);
                    newStr[wordLen] = '\0';
                    result[j++] = newStr;
                    /*update index for next word*/
                    start = i + 1;
                }
            }
        }
        /*free strings if memory not allocated*/
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

void readRegionsFile(char* filename, region_head* rHead) {
    FILE* file;
    region_node* region;
    int n, count, i;
    char temp[MSIZE];
    region = NULL;
    n = count = 0;
    file = fopen(filename, "r");
    /*check if file opened successfully*/
    if (file != NULL) {
        /*count lines in file*/
        while ((fgets(temp, MSIZE, file)) != NULL) n++;
        rewind(file);
        /*read lines*/
        for (i = 0; i < n; i++) {
            /*clear temp string*/
            nullString(temp);
            /*read string in temp and trim it*/
            fgets(temp, MSIZE, file);
            trim(temp);
            /*create new node and check for success*/
            region = make_region_node(temp);
            if (region != NULL) {
                /*add new node to list and increase count*/
                appendRegionNode(rHead, region);
                count++;
            }
        }
        fclose(file);
    } else {
        perror("Failed to open file");
    }
    /*if read less lines than in file, free region list*/
    if (count != n) {
        perror("Failed to read from file");
        freeRegionsList(rHead);
    }
}

void readCitiesFile(char* filename, city_head* cHead, region_head* rHead) {
    FILE* file;
    city_node* city;
    int n, count, i, slen;
    char** splitArray;
    char temp[MSIZE];

    city = NULL;
    n = count = 0;
    file = fopen(filename, "r");
    /*check if file opened successfully*/
    if (file != NULL) {
        /*count lines in file*/
        while ((fgets(temp, MSIZE, file)) != NULL) n++;
        rewind(file);
        /*read lines*/
        for (i = 0; i < n; i++, count++) {
            /*clear temp string*/
            nullString(temp);
            /*read string in temp and trim it*/
            fgets(temp, MSIZE, file);
            slen = strlen(temp);
            trim(temp);
            /*split string and check for success*/
            splitArray = split(temp, slen, ';');
            if (splitArray != NULL) {
                /*create new node and check for success*/
                city = make_city_node();
                if (city != NULL) {
                    /*fill new node and add to list*/
                    fillCityNode(rHead, cHead, city, splitArray);
                    appendCityNode(cHead, city);
                }
            }
        }
        fclose(file);
    } else {
        perror("Failed to open file");
    }
    /*if read less lines than in file, free city list*/
    if (count != n) {
        perror("Failed to read from file");
        freeCitiesList(cHead);
    }
}

void writeRegionsToFile(region_head* rHead, const char* filename) {
    FILE* file = fopen(filename, "w");
    region_node* current = NULL;
    /*check if file opened successfully*/
    if (file != NULL) {
        current = rHead->first;
        /*rewrite file*/
        while (current != NULL) {
            fprintf(file, "%s\n", current->name);
            current = current->next;
        }

        fclose(file);
    } else {
        perror("Failed to open file");
    }
}

void writeCitiesToFile(city_head* cHead, const char* filename) {
    FILE* file = fopen(filename, "w");
    city_node* current = NULL;
    char* regionName;
    /*check if file opened successfully*/
    if (file != NULL) {
        current = cHead->first;
        /*rewrite file*/
        while (current != NULL) {
            regionName = "undefined";
            if (current->region != NULL) {
                regionName = current->region->name;
            }
            fprintf(file, "%s;%s;%d;%.2f;%.2f;%d;%d\n", current->name, regionName, current->code,
                     current->latitude, current->longitude, current->stats[0], current->stats[1]);
            current = current->next;
        }

        fclose(file);
    } else {
        printf("Failed to open file %s\n", filename);
    }
}

void freeRegionsList(region_head* rHead) {
    region_node *current, *current1;
    current = rHead->first;
    /*free list*/
    while (current != NULL) {
        current1 = current->next;
        free(current);
        current = current1;
    }
    free(rHead);
}

void freeCity(city_node* city) {
    /*free node fields*/
    if (city->name != NULL) {
        free(city->name);
        city->name = NULL;
    }
    if (city->region != NULL) {
        city->region = NULL;
    }
    free(city);
}

void freeCitiesList(city_head* cHead) {
    city_node *current = NULL, *current1 = NULL;
    current = cHead->first;
    /*free list*/
    while (current != NULL) {
        current1 = current->next;
        freeCity(current);
        current = current1;
    }
    free(cHead);
}

void appendRegionNode(region_head* r_head, region_node* region) {
    r_head->count++;
    /*check if list is empty*/
    if (r_head->first == NULL) {
        /*new node is first*/
        r_head->first = region;
        r_head->last = region;
        region->id = 1;

    } else  {
        /*new node is last*/
        region->id = r_head->last->id + 1;
        region->prev = r_head->last;
        r_head->last->next = region;
        r_head->last = region;
    }
}

void appendCityNode(city_head* cHead, city_node* city) {
    cHead->count++;
    /*check if list is empty*/
    if (cHead->first == NULL) {
        /*new node is first*/
        cHead->first = city;
        cHead->last = city;
        city->id = 1;

    } else  {
        /*new node is lasr*/
        city->id = cHead->last->id + 1;
        city->prev = cHead->last;
        cHead->last->next = city;
        cHead->last = city;
    }
}

void deleteRegionNode(region_head* rHead, city_head* cHead, region_node* region) {
    /*check if node is first or last*/
    if (rHead->first == region) {
        /*second node is now first*/
        rHead->first = region->next;
        if (region->next != NULL) {
            region->next->prev = region->prev;
        }
    } else if (rHead->last == region) {
        /*prelast node is now last*/
        rHead->last = region->prev;
        if (region->prev != NULL) {
            region->prev->next = region->next;
        }
    } else {
        /*changing pointers of neighboring nodes*/
        if (region->prev != NULL) {
            region->prev->next = region->next;
        }
        if (region->next != NULL) {
            region->next->prev = region->prev;
        }
    }
    /*delete region in city list*/
    clearCityRegionById(cHead, region->id);
    free(region);
    rHead->count--;
}

void deleteCityNode(city_head* cHead, city_node* city) {
    /*check if node is first or last*/
    if (cHead->first == city) {
        /*second node is now first*/
        cHead->first = city->next;
        if (city->next != NULL) {
            city->next->prev = city->prev;
        }
    } else if (cHead->last == city) {
        /*prelast node is now last*/
        cHead->last = city->prev;
        if (city->prev != NULL) {
            city->prev->next = city->next;
        }
    } else {
        /*changing pointers of neighboring nodes*/
        if (city->prev != NULL) {
            city->prev->next = city->next;
        }
        if (city->next != NULL) {
            city->next->prev = city->prev;
        }
    }
    freeCity(city);
    cHead->count--;
}

void deleteRegionGUI(region_head* rHead, city_head* cHead) {
    int id;
    region_node* region = NULL;
    /*check if list is empty*/
    if (rHead->first != NULL) {
        printAllRegions(rHead);
        printf("\nEnter region id to delete (or 0 to return to menu): ");
        scanf("%d", &id);
        clearStdin();
        /*check if id is in the list*/
        if (id > 0 && id <= rHead->count) {
            region = findRegionById(rHead, id);
            printf("\nRegion with id %d:\n", id);
            printf("|%3s|%15s|\n", "Id", "Region name");
            puts("+---+---------------+");
            printRegion(region);
            puts("+---+---------------+");
            deleteRegionNode(rHead, cHead, region);
            printf("\nRegion with id %d has been successfully removed.\n", id);
        } else if (id != 0) {
            puts("\nWrong ID input.");
        }
    } else {
        puts("List is empty.");
    }
}

void deleteCityGUI(city_head* cHead) {
    int id;
    city_node* city = NULL;
    /*check if list is empty*/
    if (cHead->first != NULL) {
        printAllCities(cHead);
        printf("\nEnter city id to delete city (or 0 to return to menu): ");
        scanf("%d", &id);
        clearStdin();
        /*check if id is in the list*/
        if (id > 0 && id <= cHead->count) {
            city = findCityById(cHead, id);
            printf("\nCity with id %d:\n", id);
            printf("|%5s|%20s|%15s|%12s|%10s|%10s|%10s|%15s|\n", "ID", "City name", "Region", "Region code", "Latitude", "Longitude", "Area", "Population");
            puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
            printCity(city);
            puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
            deleteCityNode(cHead, city);
            printf("\nCity with id %d has been successfully removed.\n", id);
        } else if (id != 0) {
            puts("\nWrong ID input.");
        }
    } else {
        puts("List is empty.");
    }
}

void addRegionGUI(region_head* rHead) {
    char temp[MSIZE];
    region_node* region = NULL;

    printf("Enter region name: ");
    /*check if name was entered*/
    if (fgets(temp, MSIZE, stdin) != NULL) {
        trim(temp);
        /*create new node and check for success*/
        region = make_region_node(temp);
        if (region != NULL) {
            /*add node to end of list and print it*/
            appendRegionNode(rHead, region);
            puts("\nRegion successfully added.\n");
            printf("|%3s|%15s|\n", "Id", "Region name");
            puts("+---+---------------+");
            printRegion(region);
            puts("+---+---------------+");
        } else {
            puts("\nFailed: memory error");
        }
    } else {
        puts("\nFailed: memory error");
    }
}

void addCityGUI(region_head* rHead, city_head* cHead) {
    city_node* city = NULL;
    /*create new node and check for success*/
    city = make_city_node();
    if (city != NULL) {
        puts("Enter information for new city:\n");
        /*entering all fields*/
        editCityName(city);
        editCityRegion(rHead, city);
        editCityCode(city);
        editCityLatitude(city);
        editCityLongitude(city);
        editCityArea(city);
        editCityPopulation(city);
        /*add node to end of list*/
        appendCityNode(cHead, city);
        puts("\nCity has been successfully added.");
        printf("|%5s|%20s|%15s|%12s|%10s|%10s|%10s|%15s|\n", "ID", "City name", "Region", "Region code", "Latitude", "Longitude", "Area", "Population");
        puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
        printCity(city);
        puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
    } else {
        perror("\nFailed: memory error\n");
    }
}

void fillCityNode(region_head* rHead, city_head* cHead, city_node* city, char** str) {
    if (city != NULL) {
        /*filling node with data in strings list*/
        city->name = str[0];
        city->region = findRegionByName(rHead, str[1]); /*fill with existing region name or undefined*/
        free(str[1]);
        city->code = atoi(str[2]) ; /*convert to int type from string*/
        free(str[2]);
        city->latitude = atof(str[3]); /*convert to float type from string*/
        free(str[3]);
        city->longitude = atof(str[4]);
        free(str[4]);
        city->stats[0] = atoi(str[5]);
        free(str[5]);
        city->stats[1] = atoi(str[6]);
        free(str[6]);
        free(str);

        city->next = NULL;
        city->prev = NULL;
    } else {
        perror("Memory allocation failed");
    }
}

void clearRegionListGUI(region_head* rHead, city_head* cHead) {
    region_node *current, *current1;
    city_node* city;
    /*check if list is empty*/
    current = rHead->first;
    if (current == NULL) {
        puts("List is empty.");
    } else {
        /*free list*/
        while (current != NULL) {
            current1 = current->next;
            free(current);
            current = current1;
        }
        /*delete all region pointers in city nodes*/
        city = cHead->first;
        while (city != NULL) {
            city->region = NULL;
            city = city->next;
        }
        /*clear head*/
        rHead->first = NULL;
        rHead->last = NULL;
        rHead->count = 0;
        puts("List successfully cleared.");
    }
}

void clearCityListGUI(city_head* cHead) {
    city_node* current, *current1;
    /*check if list is empty*/
    if (cHead->first != NULL) {
        /*free list*/
        current = cHead->first;
        while (current != NULL) {
            current1 = current->next;
            freeCity(current);
            current = current1;
        }
        /*clear head*/
        cHead->last = NULL;
        cHead->first = NULL;
        cHead->count = 0;
        puts("List successfully cleared.");
    } else {
        puts("List is empty.");
    }
}

void clearCityRegionById(city_head* cHead, int id) {
    city_node* current = NULL;

    current = cHead->first;
    while (current != NULL) {
        /*clear region pointer in city node if region id matches*/
        if (current->region != NULL && current->region->id == id) {
            current->region = NULL;
        }
        current = current->next;
    }
}

city_node* findCityById(city_head* cHead, int id) {
    city_node* current = NULL;
    current = cHead->first;
    /*loop until end of list reached or id matches*/
    while (current != NULL && current->id != id) {
        current = current->next;
    }
    return current;
}

void filterCitiesGUI(region_head* rHead, city_head* cHead) {
    int option;

    puts("1. Name");
    puts("2. Region");
    puts("3. Code");
    puts("4. Latitude");
    puts("5. Longitude");
    puts("6. Area");
    puts("7. Population");
    printf("\nEnter option: ");
    scanf("%d", &option);
    clearStdin();
    /*call function depending on user choice*/
    switch (option) {
        case 1:
            filterCitiesByName(cHead);
            break;
        case 2:
            filterCitiesByRegionId(cHead, rHead);
            break;
        case 3:
            filterCitiesByCode(cHead);
            break;
        case 4:
            filterCitiesByLatitude(cHead);
            break;
        case 5:
            filterCitiesByLongitude(cHead);
            break;
        case 6:
            filterCitiesByArea(cHead);
            break;
        case 7:
            filterCitiesByPopulation(cHead);
            break;
        default:
            puts("Wrong option.");
            break;
    }
}


void filterCitiesByName(city_head* cHead) {
    city_node *current;
    char name[MSIZE];
    /*name input and trim*/
    printf("Enter name: ");
    fgets(name, MSIZE, stdin);
    trim(name);
    printf("|%5s|%20s|%15s|%12s|%10s|%10s|%10s|%15s|\n", "ID", "City name", "Region", "Region code", "Latitude", "Longitude", "Area", "Population");
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
    current = cHead->first;
    /*print cities with names starting with entered string*/
    while (current != NULL) {
        if (startsWithIgnoreCase(current->name, name) == 1) {
            printCity(current);
        }
        current = current->next;
    }
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
}

void filterCitiesByRegionId(city_head* cHead, region_head* rHead) {
    city_node *current;
    int id;
    /*print region list and get region input*/
    printAllRegions(rHead);
    printf("Enter region id (0 to show cities with undefined region): ");
    scanf("%d", &id);
    clearStdin();
    printf("|%5s|%20s|%15s|%12s|%10s|%10s|%10s|%15s|\n", "ID", "City name", "Region", "Region code", "Latitude", "Longitude", "Area", "Population");
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
    current = cHead->first;
    /*print all cities with matching region id or all cities with undefined region if id is 0*/
    while (current != NULL) {
        if ((current->region != NULL && current->region->id == id) || (current->region == NULL && id == 0)) {
            printCity(current);
        }
        current = current->next;
    }
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
}

void filterCitiesByCode(city_head* cHead) {
    city_node *current;
    int minC, maxC;
    /*input of min and max values*/
    printf("Enter min code: ");
    scanf("%d", &minC);
    clearStdin();
    printf("Enter max code: ");
    scanf("%d", &maxC);
    clearStdin();
    printf("|%5s|%20s|%15s|%12s|%10s|%10s|%10s|%15s|\n", "ID", "City name", "Region", "Region code", "Latitude", "Longitude", "Area", "Population");
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
    current = cHead->first;
    /*print all cities with code in interval*/
    while (current != NULL) {
        if (current->code >= minC && current->code <= maxC) {
            printCity(current);
        }
        current = current->next;
    }
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
}

void filterCitiesByLatitude(city_head* cHead) {
    city_node *current;
    float minL, maxL;
    /*input of min and max values*/
    printf("Enter min latitude: ");
    scanf("%f", &minL);
    clearStdin();
    printf("Enter max latitude: ");
    scanf("%f", &maxL);
    clearStdin();
    printf("|%5s|%20s|%15s|%12s|%10s|%10s|%10s|%15s|\n", "ID", "City name", "Region", "Region code", "Latitude", "Longitude", "Area", "Population");
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
    current = cHead->first;
    /*print all cities with latitude in interval*/
    while (current != NULL) {
        if (current->latitude >= minL && current->latitude <= maxL) {
            printCity(current);
        }
        current = current->next;
    }
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
}

void filterCitiesByLongitude(city_head* cHead) {
    city_node *current;
    float minL, maxL;
    /*input of min and max values*/
    printf("Enter min longitude: ");
    scanf("%f", &minL);
    clearStdin();
    printf("Enter max longitude: ");
    scanf("%f", &maxL);
    clearStdin();
    printf("|%5s|%20s|%15s|%12s|%10s|%10s|%10s|%15s|\n", "ID", "City name", "Region", "Region code", "Latitude", "Longitude", "Area", "Population");
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
    current = cHead->first;
    /*print all cities with longitude in interval*/
    while (current != NULL) {
        if (current->longitude >= minL && current->longitude <= maxL) {
            printCity(current);
        }
        current = current->next;
    }
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
}

void filterCitiesByArea(city_head* cHead) {
    city_node *current;
    int minA, maxA;
    /*input of min and max values*/
    printf("Enter min area: ");
    scanf("%d", &minA);
    clearStdin();
    printf("Enter max area: ");
    scanf("%d", &maxA);
    clearStdin();
    printf("|%5s|%20s|%15s|%12s|%10s|%10s|%10s|%15s|\n", "ID", "City name", "Region", "Region code", "Latitude", "Longitude", "Area", "Population");
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
    current = cHead->first;
    /*print all cities with area in interval*/
    while (current != NULL) {
        if (current->stats[0] >= minA && current->stats[0] <= maxA) {
            printCity(current);
        }
        current = current->next;
    }
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
}

void filterCitiesByPopulation(city_head* cHead) {
    city_node *current;
    int minP, maxP;
    /*input of min and max values*/
    printf("Enter min population: ");
    scanf("%d", &minP);
    clearStdin();
    printf("Enter max population: ");
    scanf("%d", &maxP);
    clearStdin();
    printf("|%5s|%20s|%15s|%12s|%10s|%10s|%10s|%15s|\n", "ID", "City name", "Region", "Region code", "Latitude", "Longitude", "Area", "Population");
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
    current = cHead->first;
    /*print all cities with population in interval*/
    while (current != NULL) {
        if (current->stats[1] >= minP && current->stats[1] <= maxP) {
            printCity(current);
        }
        current = current->next;
    }
    puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
}

void sortCitiesGUI(city_head* cHead) {
    int option;
    /*check if list is empty*/
    if (cHead->first != NULL) {
        /*print menu and get user input*/
        puts("1. Sort by id");
        puts("2. Sort by name");
        puts("3. Sort by region");
        puts("4. Sort by latitude");
        puts("5. Sort by longitude");
        puts("6. Sort by area");
        puts("7. Sort by population");
        printf("Enter option: ");
        scanf("%d", &option);
        clearStdin();
        /*sort if option was in the menu*/
        if (option > 0 && option <= 7) {
            sortCities(cHead, option);
            puts("List successfully sorted");
        } else {
            puts("Wrong option");
        }
    } else {
        puts("List is empty");
    }
}

void sortCities(city_head* cHead, int option) {
    city_node* sorted = NULL;
    city_node* current = cHead->first;
    city_node* next = NULL;
    city_node* temp = NULL;
    /*check if list includes more than two elements*/
    if (cHead->first != NULL && cHead->first->next != NULL) {
        while (current != NULL) {
            next = current->next;
            if (sorted == NULL || compareCities(current, sorted, option) < 0) {
                /*Insert element in the beginning*/
                current->next = sorted;
                if (sorted != NULL) sorted->prev = current;
                sorted = current;
                sorted->prev = NULL;
            } else {
                /* Find the insertion point within the sorted list*/
                temp = sorted;
                while (temp->next != NULL && compareCities(current, temp->next, option) > 0) {
                    temp = temp->next;
                }
                /*insert element*/
                current->next = temp->next;
                if (temp->next != NULL) temp->next->prev = current;
                temp->next = current;
                current->prev = temp;
            }
            current = next;
        }
        /*update list head*/
        cHead->first = sorted;
        temp = sorted;
        while (temp != NULL && temp->next != NULL) {
            temp = temp->next;
        }
        cHead->last = temp;
    }
}

int compareCities(city_node* a, city_node* b, int option) {
    int result;
    /*result calculated based on chosen field*/
    switch (option) {
        case 1:
            result =  a->id - b->id;
            break;
        case 2:
            result = strcmp(a->name, b->name);
            break;
        case 3:
            result = a->code - b->code;
            break;
        case 4:
            result = (a->latitude > b->latitude) ? 1 : (a->latitude < b->latitude) ? -1 : 0;
            break;
        case 5:
            result = (a->longitude > b->longitude) ? 1 : (a->longitude < b->longitude) ? -1 : 0;
            break;
        case 6:
            result = a->stats[0] - b->stats[0];
            break;
        case 7:
            result = a->stats[1] - b->stats[1];
            break;
        default:
            result = 0;
            break;
    }
    /*returns 0 if a equal to b, positive if a is greater than b, negative if lower*/
    return result;
}

region_node* findRegionByName(region_head* rHead, char name[MSIZE]) {
    region_node* current = NULL;
    current = rHead->first;
    /*search and return region with same name*/
    while (current != NULL && strcmp(current->name, name) != 0) {
        current = current->next;
    }
    return current;
}

region_node* findRegionById(region_head* rHead, int id) {
    region_node* current = NULL;
    current = rHead->first;
    /*search and return region with same id*/
    while (current != NULL && current->id != id) {
        current = current->next;
    }
    return current;
}

void updateCityDataGUI(region_head* rHead, city_head* cHead) {
    city_node* city;
    int cityId, option;
    /*user choose city to update*/
    printAllCities(cHead);
    printf("Enter city id: ");
    scanf("%d", &cityId);
    clearStdin();
    city = findCityById(cHead, cityId);
    /*check if city with such id found*/
    if (city != NULL) {
        clearConsole();
        /*print city to update*/
        puts("Update city data\n");
        printf("|%5s|%20s|%15s|%12s|%10s|%10s|%10s|%15s|\n", "ID", "City name", "Region", "Region code", "Latitude", "Longitude", "Area", "Population");
        puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
        printCity(city);
        puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
        /*print menu and get input*/
        puts("\nWhich field would you like to edit?\n");
        puts("1. Name");
        puts("2. Region");
        puts("3. Code");
        puts("4. Latitude");
        puts("5. Longitude");
        puts("6. Area");
        puts("7. Population");
        puts("8. All fields");
        printf("\nEnter option: ");
        scanf("%d", &option);
        clearStdin();
        clearConsole();
        /*updates field based on users choice*/
        switch (option) {
            case 1:
                puts("1. Name\n");
                editCityName(city);
                break;
            case 2:
                puts("2. Region\n");
                editCityRegion(rHead, city);
                break;
            case 3:
                puts("3. Code\n");
                editCityCode(city);
                break;
            case 4:
                puts("4. Latitude\n");
                editCityLatitude(city);
                break;
            case 5:
                puts("5. Longitude\n");
                editCityLongitude(city);
                break;
            case 6:
                puts("6. Area\n");
                editCityArea(city);
                break;
            case 7:
                puts("7. Population\n");
                editCityPopulation(city);
                break;
            case 8:
                puts("8. All fields\n");
                editCityName(city);
                editCityRegion(rHead, city);
                editCityCode(city);
                editCityLatitude(city);
                editCityLongitude(city);
                editCityArea(city);
                editCityPopulation(city);
                break;
            default:
                puts("\nWrong option.");
                break;
        }
        /*print updated city*/
        puts("\nUpdated city:\n");
        printf("|%5s|%20s|%15s|%12s|%10s|%10s|%10s|%15s|\n", "ID", "City name", "Region", "Region code", "Latitude", "Longitude", "Area", "Population");
        puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
        printCity(city);
        puts("+-----+--------------------+---------------+------------+----------+----------+----------+---------------+");
    } else {
        puts("\nCity not found\n");
    }
}



void editCityName(city_node* city) {
    char temp[MSIZE];

    printf("Enter city name: ");
    /*get input of new name and check for success*/
    if (fgets(temp, MSIZE, stdin) != NULL) {
        trim(temp);
        /*delete old name*/
        if (city->name != NULL) {
            free(city->name);
            city->name = NULL;
        }
        /*memory allocation and name update*/
        city->name = (char*)malloc(strlen(temp) + 1);
        if (city->name != NULL) {
            strcpy(city->name, temp);
        } else {
            puts("Memory error\n");
        }
    } else {
        puts("Memory error\n");
    }
}

void editCityRegion(region_head* rHead, city_node* city) {
    region_node* region;
    int success;
    int regionId;
    /*check if list is empty*/
    if (rHead->first == NULL) {
        puts("Region list is empty.");
    } else {
        /*get region id from region list*/
        printAllRegions(rHead);
        printf("Enter region id: ");
        success = scanf("%d", &regionId);
        clearStdin();
        if (success != 1) {
            regionId = 0;
        }
        region = findRegionById(rHead, regionId);
        /*check if region found and update the field*/
        if (region != NULL) {
            city->region = region;
        } else {
            puts("Region not found");
        }
    }
}

void editCityCode(city_node* city) {
    int code;
    int success;
    /*get new value*/
    printf("Enter city code: ");
    success = scanf("%d", &code);
    clearStdin();
    /*validate input and update the field*/
    if (code < 0 || code > 95 || success != 1) {
        puts("Invalid or impossible code");
    } else {
        city->code = code;
    }
}

void editCityLatitude(city_node* city) {
    float latitude;
    int success;
    /*get new value*/
    printf("Enter city latitude: ");
    success = scanf("%f", &latitude);
    clearStdin();
    if (latitude < -90 || latitude > 90 || success != 1) {
        puts("Invalid or impossible latitude");
    } else {
        city->latitude = latitude;
    }
}

void editCityLongitude(city_node* city) {
    float longitude;
    int success;
    /*get new value*/
    printf("Enter city longitude: ");
    success = scanf("%f", &longitude);
    clearStdin();
    /*validate input and update the field*/
    if (longitude < -180 || longitude > 180 || success != 1) {
        puts("Invalid or impossible longitude");
    } else {
        city->longitude = longitude;
    }
}

void editCityArea(city_node* city) {
    int area;
    int success;
    /*get new value*/
    printf("Enter city area: ");
    success = scanf("%d", &area);
    clearStdin();
    /*validate input and update the field*/
    if (area < 1 || area > 100000 || success != 1) {
        puts("Invalid or impossible area");
    } else {
        city->stats[0] = area;
    }
}

void editCityPopulation(city_node* city) {
    int population;
    int success;
    /*get new value*/
    printf("Enter city population: ");
    success = scanf("%d", &population);
    clearStdin();
    /*validate input and update the field*/
    if (population < 1 || population > 15000000 || success != 1) {
        puts("Invalid or impossible population");
    } else {
        city->stats[1] = population;
    }
}
