#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXLEN 128

struct MACaddress{
    unsigned octet1:8;
    unsigned octet2:8;
    unsigned octet3:8;
    unsigned octet4:8;
    unsigned octet5:8;
    unsigned octet6:8;
};

typedef struct MACaddress MAC;

int menu_input(){
    int answer;
    puts("Enter 1 to input MAC address");
    puts("Enter 2 to show MAC address");
    puts("Enter 3 to toggle broadcast bit in MAC address");
    puts("Enter 0 to exit\n");
    scanf("%d", &answer);
    getchar();
    return answer;
}

void split_string(char *inputString, char **words, int *wordCount, char delimiter) {
    int wordIndex = 0, wordStart=0, wordLength=0, inWord=0, i, len;

    for (i = 0; i <= strlen(inputString); i++) {
        if ((inputString[i] == delimiter || inputString[i] == '\0')&&(inWord==1)) {
            words[wordIndex] = (char *)malloc(wordLength + 1);
            strncpy(words[wordIndex], inputString + wordStart, wordLength);
            words[wordIndex][wordLength] = '\0';
            wordIndex++;
            inWord = 0;
        } else {
            if (inWord==0) {
                wordStart = i;
                wordLength = 1;
                inWord = 1;
            } else {
                wordLength++;
            }
        }
    }
    *wordCount = wordIndex;
    for (i = 0; i < *wordCount; i++) {
        len = strlen(words[i]);
        if (words[i][len - 1] == '\r') words[i][len - 1] = '\0';
    }
}

void input_mac(MAC *address){
    char input[MAXLEN], *octet[6];
    unsigned int a;
    int i, wordCount;
    printf("Enter the MAC-Address: ");
    fgets(input, MAXLEN, stdin);
    input[strcspn(input, "\n")] = 0;
    split_string(input,octet,&wordCount,':');
    for (i=0; i<6; i++) {
        if (sscanf(octet[i], "%x", &a) != 1 || a > 255 || strlen(octet[i])>2) printf("Invalid Input. Octet%d will be written incorrectly in the MAC address.\n", i+1);
        switch (i) {
            case 0: address->octet1 = (unsigned char)a; break;
            case 1: address->octet2 = (unsigned char)a; break;
            case 2: address->octet3 = (unsigned char)a; break;
            case 3: address->octet4 = (unsigned char)a; break;
            case 4: address->octet5 = (unsigned char)a; break;
            case 5: address->octet6 = (unsigned char)a; break;
        }
    }
    puts("");
}

void output_mac(MAC address){
    printf("Current MAC-address: %02X:%02X:%02X:%02X:%02X:%02X\n\n", address.octet1, address.octet2, address.octet3, address.octet4, address.octet5, address.octet6);
}

void toggle_broadcast_bit(MAC address) {
    output_mac(address);
    if ((address.octet1 & 0x80) == 0) {
        address.octet1 |= 0x80;
    } else {
        address.octet1 &= 0x7F;
    }
    printf("New MAC-address: %02X:%02X:%02X:%02X:%02X:%02X\n\n", address.octet1, address.octet2, address.octet3, address.octet4, address.octet5, address.octet6);
    printf("MAC-address succesfully changed from ");
    if ((address.octet1 & 0x80)!= 0) {
        printf("unicast to multicast\n\n");
    } else {
        printf("multicast to unicast\n\n");
    }
}

void clear_console(){
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

int main(){
    int answer;
    MAC address = {0};
    do {
        answer = menu_input();
        clear_console();
        if (answer == 1){
            input_mac(&address);
        }
        else if (answer == 2){
            output_mac(address);
        }
        else if (answer == 3){
            toggle_broadcast_bit(address);
        }
    } while (answer != 0);
    return 0;
}
