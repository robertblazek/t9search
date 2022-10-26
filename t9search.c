#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef DEBUG
#define log(msg, ...) (0)
#else
#define log(msg, ...) fprintf(stdout, "\033[0;34m" "> " msg "\033[0m" "\n")
#endif

#define ITEM_LENGTH 100
#define ITEM_COUNT 50



typedef struct {
    char name[ITEM_LENGTH];
    char tel[ITEM_LENGTH];
} Contact;

 typedef struct {
     Contact item[ITEM_COUNT];
 } contacts;



void loadInput(contacts *c) {
    for (int itemNumber = 0; itemNumber < ITEM_COUNT; ++itemNumber) {
        for (int charNumber = 0; charNumber < ITEM_LENGTH; ++charNumber) { /* vynulovat pole */
            c->item[itemNumber].name[charNumber] = 0;
            c->item[itemNumber].tel[charNumber] = 0;
        }
        fgets(c->item[itemNumber].name, ITEM_LENGTH, stdin);
        fgets(c->item[itemNumber].tel, ITEM_LENGTH, stdin);

        c->item[itemNumber].name[strcspn(c->item[itemNumber].name, "\n")] = 0;
        c->item[itemNumber].tel[strcspn(c->item[itemNumber].tel, "\n")] = 0;

    }



    int a;
    do {
        a = getchar();
    } while (a != EOF);
}

int verifyInput(contacts *c) {
    int returnValue = 0;
    for (int itemNumber = 0; itemNumber < ITEM_COUNT; ++itemNumber) {
        if ( (strlen(c->item[itemNumber].name) > 0) && (strlen(c->item[itemNumber].tel) == 0) ) {
            returnValue = 1;
        }
        if ( (strlen(c->item[itemNumber].name) == 0) && (strlen(c->item[itemNumber].tel) > 0) ) {
            returnValue = 1;
        }
        for (int charNumber = 0; charNumber < ITEM_LENGTH; ++charNumber) {
            if (c->item[itemNumber].tel[charNumber] != '+' && c->item[itemNumber].tel[charNumber] != '\0') {
                if (c->item[itemNumber].tel[charNumber] < '0' || c->item[itemNumber].tel[charNumber] > '9') {
                    returnValue = 1;
                }
            }
        }
    }
    return returnValue;
}

bool matchNumber(char *q, Contact *cc, bool *spaces) {

}

void printContact(Contact *cc) {
    if (strlen(cc->name) > 0 && strlen(cc->tel) > 0) {
        printf("%s, %s\n", cc->name, cc->tel);
    }
}

void printAll(contacts *c) {
    for (int itemNumber = 0; itemNumber < ITEM_COUNT; ++itemNumber) {
        printContact((Contact *) &c->item[itemNumber]);
    }
}

int main(int argc, char *argv[]) {
    log("zacinam");
    contacts *pc;
    contacts c;
    pc = &c;

    loadInput(pc);
    if (verifyInput(pc) == 0) {
        if (argc > 1) {
            char *query = argv[1];

        }
        else printAll(pc);
    }
    else {
        fprintf(stderr, "error: Phone number contains non-digit characters or your inputs are too long!\n");
        log("error");
        return 1;

    }


    log("koncim");
    return 0;
}
