#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

        int charNumber = 0;
        bool readingName = true;
        for (int i = 0; i < 2; ++i) {

            while (1) {

                char a;


                a = getchar();

                if ((a == EOF) || (charNumber > itemLength - 1) || (a == '\n')) {
                    break;
                } else {
                    if (readingName) {
                        c->item[itemNumber].name[charNumber] = a;
                    } else {
                        c->item[itemNumber].tel[charNumber] = a;
                    }

                }

                charNumber++;
            }
        }
    }
}

int main() {
    log("zacinam");
    contacts *pc;
    contacts c;
    pc = &c;

    loadInput(pc);


    log("koncim");
    return 0;
}
