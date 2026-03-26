#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static const char *input;

static void skip_spaces(void) {
    while (isspace((unsigned char)*input)) {
        input++;
    }
}

static int parse_expression(void);

static int parse_number(void) {
    int value = 0;
    skip_spaces();
    if (!isdigit((unsigned char)*input)) {
        fprintf(stderr, "Expected number near '%s'\n", input);
        exit(1);
    }
    while (isdigit((unsigned char)*input)) {
        value = value * 10 + (*input - '0');
        input++;
    }
    return value;
}

static int parse_factor(void) {
    int value;
    skip_spaces();
    if (*input == '(') {
        input++;
        value = parse_expression();
        skip_spaces();
        if (*input != ')') {
            fprintf(stderr, "Missing closing parenthesis.\n");
            exit(1);
        }
        input++;
        return value;
    }
    return parse_number();
}

static int parse_term(void) {
    int value = parse_factor();
    skip_spaces();
    while (*input == '*' || *input == '/') {
        char op = *input++;
        int rhs = parse_factor();
        if (op == '*') {
            value *= rhs;
        } else {
            value /= rhs;
        }
        skip_spaces();
    }
    return value;
}

static int parse_expression(void) {
    int value = parse_term();
    skip_spaces();
    while (*input == '+' || *input == '-') {
        char op = *input++;
        int rhs = parse_term();
        if (op == '+') {
            value += rhs;
        } else {
            value -= rhs;
        }
        skip_spaces();
    }
    return value;
}

int main(void) {
    char expr[256];
    int result;

    printf("Enter arithmetic expression: ");
    if (!fgets(expr, sizeof(expr), stdin)) {
        return 0;
    }

    input = expr;
    result = parse_expression();
    skip_spaces();
    if (*input != '\0' && *input != '\n') {
        fprintf(stderr, "Unexpected token near '%s'\n", input);
        return 1;
    }

    printf("Result = %d\n", result);
    return 0;
}
