#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int is_operator(char c) {
    return c == '*' || c == '+' || c == '?';
}

static int validate_regex(const char *pattern) {
    int balance = 0;
    size_t i;

    if (pattern[0] == '\0') {
        return 0;
    }

    for (i = 0; pattern[i] != '\0'; i++) {
        char c = pattern[i];
        if (c == '(') {
            balance++;
        } else if (c == ')') {
            balance--;
            if (balance < 0) {
                return 0;
            }
        } else if (c == '|') {
            if (i == 0 || pattern[i + 1] == '\0') {
                return 0;
            }
        } else if (is_operator(c)) {
            if (i == 0 || is_operator(pattern[i - 1]) || pattern[i - 1] == '(') {
                return 0;
            }
        } else if (!isalnum((unsigned char)c) && c != '.' && c != ' ') {
            return 0;
        }
    }

    return balance == 0;
}

static int match_here(const char *pattern, const char *text);

static int char_matches(char pattern_char, char text_char) {
    return pattern_char == '.' || pattern_char == text_char;
}

static int match_star(char c, const char *pattern, const char *text) {
    do {
        if (match_here(pattern, text)) {
            return 1;
        }
    } while (*text != '\0' && char_matches(c, *text++));
    return 0;
}

static int match_plus(char c, const char *pattern, const char *text) {
    if (*text == '\0' || !char_matches(c, *text)) {
        return 0;
    }
    text++;
    while (*text != '\0' && char_matches(c, *text)) {
        if (match_here(pattern, text)) {
            return 1;
        }
        text++;
    }
    return match_here(pattern, text);
}

static int match_question(char c, const char *pattern, const char *text) {
    if (match_here(pattern, text)) {
        return 1;
    }
    if (*text != '\0' && char_matches(c, *text)) {
        return match_here(pattern, text + 1);
    }
    return 0;
}

static int match_here(const char *pattern, const char *text) {
    if (pattern[0] == '\0') {
        return *text == '\0';
    }
    if (pattern[1] == '*') {
        return match_star(pattern[0], pattern + 2, text);
    }
    if (pattern[1] == '+') {
        return match_plus(pattern[0], pattern + 2, text);
    }
    if (pattern[1] == '?') {
        return match_question(pattern[0], pattern + 2, text);
    }
    if (*text != '\0' && char_matches(pattern[0], *text)) {
        return match_here(pattern + 1, text + 1);
    }
    return 0;
}

int main(void) {
    char pattern[256];
    char text[256];

    printf("Enter regular expression: ");
    if (!fgets(pattern, sizeof(pattern), stdin)) {
        return 0;
    }
    pattern[strcspn(pattern, "\n")] = '\0';

    if (!validate_regex(pattern)) {
        printf("Invalid regular expression.\n");
        return 1;
    }

    printf("Enter string to test: ");
    if (!fgets(text, sizeof(text), stdin)) {
        return 0;
    }
    text[strcspn(text, "\n")] = '\0';

    if (match_here(pattern, text)) {
        printf("Match successful.\n");
    } else {
        printf("No match.\n");
    }

    return 0;
}
