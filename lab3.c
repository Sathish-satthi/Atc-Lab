#include <ctype.h>
#include <stdio.h>

int main(void) {
    int c;
    long lines = 0;
    long words = 0;
    long chars = 0;
    int in_word = 0;

    printf("Enter text. Press Ctrl+Z then Enter on Windows to finish:\n");

    while ((c = getchar()) != EOF) {
        chars++;
        if (c == '\n') {
            lines++;
        }
        if (isspace(c)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            words++;
        }
    }

    printf("Lines      : %ld\n", lines);
    printf("Words      : %ld\n", words);
    printf("Characters : %ld\n", chars);

    return 0;
}
