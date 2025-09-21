#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Distance.h"
#include <string.h>

// method to make everything lowercase
static void lowercase(char *s){
    for (; *s; ++s) *s = (char)tolower((unsigned char)*s);
}
int main() {
    FILE *fp = fopen("aspell-english-common.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Array of strings
    char results[20][46] = {{0}};
    char d1stor[20][46] = {{0}};
    char d2stor[20][46] = {{0}};
    char d3stor[20][46] = {{0}};

    // Ints for tracking
    int dist1, dist2, dist3, hits = 0;

    // Strings
    char dictWord[46];
    char userString[46];

    while (1) {

        printf("\nEnter a word to spellcheck, or exit: ");
        scanf("%s", userString);
        lowercase(userString);

        // if input = exit, exit
        if (_stricmp(userString, "exit") == 0) {
            printf("Exiting...\n");
            exit(0);
        }

        // reset between inputs
        bool match = false;
        memset(results, 0, sizeof(results));
        memset(d1stor, 0, sizeof(d1stor));
        memset(d2stor, 0, sizeof(d2stor));
        memset(d3stor, 0, sizeof(d3stor));
        dist1 = dist2 = dist3 = hits = 0;
        rewind(fp);


        // if input != exit
        if (_stricmp(userString, "exit") != 0) {
            while (fgets(dictWord, 46, fp) != NULL) {
                // get the string, and remove \n from the char array
                dictWord[strcspn(dictWord, "\r\n")] = '\0';

                // make copies so difference() doesn't mess up originals
                char a[46], b[46];
                strcpy(a, userString), strcpy(b, dictWord);
                lowercase(a);
                lowercase(b);

                // get the # of substitutions to match input and dictionary words
                int x = difference(a,b,0,0);

                // if word is spelled correctly
                if ( x == 0) {
                    match = true;
                    memset(results, 0, sizeof(results));
                    printf("'%s' is spelled correctly", userString);
                    break;
                }

                // Fill respective arrays based on distance

                if ( x == 1 && dist1 < 20) {
                    strncpy(d1stor[dist1],dictWord,46);
                    d1stor[dist1][46] = '\0';
                    dist1++;
                }

                if ( x == 2 && dist2 < 20) {
                    strncpy(d2stor[dist2],dictWord,46);
                    d2stor[dist2][46] = '\0';
                    dist2++;
                }
                if ( x == 3 && dist3 < 20) {
                    strncpy(d3stor[dist3],dictWord,46);
                    d3stor[dist3][46] = '\0';
                    dist3++;
                }
            }

            // if input was not spelled correctly, print up to 20 suggestions
            if (match == false) {
                // copy up to first 20 of 'only 1 substitution' to results
                for (int i = 0; i <= dist1 && i<20; i++) {
                    strncpy(results[i], d1stor[i],46);
                }

                // if there wasn't 20 of 'only 1 substitution', start adding 'only 2'
                if (dist1 < 20) {
                    for (int i = dist1; i <= (dist1 + dist2) && i<20; i++) {
                        strncpy(results[i], d2stor[i],46);
                    }
                }
                // if there wasn't 20 of 'only 1 & 2 substitutions', start adding 'only 3'
                if ((dist1 + dist2) < 20) {
                    for (int i = (dist1 + dist2); i <= (dist1 + dist2 + dist3) && i<20; i++) {
                        strncpy(results[i], d3stor[i],46);
                    }
                }

                // if there was more than 20 total subs, only fill the list to 20;
                //  otherwise, fill the list with what was collected.
                if ((dist1 + dist2 + dist3) >= 20) {
                    hits = 20;
                } else hits = (dist1 + dist2 + dist3);
                printf("Similar words:\n");
                for (int i = 0; i < hits; i++) {
                    puts(results[i]);
                }
            }
        }
    }
}