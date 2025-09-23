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

// method to trim strings
static void trim(char *s) {
    s[strcspn(s, "\r\n")] = '\0';
}

// Dynamic array (resizable) for dictionary words
typedef struct {
    char **w;     // pointer to array of strings
    size_t n;     // number of words currently stored
    size_t cap;   // allocated capacity
} WordList;

// Push a new word into the WordList
static void wl_push(WordList *wl, const char *word){
    // Grow array if full
    if (wl->n == wl->cap){
        wl->cap = wl->cap ? wl->cap * 2 : 1024; // start at 1024, double each time
        wl->w = realloc(wl->w, wl->cap * sizeof *wl->w);
        if (!wl->w){ perror("realloc"); exit(1); }
    }
    // Duplicate string so it has its own storage
    wl->w[wl->n++] = strdup(word);
}




int main() {
    // user inputs a file to read from
    char fileName[999];
    printf("\nEnter a word list to use: ");
    scanf("%s", fileName);
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }


    WordList dict = {0};
    char line[46];


    // Array of strings - closest matches ; User input string
    char results[20][46] = {{0}};
    char userString[46];

    // Read the file & fill the in-code dictionary
    while (fgets(line, sizeof line, fp)){
        trim(line);
        lowercase(line);
        if (*line == '\0') continue;
        wl_push(&dict, line);
    }
    fclose(fp);

    // begin spelling
    while (1) {
        printf("\nEnter a word to spellcheck, or exit: ");
        scanf("%s", userString);
        lowercase(userString);

        // values for filling and printing
        int lowest = 99;
        int tracker = 0;

        // if input = exit, exit
        if (_stricmp(userString, "exit") == 0) {
            printf("Exiting...\n");
            exit(0);
        }

        // reset between inputs
        bool match = false;
        memset(results, 0, sizeof(results));


        // if input != exit
        for (size_t i = 0; i < dict.n; ++i) {
            const char *dictWord = dict.w[i];

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

            // if a new closest match is found, throw out old and start filling
            if ( x < lowest) {
                lowest = x;
                memset(results, 0, sizeof(results));
                tracker = 0;
                strncpy(results[tracker],dictWord,46);
                results[tracker][45] = '\0';
                tracker++;
            }

            // if word is as close as the rest, continue filling
            if ( x == lowest && tracker < 20) {
                strncpy(results[tracker],dictWord,46);
                results[tracker][45] = '\0';
                tracker++;
            }
        }

        // at the end, if the word was spelled wrong, print out the closest options
        if (!match) {
            for (int i = 0; i < tracker; ++i) {
                puts(results[i]);
            }
        }
    }
}