#include <stdio.h>

int main(void) {
    int c, next;
    int in_string = 0;
    int in_char = 0;

    printf("Enter C code. Press Ctrl+Z then Enter on Windows to finish:\n");

    while ((c = getchar()) != EOF) {
        if (!in_string && !in_char && c == '/') {
            next = getchar();
            if (next == '/') {
                while ((c = getchar()) != EOF && c != '\n') {
                }
                if (c == '\n') {
                    putchar('\n');
                }
            } else if (next == '*') {
                int prev = 0;
                while ((c = getchar()) != EOF) {
                    if (prev == '*' && c == '/') {
                        break;
                    }
                    prev = c;
                }
            } else {
                putchar('/');
                if (next != EOF) {
                    ungetc(next, stdin);
                }
            }
            continue;
        }

        if (c == '"' && !in_char) {
            in_string = !in_string;
        } else if (c == '\'' && !in_string) {
            in_char = !in_char;
        }

        putchar(c);
    }

    return 0;
}
