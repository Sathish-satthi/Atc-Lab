#include <ctype.h>
#include <stdio.h>
#include <string.h>

static const char *keywords[] = {
    "auto",     "break",   "case",   "char",     "const",   "continue",
    "default",  "do",      "double", "else",     "enum",    "extern",
    "float",    "for",     "goto",   "if",       "int",     "long",
    "register", "return",  "short",  "signed",   "sizeof",  "static",
    "struct",   "switch",  "typedef","union",    "unsigned","void",
    "volatile", "while"
};

static int is_keyword(const char *word) {
    size_t count = sizeof(keywords) / sizeof(keywords[0]);
    size_t i;
    for (i = 0; i < count; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

static int is_two_char_operator(char a, char b) {
    char pair[3];
    const char *ops[] = {"==", "!=", "<=", ">=", "++", "--", "&&", "||",
                         "+=", "-=", "*=", "/=", "%="};
    size_t i;
    pair[0] = a;
    pair[1] = b;
    pair[2] = '\0';
    for (i = 0; i < sizeof(ops) / sizeof(ops[0]); i++) {
        if (strcmp(pair, ops[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int main(void) {
    char input[4096];
    size_t i = 0;

    printf("Enter a C-like statement or code line:\n");
    if (!fgets(input, sizeof(input), stdin)) {
        return 0;
    }

    while (input[i] != '\0') {
        if (isspace((unsigned char)input[i])) {
            i++;
            continue;
        }

        if (isalpha((unsigned char)input[i]) || input[i] == '_') {
            char token[128];
            size_t j = 0;
            while (isalnum((unsigned char)input[i]) || input[i] == '_') {
                if (j + 1 < sizeof(token)) {
                    token[j++] = input[i];
                }
                i++;
            }
            token[j] = '\0';
            printf("%s : %s\n", token, is_keyword(token) ? "Keyword" : "Identifier");
            continue;
        }

        if (isdigit((unsigned char)input[i])) {
            char token[128];
            size_t j = 0;
            while (isdigit((unsigned char)input[i]) || input[i] == '.') {
                if (j + 1 < sizeof(token)) {
                    token[j++] = input[i];
                }
                i++;
            }
            token[j] = '\0';
            printf("%s : Number\n", token);
            continue;
        }

        if (input[i + 1] != '\0' && is_two_char_operator(input[i], input[i + 1])) {
            printf("%c%c : Operator\n", input[i], input[i + 1]);
            i += 2;
            continue;
        }

        if (strchr("+-*/%=<>!&|^", input[i])) {
            printf("%c : Operator\n", input[i]);
            i++;
            continue;
        }

        if (strchr("(){}[];,.", input[i])) {
            printf("%c : Delimiter\n", input[i]);
            i++;
            continue;
        }

        printf("%c : Unknown\n", input[i]);
        i++;
    }

    return 0;
}
