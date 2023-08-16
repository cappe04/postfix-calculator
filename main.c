#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define MAX_STRING 256
#define MAX_TOKENS 128  // maximum (MAX_STRING / 2 - 1)

static char TOKEN_STRING[] = "()-+/*";

enum TokenType {
    TOKEN_OPR, // 0     (
    TOKEN_CPR, // 1     )
    TOKEN_SUB, // 2     -
    TOKEN_ADD, // 3     +
    TOKEN_DIV, // 4     /
    TOKEN_MUL, // 5     *
    TOKEN_NUM, // 6     ex. 3.14
};

typedef struct _Token {
    enum TokenType type;
    double value;
} Token;

void lexer(const char* string, Token* tokens, int* len_tokens);
void parser(Token* tokens, int len_tokens, Token* postfix);

int token_typevalue(enum TokenType type);
bool token_ge(Token a, Token b);

void print_tokens(Token* tokens, int len);

int main(){

    const char test_string[] = "((2.71 + 1.61) - 3.14 * (1 / 137)) + 1.6";
    int test_len = strlen(test_string);

    Token tokens[MAX_TOKENS];
    int len;

    lexer(test_string, tokens, &len);
    print_tokens(tokens, len);
    printf("    %d\n", len);

    Token posfix[MAX_TOKENS];

    parser(tokens, len, posfix);
    printf("\n");

    print_tokens(posfix, len);

    return 0;
}

void lexer(const char* string, Token* tokens, int* len_tokens){

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
                    tokens[num_token++] = (Token) { TOKEN_NUM, atof(digits) };
                    memset(digits, 0, MAX_STRING);
                    len_digits = 0;
                }

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