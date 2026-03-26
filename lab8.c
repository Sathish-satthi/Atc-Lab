#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum { T_EOF, T_ID, T_KEYWORD, T_NUMBER, T_SYMBOL } TokenType;

typedef struct { TokenType type; char text[128]; int line; } Token;

static const char *src;
static int src_len;
static int src_pos;
static int line_no;

const char *keywords[] = { "if", "else", "for", "while", "do", "switch", "break", "continue", "goto", "return", NULL };

int is_keyword(const char *id) {
    for (int i = 0; keywords[i]; i++) {
        if (strcmp(id, keywords[i]) == 0) return 1;
    }
    return 0;
}

void skip_whitespace_comments() {
    while (src_pos < src_len) {
        char c = src[src_pos];
        if (c == ' ' || c == '\t' || c == '\r' || c == '\f' || c == '\v') {
            src_pos++;
        } else if (c == '\n') {
            line_no++; src_pos++;
        } else if (c == '/' && src_pos + 1 < src_len && src[src_pos + 1] == '/') {
            src_pos += 2;
            while (src_pos < src_len && src[src_pos] != '\n') src_pos++;
        } else if (c == '/' && src_pos + 1 < src_len && src[src_pos + 1] == '*') {
            src_pos += 2;
            while (src_pos + 1 < src_len && !(src[src_pos] == '*' && src[src_pos + 1] == '/')) {
                if (src[src_pos] == '\n') line_no++;
                src_pos++;
            }
            if (src_pos + 1 < src_len) src_pos += 2;
        } else break;
    }
}

Token next_token() {
    skip_whitespace_comments();
    Token tok; tok.line = line_no; tok.type = T_EOF; tok.text[0] = '\0';
    if (src_pos >= src_len) return tok;

    char c = src[src_pos];
    if (isalpha(c) || c == '_') {
        int start = src_pos;
        while (src_pos < src_len && (isalnum((unsigned char)src[src_pos]) || src[src_pos] == '_')) src_pos++;
        int len = src_pos - start; if (len > 126) len = 126;
        strncpy(tok.text, src + start, len); tok.text[len] = '\0';
        tok.type = is_keyword(tok.text) ? T_KEYWORD : T_ID;
        return tok;
    }

    if (isdigit(c)) {
        int start = src_pos;
        while (src_pos < src_len && isdigit((unsigned char)src[src_pos])) src_pos++;
        int len = src_pos - start; if (len > 126) len = 126;
        strncpy(tok.text, src + start, len); tok.text[len] = '\0';
        tok.type = T_NUMBER;
        return tok;
    }

    int start = src_pos;
    src_pos++;
    if (src_pos < src_len) {
        if ((c == '<' || c == '>' || c == '=' || c == '!') && src[src_pos] == '=') src_pos++;
        else if ((c == '&' || c == '|') && src[src_pos] == c) src_pos++;
    }
    int len = src_pos - start;
    if (len > 126) len = 126;
    strncpy(tok.text, src + start, len); tok.text[len] = '\0';
    tok.type = T_SYMBOL;
    return tok;
}

Token lookahead;

void consume() { lookahead = next_token(); }
int match(const char *s) { if (strcmp(lookahead.text, s) == 0) { consume(); return 1; } return 0; }

void parse_statement();

void parse_until_close_paren() {
    if (!match("(")) return;
    int depth = 1;
    while (depth > 0 && lookahead.type != T_EOF) {
        if (match("(")) depth++;
        else if (match(")")) depth--;
        else consume();
    }
}

void parse_simple_stmt() {
    while (lookahead.type != T_EOF && strcmp(lookahead.text, ";") != 0) {
        if (strcmp(lookahead.text, "(") == 0) parse_until_close_paren();
        else if (strcmp(lookahead.text, "{") == 0) { parse_statement(); }
        else consume();
    }
    match(";");
}

void parse_block() {
    if (!match("{")) return;
    while (lookahead.type != T_EOF && !match("}")) {
        parse_statement();
    }
}

void parse_statement() {
    if (lookahead.type == T_KEYWORD) {
        if (strcmp(lookahead.text, "if") == 0) {
            int line = lookahead.line; consume(); parse_until_close_paren();
            printf("Found if at line %d\n", line);
            parse_statement();
            if (match("else")) { printf("Found else at line %d\n", line_no); parse_statement(); }
            return;
        }
        if (strcmp(lookahead.text, "for") == 0) {
            int line = lookahead.line; consume(); parse_until_close_paren();
            printf("Found for at line %d\n", line);
            parse_statement(); return;
        }
        if (strcmp(lookahead.text, "while") == 0) {
            int line = lookahead.line; consume(); parse_until_close_paren();
            printf("Found while at line %d\n", line);
            parse_statement(); return;
        }
        if (strcmp(lookahead.text, "do") == 0) {
            int line = lookahead.line; consume(); parse_statement();
            if (match("while")) { parse_until_close_paren(); match(";"); printf("Found do-while at line %d\n", line); }
            return;
        }
        if (strcmp(lookahead.text, "switch") == 0) {
            int line = lookahead.line; consume(); parse_until_close_paren();
            printf("Found switch at line %d\n", line);
            parse_statement(); return;
        }
        if (strcmp(lookahead.text, "break") == 0) { printf("Found break at line %d\n", lookahead.line); consume(); match(";"); return; }
        if (strcmp(lookahead.text, "continue") == 0) { printf("Found continue at line %d\n", lookahead.line); consume(); match(";"); return; }
        if (strcmp(lookahead.text, "goto") == 0) { printf("Found goto at line %d\n", lookahead.line); consume(); if (lookahead.type == T_ID) consume(); match(";"); return; }
        if (strcmp(lookahead.text, "return") == 0) { printf("Found return at line %d\n", lookahead.line); consume(); while (lookahead.type != T_EOF && strcmp(lookahead.text, ";") != 0) consume(); match(";"); return; }
    }

    if (strcmp(lookahead.text, "{") == 0) { parse_block(); return; }
    parse_simple_stmt();
}

int main() {
    FILE *f = stdin;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);
    if (len < 0) len = 0;
    src = malloc(len + 1);
    if (!src) return 1;
    fread((char*)src, 1, len, f);
    ((char*)src)[len] = '\0';

    src_len = (int)len;
    src_pos = 0;
    line_no = 1;
    consume();

    while (lookahead.type != T_EOF) {
        parse_statement();
    }

    free((void*)src);
    return 0;
}
