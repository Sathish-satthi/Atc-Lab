#include <stdio.h>
#include <string.h>

typedef struct {
    char result[16];
    char arg1[16];
    char op[4];
    char arg2[16];
} Statement;

typedef struct {
    char op[4];
    char left[16];
    char right[16];
    char representative[16];
} ExpressionNode;

int main(void) {
    Statement stmt[20];
    ExpressionNode expr[20];
    int n, expr_count = 0;
    int i, j;

    printf("Enter number of three-address statements: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > 20) {
        printf("Invalid input.\n");
        return 1;
    }

    printf("Enter statements as: result arg1 op arg2\n");
    printf("For assignment, use op as = and arg2 as _\n");

    for (i = 0; i < n; i++) {
        if (scanf("%15s %15s %3s %15s",
                  stmt[i].result, stmt[i].arg1, stmt[i].op, stmt[i].arg2) != 4) {
            printf("Invalid statement format.\n");
            return 1;
        }
    }

    printf("\nOptimized Basic Block:\n");

    for (i = 0; i < n; i++) {
        if (strcmp(stmt[i].op, "=") == 0) {
            printf("%s = %s\n", stmt[i].result, stmt[i].arg1);
            continue;
        }

        for (j = 0; j < expr_count; j++) {
            if (strcmp(expr[j].op, stmt[i].op) == 0 &&
                strcmp(expr[j].left, stmt[i].arg1) == 0 &&
                strcmp(expr[j].right, stmt[i].arg2) == 0) {
                break;
            }
        }

        if (j < expr_count) {
            printf("%s = %s\n", stmt[i].result, expr[j].representative);
        } else {
            strcpy(expr[expr_count].op, stmt[i].op);
            strcpy(expr[expr_count].left, stmt[i].arg1);
            strcpy(expr[expr_count].right, stmt[i].arg2);
            strcpy(expr[expr_count].representative, stmt[i].result);
            expr_count++;
            printf("%s = %s %s %s\n",
                   stmt[i].result, stmt[i].arg1, stmt[i].op, stmt[i].arg2);
        }
    }

    return 0;
}
