#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define MAX_STRING 256
#define MAX_TOKENS 128  // maximum (MAX_STRING / 2 - 1)

static char TOKEN_STRING[] = "()-+/*";

enum TokenType {
    TOKEN_OPR = 0,  // 0    (
    TOKEN_CPR,      // 1    )
    TOKEN_SUB,      // 2    -
    TOKEN_ADD,      // 3    +
    TOKEN_DIV,      // 4    /
    TOKEN_MUL,      // 5    *
    TOKEN_NUM,      // 6    ex. 3.14
};

typedef struct _Token {
    enum TokenType type;
    double value;
} Token;

void lexer(const char* string, Token* tokens, int* len_tokens);
void parser(Token* tokens, int len_tokens, Token* postfix);
double calculate(Token* postfix, int len_tokens);

// ------ utility functions -------
int token_typevalue(enum TokenType type);
bool token_ge(Token a, Token b);
void print_tokens(Token* tokens, int len);
// --------------------------------

int main(){
    const char test_string[] = "(2.71 - 3.14 + 4) * (5.412 + 6/7)";

    int len;

    Token tokens[MAX_TOKENS];
    lexer(test_string, tokens, &len);
    printf("Tokenized expression:           ");
    print_tokens(tokens, len); printf("\n");

    Token posfix[MAX_TOKENS];
    printf("Expression in postfix notation: ");
    parser(tokens, len, posfix);
    print_tokens(posfix, len); printf("\n");

    double result = calculate(posfix, len);
    printf("Calculated expression:          %f\n", result);

    return 0;
}

void lexer(const char* string, Token* tokens, int* len_tokens){
    // Reads a string and creates tokens from numbers and operators
    // Args: tokens as refrence, len_tokens as refrence

    char digits[MAX_STRING];
    memset(digits, 0, MAX_STRING);

    int len_digits = 0;
    int num_token = 0;
    int i_string = 0;

    for(i_string; i_string<strlen(string); i_string++){
        // Ignore spaces
        if(string[i_string] == ' ') continue;;
        // If operator
        int i = 0;
        for(i; i < 6; i++){
            if(string[i_string] == TOKEN_STRING[i]){
                if(len_digits > 0){
                    // Create token from number buffer and flush buffer
                    tokens[num_token++] = (Token) { TOKEN_NUM, atof(digits) };
                    memset(digits, 0, MAX_STRING);
                    len_digits = 0;
                }
                // Create token from operator
                tokens[num_token++] = (Token) { (enum TokenType) i, 0 };
                break;
            }
        }
        // If not operator, add to digit buffer
        if(string[i_string] != TOKEN_STRING[i]){
            digits[len_digits++] = string[i_string];
        }
    }
    // If expression ends with digit, add it to tokens
    if(strlen(digits) > 0){
        tokens[num_token++] = (Token) { TOKEN_NUM, atof(digits) };
    }

    *len_tokens = num_token;
}

void parser(Token* tokens, int len_tokens, Token* postfix){
    // Create postfix notaion from given tokens
    // Args: postfix by refrence

    Token stack[MAX_TOKENS];
    int len_stack = 0;
    int len_postfix = 0;

    for(int i_token = 0; i_token < len_tokens; i_token++){

        Token token = tokens[i_token];
        // Add to postfix if number
        if(token.type == TOKEN_NUM){
            postfix[len_postfix++] = token;
            continue;
        }
        // If token is "("
        if(token.type == TOKEN_OPR){
            stack[len_stack++] = token;
            continue;
        }
        // If token is ")" add to postfix until "("
        if(token.type == TOKEN_CPR){
            while (len_stack > 0 && stack[len_stack - 1].type != TOKEN_OPR){
                postfix[len_postfix++] = stack[--len_stack];
            }
            postfix[len_postfix++] = stack[--len_stack];
            postfix[len_postfix++] = token;
            continue;
        }
        // If new operator is < stack top empty stack until 
        // new operator is > stack top, than add to stack
        while(len_stack > 0 && token_ge(stack[len_stack - 1], token)){
            postfix[len_postfix++] = stack[--len_stack];
        }
        stack[len_stack++] = token;
    }
    // Empty stack
    while(len_stack > 0){
        postfix[len_postfix++] = stack[--len_stack];
    }
};

double calculate(Token* postfix, int len_tokens){
    // Calculates posfix expression

    double stack[MAX_TOKENS];
    int len_stack = 0;

    for(int i_postfix = 0; i_postfix<len_tokens; i_postfix++){
        Token token = postfix[i_postfix];
        // Ignore parentheses
        if(token_typevalue(token.type) == 0) continue;
        // If token is number add to stack
        if(token.type == TOKEN_NUM){
            stack[len_stack++] = token.value;
            continue;
        }
        // If token is oprator, twice from stack, do operation
        // And append back to stack
        // It's equvelent to reduce(op, stack) in other language
        double b = stack[--len_stack];
        double a = stack[--len_stack];

        switch (token.type){
        case TOKEN_ADD:
            stack[len_stack++] = a + b;
            break;
        
        case TOKEN_SUB:
            stack[len_stack++] = a - b;
            break;
        
        case TOKEN_DIV:
            stack[len_stack++] = a / b;
            break;
        
        case TOKEN_MUL:
            stack[len_stack++] = a * b;
            break;
        
        default:
            break;
        }
    }
    // Stack has been reduced to only one elemnt
    // Return that element
    return stack[0];
};

// ------ utility functions -------
bool token_ge(Token a, Token b){
    return token_typevalue(a.type) >= token_typevalue(b.type);
}

int token_typevalue(enum TokenType type){
    switch (type){
    case TOKEN_CPR:
    case TOKEN_OPR:
        return 0;

    case TOKEN_SUB:
    case TOKEN_ADD:
        return 1;

    case TOKEN_DIV:
    case TOKEN_MUL:
        return 2;

    case TOKEN_NUM:
        return 3;
    }
};

void print_tokens(Token* tokens, int len){
    for(int i = 0; i<len; i++){
        if(tokens[i].type == TOKEN_NUM){
            printf("%.2f ", tokens[i].value);
        } else {
            if(token_typevalue(tokens[i].type) > 0){
                printf("%c ", TOKEN_STRING[tokens[i].type]);
            }
        };
    }
};
// --------------------------------