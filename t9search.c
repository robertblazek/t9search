#include <stdio.h>
#include <string.h>

#ifndef DEBUG
#define log(msg, ...) (0)
#else
#define log(msg, ...) fprintf(stdout, "\033[0;34m" "> " msg "\033[0m" "\n")
#endif

#define itemLength 100
#define itemCount 50



typedef struct {
    char name[itemLength];
    char tel[itemLength];
} Contact;

 typedef struct {
     Contact item[itemCount];
 } contacts;



void loadInput(contacts *c) {
    for (int itemNumber = 0; itemNumber < itemCount; ++itemNumber) {
        for (int charNumber = 0; charNumber < itemLength; ++charNumber) { /* vynulovat pole */
            c->item[itemNumber].name[charNumber] = 0;
            c->item[itemNumber].tel[charNumber] = 0;
        }
        fgets(c->item[itemNumber].name, itemLength, stdin);
        fgets(c->item[itemNumber].tel, itemLength, stdin);

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
    for (int itemNumber = 0; itemNumber < itemCount; ++itemNumber) {
        for (int charNumber = 0; charNumber < itemLength; ++charNumber) {
            if (c->item[itemNumber].tel[charNumber] != '+' && c->item[itemNumber].tel[charNumber] != '\0') {
                if (c->item[itemNumber].tel[charNumber] < '0' || c->item[itemNumber].tel[charNumber] > '9') {
                    returnValue = 1;
                }
            }
        }
    }
    return returnValue;
}

int main() {
    log("zacinam");
    contacts *pc;
    contacts c;
    pc = &c;

    loadInput(pc);
    if (verifyInput(pc) == 0) {

    }
    else {
        fprintf(stderr, "error: Phone number contains non-digit characters or your inputs are too long!\n");
        log("error");
        return 1;

    }


    log("koncim");
    return 0;
}
