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
    size_t ccLength = strlen(cc->tel);

    char numbers[ITEM_LENGTH] = {};

    for (size_t telIndex = 0; telIndex < ccLength; ++telIndex) {
        if (cc->tel[telIndex] == '+') {
            numbers[telIndex] = '0';
        }
        else numbers[telIndex] = cc->tel[telIndex];
    }

    if (strlen(q) < ccLength) {
        if (*spaces == true) {
            unsigned long qIndex = 0;
            //unsigned long ccIndex = 0;
            for (unsigned long ccIndex = 0; ccIndex < ccLength; ++ccIndex) {
                if (numbers[ccIndex] == q[qIndex]) {
                    qIndex++;
                }
            }
            if (qIndex == strlen(q)) {
                return true;
            }
        }
        else if (strstr(numbers, q) != NULL){
            return true;
        }
        else return false;
    }
    return false;
}

bool matchName(char *q, Contact *cc, bool *spaces) {
    const char *map[10];
    map[0] = "+";
    map[1] = "";
    map[2] = "aAbBcC";
    map[3] = "dDeEfF";
    map[4] = "gGhHiI";
    map[5] = "jJkKlL";
    map[6] = "mMnNoO";
    map[7] = "pPqQrRsS";
    map[8] = "tTuUvV";
    map[9] = "wWxXyYzZ";
    
    char numbers[ITEM_LENGTH] = {};

    size_t nameLength = strlen(cc->name);

    for (size_t nameIndex = 0; nameIndex < nameLength; nameIndex++) {
        for (int mapIndex = 0; mapIndex < 10; ++mapIndex) {
            if ( mapIndex != 1 ) {
                if (strchr(map[mapIndex], cc->name[nameIndex]) != NULL) {
                    numbers[nameIndex] = mapIndex + '0';
                }

            }
        }
        if (cc->name[nameIndex] == 32) {
            numbers[nameIndex] = '#';
        }
    }



    if (strlen(q) < nameLength) {
        if (*spaces == true) {
            unsigned long qIndex = 0;
            //unsigned long ccIndex = 0;
            for (unsigned long nameIndex = 0; nameIndex < nameLength; ++nameIndex) {
                if (numbers[nameIndex] == q[qIndex] && numbers[nameIndex] != 0) {
                    qIndex++;
                }
            }
            if (qIndex == strlen(q)) {
                return true;
            }
        }
        else if (strstr(numbers, q) != NULL){
            return true;
        }
        else return false;
    }
    return false;

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

void notFound() {
    printf("%s", "Not found\n");
}

int verifyArgs(int *argc, char *argv[]) {
    int returnValue = 0;
    if (*argc <= 3) {
        if (*argc == 2) {
            for (unsigned long i = 0; i < strlen(argv[1]); ++i) {
                if (argv[1][i] < '0' || argv[1][i] > '9') {
                    fprintf(stderr, "error: Argument contains non-digit characters");
                    returnValue = 1;
                }
            }
        } else if (*argc == 3) {
            if (strcmp(argv[1], "-s") == 0) {
                for (unsigned long i = 0; i < strlen(argv[2]); ++i) {
                    if (argv[2][i] < '0' || argv[2][i] > '9') {
                        fprintf(stderr, "error: Argument contains non-digit characters");
                        returnValue = 1;
                    }
                }
            }
            else {
                fprintf(stderr, "error: First argument should be '-s'");
                returnValue = 1;
            }
        }
        else returnValue = 0;
    } else {
        fprintf(stderr, "error: Too many arguments");
        returnValue = 1;
    }

    return returnValue;
}

int main(int argc, char *argv[]) {
    log("zacinam");

    int argVerify = verifyArgs(&argc, argv);
    if (argVerify != 0) {
        return argVerify;
    }

    contacts *pc;
    contacts c;
    pc = &c;

    loadInput(pc);
    bool spaces = false;
    bool *sp = &spaces;
    char *query = argv[1];

    int foundCount = 0;

    if (verifyInput(pc) == 0) {
        if (argc > 1) {
            if (strcmp(argv[1],"-s") == 0) {
                 query = argv[2];
                 spaces = true;
            }
            for (int itemNumber = 0; itemNumber < ITEM_COUNT; ++itemNumber) {
                if (matchNumber(query, &pc->item[itemNumber], sp) || matchName(query, &pc->item[itemNumber], sp)) {
                    printContact(&pc->item[itemNumber]);
                    foundCount++;
                }
            }
            if (foundCount == 0) notFound();
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
