/*
 * t9search.c - first project in IZP, first semester VUT Brno
 * author: Robert Blazek (xblaze39@stud.fit.vutbr.cz)
 * syntax: ./t9search.c [-s] [query]
 *
 * usage:
 *   - no parameters: read input from stdin and write it to stdout
 *
 *   - optional parameter [-s]: allows for result to be matched
 *     even if it contains another characters between the matched values
 *
 *   - optional [query]: the string to be searched within the contacts
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef DEBUG /* Enable or disable debug messages */
#define log(msg, ...) (void)0
#else
#define log(msg, ...) fprintf(stdout, "\033[0;34m" "> " msg "\033[0m" "\n")
#endif

#define ITEM_LENGTH 100 /* Maximum length of each contact record */
#define ITEM_COUNT 50 /* Maximum number of contacts */



typedef struct {
    char name[ITEM_LENGTH];
    char tel[ITEM_LENGTH];
} Contact; /* Define data structure */

 typedef struct {
     Contact item[ITEM_COUNT];
 } contacts;

/* Declare function prototypes */
void loadInput(contacts *c);
int verifyInput(contacts *c);
bool matchNumber(char *q, Contact *cc, bool *spaces);
bool matchName(char *q, Contact *cc, bool *spaces);
void printContact(Contact *cc);
void printAll(contacts *c);
void notFound();
int verifyArgs(int *argc, char *argv[]);





void loadInput(contacts *c) { /* Read input from stdin */
    for (int itemNumber = 0; itemNumber < ITEM_COUNT; ++itemNumber) {
        for (int charNumber = 0; charNumber < ITEM_LENGTH; ++charNumber) { /* Zero the array */
            c->item[itemNumber].name[charNumber] = 0;
            c->item[itemNumber].tel[charNumber] = 0;
        }
        fgets(c->item[itemNumber].name, ITEM_LENGTH, stdin); /* get strings from stdin, if a string overflows we'll deal with it later */
        fgets(c->item[itemNumber].tel, ITEM_LENGTH, stdin);

        c->item[itemNumber].name[strcspn(c->item[itemNumber].name, "\n")] = 0; /* replace newline with null */
        c->item[itemNumber].tel[strcspn(c->item[itemNumber].tel, "\n")] = 0;
    }
    int a;
    do { /* discard any extra input */
        a = getchar();
    } while (a != EOF);
}

int verifyInput(contacts *c) {
    int returnValue = 0;
    for (int itemNumber = 0; itemNumber < ITEM_COUNT; ++itemNumber) {
        if ( (strlen(c->item[itemNumber].name) > 0) && (strlen(c->item[itemNumber].tel) == 0) ) {
            returnValue = 1; /* if name exists, phone number can't be empty */
        }
        if ( (strlen(c->item[itemNumber].name) == 0) && (strlen(c->item[itemNumber].tel) > 0) ) {
            returnValue = 1; /* if phone number exists, name can't be empty */
        }
        for (int charNumber = 0; charNumber < ITEM_LENGTH; ++charNumber) {
            if (c->item[itemNumber].tel[charNumber] != '+' && c->item[itemNumber].tel[charNumber] != '\0') {
                if (c->item[itemNumber].tel[charNumber] < '0' || c->item[itemNumber].tel[charNumber] > '9') {
                    returnValue = 1; /* phone number can only contain digits and '+' */
                }
            }
        }
    }
    return returnValue; /* 0 == no problems */
}

bool matchNumber(char *q, Contact *cc, bool *spaces) { /* searching for a match in a phone number */
    size_t ccLength = strlen(cc->tel); /* get actual length of phone number */

    char numbers[ITEM_LENGTH] = {}; /* initialise array to search in */

    for (size_t telIndex = 0; telIndex < ccLength; ++telIndex) { /* take care of plus sign and copy into 'numbers[]' */
        if (cc->tel[telIndex] == '+') {
            numbers[telIndex] = '0';
        }
        else numbers[telIndex] = cc->tel[telIndex];
    }

    if (strlen(q) < ccLength) { /* verify the query is actually smaller than the input */
        if (*spaces == true) { /* checking for the '-s' param */
            unsigned long qIndex = 0;
            for (unsigned long ccIndex = 0; ccIndex < ccLength; ++ccIndex) { /* iterate over the phone number */
                if (numbers[ccIndex] == q[qIndex]) {
                    qIndex++; /* if query character matches, increase index */
                }
            }
            if (qIndex == strlen(q)) {
                return true; /* if we found all characters in succession */
            }
        }
        else if (strstr(numbers, q) != NULL){ /* without the '-s' parameter, checking for substring */
            return true;
        }
        else return false;
    }
    return false;
}

