#include <stdio.h>
#include <string.h>

typedef struct {
    char lhs[32];
    char rhs1[64];
    char rhs2[64];
} Production;

static char first_symbol(const char *rhs) {
    while (*rhs == ' ' || *rhs == '\t') {
        rhs++;
    }
    return *rhs;
}

int main(void) {
    Production p[20];
    int n, i, j;
    int ambiguous = 0;

    printf("Enter number of productions: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > 20) {
        printf("Invalid input.\n");
        return 1;
    }

    printf("Enter productions in form A alpha beta\n");
    printf("Example: E E+E E*E\n");

    for (i = 0; i < n; i++) {
        if (scanf("%31s %63s %63s", p[i].lhs, p[i].rhs1, p[i].rhs2) != 3) {
            printf("Invalid production input.\n");
            return 1;
        }
    }

    for (i = 0; i < n; i++) {
        if (first_symbol(p[i].rhs1) == first_symbol(p[i].rhs2)) {
            ambiguous = 1;
            printf("Possible ambiguity in production: %s -> %s | %s\n",
                   p[i].lhs, p[i].rhs1, p[i].rhs2);
        }
        if (strcmp(p[i].lhs, "E") == 0 &&
            ((strstr(p[i].rhs1, "E+E") != NULL && strstr(p[i].rhs2, "E*E") != NULL) ||
             (strstr(p[i].rhs2, "E+E") != NULL && strstr(p[i].rhs1, "E*E") != NULL))) {
            ambiguous = 1;
            printf("Classic arithmetic ambiguity detected in %s productions.\n", p[i].lhs);
        }
        for (j = i + 1; j < n; j++) {
            if (strcmp(p[i].lhs, p[j].lhs) == 0 &&
                first_symbol(p[i].rhs1) == first_symbol(p[j].rhs1)) {
                ambiguous = 1;
                printf("Common-prefix ambiguity between productions of %s.\n", p[i].lhs);
            }
        }
    }

    if (!ambiguous) {
        printf("No obvious ambiguity detected by this heuristic checker.\n");
    }

    return 0;
}
