#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char value[16];
    struct Node *left;
    struct Node *right;
} Node;

static const char *input;

static void skip_spaces(void) {
    while (isspace((unsigned char)*input)) {
        input++;
    }
}

static Node *new_node(const char *value, Node *left, Node *right) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    snprintf(node->value, sizeof(node->value), "%s", value);
    node->left = left;
    node->right = right;
    return node;
}

static Node *parse_expression(void);

static Node *parse_factor(void) {
    char number[16];
    int idx = 0;
    skip_spaces();

    if (*input == '(') {
        Node *node;
        input++;
        node = parse_expression();
        skip_spaces();
        if (*input == ')') {
            input++;
        }
        return node;
    }

    while (isdigit((unsigned char)*input)) {
        if (idx + 1 < (int)sizeof(number)) {
            number[idx++] = *input;
        }
        input++;
    }
    number[idx] = '\0';
    return new_node(number, NULL, NULL);
}

static Node *parse_term(void) {
    Node *node = parse_factor();
    skip_spaces();
    while (*input == '*' || *input == '/') {
        char op[2];
        Node *right;
        op[0] = *input++;
        op[1] = '\0';
        right = parse_factor();
        node = new_node(op, node, right);
        skip_spaces();
    }
    return node;
}

static Node *parse_expression(void) {
    Node *node = parse_term();
    skip_spaces();
    while (*input == '+' || *input == '-') {
        char op[2];
        Node *right;
        op[0] = *input++;
        op[1] = '\0';
        right = parse_term();
        node = new_node(op, node, right);
        skip_spaces();
    }
    return node;
}

static void preorder(Node *root) {
    if (root == NULL) {
        return;
    }
    printf("%s ", root->value);
    preorder(root->left);
    preorder(root->right);
}

static void inorder(Node *root) {
    if (root == NULL) {
        return;
    }
    if (root->left != NULL) {
        printf("( ");
    }
    inorder(root->left);
    printf("%s ", root->value);
    inorder(root->right);
    if (root->right != NULL) {
        printf(") ");
    }
}

static void postorder(Node *root) {
    if (root == NULL) {
        return;
    }
    postorder(root->left);
    postorder(root->right);
    printf("%s ", root->value);
}

static void free_tree(Node *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main(void) {
    char expr[256];
    Node *root;

    printf("Enter arithmetic expression: ");
    if (!fgets(expr, sizeof(expr), stdin)) {
        return 0;
    }

    input = expr;
    root = parse_expression();

    printf("Preorder  : ");
    preorder(root);
    printf("\n");

    printf("Inorder   : ");
    inorder(root);
    printf("\n");

    printf("Postorder : ");
    postorder(root);
    printf("\n");

    free_tree(root);
    return 0;
}