bool matchName(char *q, Contact *cc, bool *spaces) { /* searching for a match in a name */
    const char *map[10]; /* create a quasi-map of corresponding characters */
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

    size_t nameLength = strlen(cc->name); /* get actual length of the name */

    for (size_t nameIndex = 0; nameIndex < nameLength; nameIndex++) { /* replace letters with corresponding numbers */
        for (int mapIndex = 0; mapIndex < 10; ++mapIndex) {
            if ( mapIndex != 1 ) { /* 1 is only here for readability and iteration */
                if (strchr(map[mapIndex], cc->name[nameIndex]) != NULL) { /* if map index contains given char */
                    numbers[nameIndex] = mapIndex + '0'; /* save ordinal value to 'numbers[]' */
                }

            }
        }
        if (cc->name[nameIndex] == 32) { /* within the scope of the string, replace null characters (spaces) with '#' */
            numbers[nameIndex] = '#'; /* this will make the array compatible with 'strstr()' */
        }
    }



    if (strlen(q) < nameLength) { /* adaptation of the number-searching algorithm */
        if (*spaces == true) { /* checking for the '-s' param */
            unsigned long qIndex = 0;
            for (unsigned long nameIndex = 0; nameIndex < nameLength; ++nameIndex) { /* iterate over the array */
                if (numbers[nameIndex] == q[qIndex] && numbers[nameIndex] != 0) {
                    qIndex++; /* if query character matches, increase index */
                }
            }
            if (qIndex == strlen(q)) { /* if we found all characters in succession */
                return true;
            }
        }
        else if (strstr(numbers, q) != NULL){ /* without the '-s' parameter, checking for substring */
            return true;
        }
        else return false;
    }
    return false;

}

void printContact(Contact *cc) { /* print individual contact */
    if (strlen(cc->name) > 0 && strlen(cc->tel) > 0) {
        printf("%s, %s\n", cc->name, cc->tel);
    }
}

void printAll(contacts *c) { /* print all contacts */
    for (int itemNumber = 0; itemNumber < ITEM_COUNT; ++itemNumber) {
        printContact((Contact *) &c->item[itemNumber]);
    }
}

void notFound() { /* print not found message */
    printf("%s", "Not found\n");
}

int verifyArgs(int *argc, char *argv[]) { /* verify argument count and content */
    int returnValue = 0;
    if (*argc <= 3) { /* maximum allowed count is 3 */
        if (*argc == 2) { /* only search query provided */
            for (unsigned long i = 0; i < strlen(argv[1]); ++i) {
                if (argv[1][i] < '0' || argv[1][i] > '9') { /* allow digits only */
                    fprintf(stderr, "error: Argument contains non-digit characters");
                    returnValue = 1;
                }
            }
        } else if (*argc == 3) { /* two arguments provided */
            if (strcmp(argv[1], "-s") == 0) { /* if 1st provided argument is '-s' */
                for (unsigned long i = 0; i < strlen(argv[2]); ++i) {
                    if (argv[2][i] < '0' || argv[2][i] > '9') { /* allow digits only */
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
        else returnValue = 0; /* only other situation - only one (implicit) argument */
    } else {
        fprintf(stderr, "error: Too many arguments");
        returnValue = 1;
    }

    return returnValue; /* anything else than 0 == bad */
}

int main(int argc, char *argv[]) {

    int argVerify = verifyArgs(&argc, argv);
    if (argVerify != 0) { /* if args are invalid, end the program with non-zero code */
        return argVerify;
    }

    contacts *pc;
    contacts c; /* initialize structure and pointer */
    pc = &c;

    loadInput(pc); /* load input from stdin */
    bool spaces = false;
    bool *sp = &spaces; /* are we running with the '-s' flag? - initialize value and pointer */
    char *query = argv[1]; /* initialize search query pointer */

    int foundCount = 0;

    if (verifyInput(pc) == 0) { /* if input is valid */
        if (argc > 1) {
            if (strcmp(argv[1],"-s") == 0) { /* if '-s' was used, change bool and pointer position accordingly */
                 query = argv[2];
                 spaces = true;
            }
            for (int itemNumber = 0; itemNumber < ITEM_COUNT; ++itemNumber) { /* for each item */
                if (matchNumber(query, &pc->item[itemNumber], sp) || matchName(query, &pc->item[itemNumber], sp)) {
                    printContact(&pc->item[itemNumber]); /* if there's a match, print contact and increase foundCount */
                    foundCount++;
                }
            }
            if (foundCount == 0) notFound(); /* nothing was found */
        }
        else printAll(pc); /* if no arguments are provided, print everything */
    }
    else {
        fprintf(stderr, "error: Phone number contains non-digit characters or your inputs are too long!\n");
        return 1;

    }


    return 0;
}
