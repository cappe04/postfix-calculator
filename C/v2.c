#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define ERROR(...) fprintf(stderr, __VA_ARGS__); exit(1);
#define MAX_SIZE 256

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_PAR,
    TOKEN_SUB,
    TOKEN_ADD,
    TOKEN_DIV,
    TOKEN_MUL,
    TOKEN_POW,
};

typedef struct s_token {
    enum TokenType type;
    float value;
    bool l_assoc;
} Token;

Token new_token(enum TokenType type, float value){
    Token token = { type, value, true };
    switch (type){
    case TOKEN_NUMBER:
        break;
    case TOKEN_SUB:
    case TOKEN_ADD:
        token.value = 1;
        break;
    case TOKEN_DIV:
    case TOKEN_MUL:
        token.value = 2;
        break;
    case TOKEN_POW:
        token.value = 3;
        token.l_assoc = false;
        break;
    default:
        token.value = 0;
        break;
    }
    
    return token;
}

typedef struct s_expr {
    Token* tokens;
    size_t length;
} Expr;

Expr postfix(const char* expr_str){
    Token* postf = malloc(MAX_SIZE * sizeof(Token));
    Token stack[MAX_SIZE];
    char digits[MAX_SIZE]; memset(digits, 0, MAX_SIZE);

    size_t l_postf = 0;
    size_t l_stack = 0;
    size_t l_digits = 0;

    for(int i = 0; i<strlen(expr_str); i++){
        if(expr_str[i] == ' ') continue;

        if(isdigit(expr_str[i]) || expr_str[i] == '.'){
            // Append to digits
            digits[l_digits++] = expr_str[i];
            continue;
        }
        else if(l_digits){
            // Append digits to postf
            postf[l_postf++] = new_token(TOKEN_NUMBER, atof(digits));
            // Clear digits
            l_digits = 0;
            memset(digits, 0, MAX_SIZE);
        }

        if(expr_str[i] == '('){
            // Append to stack
            stack[l_stack++] = new_token(TOKEN_PAR, 0);
        }
        else if(expr_str[i] == ')'){
            while(l_stack && stack[l_stack - 1].type != TOKEN_PAR){
                // Append stack top to postf
                postf[l_postf++] = stack[--l_stack];
            }
            // Pop stack
            --l_stack;
        }
        else {
            Token operator;
            // Create operator token
            switch (expr_str[i]){
            case '-':
                operator = new_token(TOKEN_SUB, 0);
                break;
            case '+':
                operator = new_token(TOKEN_ADD, 0);
                break;
            case '/':
                operator = new_token(TOKEN_DIV, 0);
                break;
            case '*':
                operator = new_token(TOKEN_MUL, 0);
                break;
            case '^':
                operator = new_token(TOKEN_POW, 0);
                break;
            default:
                ERROR((char[9]) { 'E', 'r', 'r', 'o', 'r', ':', ' ', expr_str[i], '\0' });
            }
            while(l_stack && 
                (stack[l_stack - 1].value > operator.value || 
                (stack[l_stack - 1].value == operator.value && operator.l_assoc)))
            {
                postf[l_postf++] = stack[--l_stack];
            }
            stack[l_stack++] = operator;
        }

    };

    if(l_digits){
        // Remaining digits to postfix
        postf[l_postf++] = new_token(TOKEN_NUMBER, atof(digits));
    }

    if(l_stack){
        // Move remaining operatirs to postfix
        while(l_stack){
            postf[l_postf++] = stack[--l_stack];
        }
    }

    // Free remaining postfix buffer
    for(int i = l_postf+1; i<MAX_SIZE; i++){
        free(&postf[i]);
    }

    return (Expr) { postf, l_postf };
}

float eval(Expr expr){
    float stack[MAX_SIZE];
    size_t l_stack = 0;

    for(int i = 0; i<expr.length; i++){
        if(expr.tokens[i].type == TOKEN_NUMBER){
            stack[l_stack++] = expr.tokens[i].value;
            continue;
        }

        float b = stack[--l_stack];
        float a = stack[--l_stack];
        
        switch (expr.tokens[i].type){
        case TOKEN_SUB:
            stack[l_stack++] = a - b;
            break;
        case TOKEN_ADD:
            stack[l_stack++] = a + b;
            break;
        case TOKEN_DIV:
            stack[l_stack++] = a / b;
            break;
        case TOKEN_MUL:
            stack[l_stack++] = a * b;
            break;
        case TOKEN_POW:
            stack[l_stack++] = pow(a, b);
            break;
        
        default:
            break;
        }
    }
    return stack[0];
}

int main(){

    Expr postf = postfix("(2.71 - 3.14 + 4) * (5.412 + 6/7)^2");
    for(int i = 0; i<postf.length; i++){
        if(postf.tokens[i].type == TOKEN_NUMBER){
            printf("%.2f ", postf.tokens[i].value);
        }
        else {
            printf("%c ", "  -+/*^"[(int)postf.tokens[i].type]);
        }
    }
    float value = eval(postf);
    printf("\n%f\n", value);

    return 0;
}