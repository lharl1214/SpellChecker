//
// Created by lharr on 9/14/2025.
//

#include "Distance.h"

#include <stdio.h>
#include <string.h>

int difference (char s1[], char s2[], int p1, int p2) {
    if (s1[p1] == '\0' && s2[p2] == '\0') {return 0;}
    if (s1[p1] == '\0') return (int)strlen(s2+p2);
    if (s2[p2] == '\0') return (int)strlen(s1+p1);

    if ((s1[p1] != s2[p2]) && strlen(s1) > strlen(s2) ) {
        for (int i = p1; s1[i] != '\0'; ++i) {
            s1[i] = s1[i+1];
        }
        return 1+ difference(s1, s2, p1, p2);
    }

    if ((s1[p1] != s2[p2]) && strlen(s1) < strlen(s2) ) {
        for (int i = p2; s2[i] != '\0'; ++i) {
            s2[i] = s2[i+1];
        }
        return 1+ difference(s1, s2, p1, p2);
    }


    if ((s1[p1] != s2[p2]) && strlen(s1) == strlen(s2)) {
        return 1+ difference (s1, s2, p1+1, p2 +1);
    }
    if (s1[p1] == s2[p2]) { return difference (s1, s2, p1+1, p2 +1);}

}


