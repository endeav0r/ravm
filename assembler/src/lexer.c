#include "lexer.h"

struct _match matches[LEXER_MATCH_TOKENS];


void lexer_tokens_delete (struct _token * token) {
    if (token->text != NULL)
        free(token->text);
    if (token->next != NULL)
        lexer_tokens_delete(token->next);
    free(token);
}


void lexer_match_set (struct _match * match, const char * text, int type) {
    strcpy(match->text, text);
    match->type = type;
}


void lexer_init () {
    lexer_match_set(&(matches[0]), "add",  TOKEN_ADD );
    lexer_match_set(&(matches[1]), "sub",  TOKEN_SUB );
    lexer_match_set(&(matches[2]), "mul",  TOKEN_MUL );
    lexer_match_set(&(matches[3]), "div",  TOKEN_DIV );
    lexer_match_set(&(matches[4]), "mod",  TOKEN_MOD );
    lexer_match_set(&(matches[5]), "mov",  TOKEN_MOV );
    lexer_match_set(&(matches[6]), "jmp",  TOKEN_JMP );
    lexer_match_set(&(matches[7]), "jz",   TOKEN_JZ  );
    lexer_match_set(&(matches[8]), "je",   TOKEN_JE  );
    lexer_match_set(&(matches[9]), "jg",   TOKEN_JG  );
    lexer_match_set(&(matches[10]), "call", TOKEN_CALL);
    lexer_match_set(&(matches[11]), "ret",  TOKEN_RET );
    lexer_match_set(&(matches[12]), "push", TOKEN_PUSH);
    lexer_match_set(&(matches[13]), "pop",  TOKEN_POP );
    lexer_match_set(&(matches[14]), "cmp",  TOKEN_CMP );
    lexer_match_set(&(matches[15]), "hlt",  TOKEN_HLT );
    lexer_match_set(&(matches[16]), "nop",  TOKEN_NOP );
    lexer_match_set(&(matches[17]), "r0",   TOKEN_R0  );
    lexer_match_set(&(matches[18]), "r1",   TOKEN_R1  );
    lexer_match_set(&(matches[19]), "r2",   TOKEN_R2  );
    lexer_match_set(&(matches[20]), "r3",   TOKEN_R3  );
    lexer_match_set(&(matches[21]), "r4",   TOKEN_R4  );
    lexer_match_set(&(matches[22]), "r5",   TOKEN_R5  );
    lexer_match_set(&(matches[23]), "r6",   TOKEN_R6  );
    lexer_match_set(&(matches[24]), "r7",   TOKEN_R7  );
    lexer_match_set(&(matches[25]), "rsp",  TOKEN_RSP );
    lexer_match_set(&(matches[26]), "rbp",  TOKEN_RBP );
    lexer_match_set(&(matches[27]), "["  ,  TOKEN_BRACKET_OPEN);
    lexer_match_set(&(matches[28]), "]"  ,  TOKEN_BRACKET_CLOSE);
    lexer_match_set(&(matches[29]), ":"  ,  TOKEN_COLON);
    lexer_match_set(&(matches[30]), "\n" ,  TOKEN_TERMINATOR);
    lexer_match_set(&(matches[31]), ","  ,  TOKEN_SEPERATOR);
    lexer_match_set(&(matches[32]), "and",  TOKEN_AND);
    lexer_match_set(&(matches[33]), "or",   TOKEN_OR);
    lexer_match_set(&(matches[34]), "xor",  TOKEN_XOR);
    lexer_match_set(&(matches[35]), "db",   TOKEN_DB);
}


void lexer_text_lower (char * text) {
    int i;
    for (i = 0; i < strlen(text); i++) {
        if ((text[i] >= 'A') && (text[i] <= 'Z'))
            text[i] += 'a' - 'A';
    }
}


int lexer_valid_decimal (const char * text) {
    int i;
    i = 0;
    if (text[0] == '-')
        i = 1;
    while (i < strlen(text)) {
        if ((text[i] < '0') || (text[i] > '9'))
            return 0;
        i++;
    }
    return 1;
}


int lexer_valid_label (const char * text) {
    int i;
    for (i = 0; i < strlen(text); i++) {
        if (   ((text[i] >= 'A') && (text[i] <= 'Z'))
            || ((text[i] >= 'a') && (text[i] <= 'z'))
            || ((text[i] >= '0') && (text[i] <= '9'))
            ||  (text[i] == '_')) {}
        else
            return 0;
    }
    return 1;
}


struct _token * lexer_token_create (const char * text, int line) {
    int i, matched;
    struct _token * token;
    char lower[LEXER_BUF_SIZE];
    
    strcpy(lower, text);
    lexer_text_lower(lower);
    
    token = (struct _token *) malloc(sizeof(struct _token));
    token->type = -1;
    token->text = (char *) malloc(strlen(text + 1));
    strcpy(token->text, text);
    token->line = line;
    token->next = NULL;
    
    matched = 0;
    for (i = 0; i < LEXER_MATCH_TOKENS; i++) {
        if (strcmp(lower, matches[i].text) == 0) {
            matched = 1;
            token->type = matches[i].type;
            break;
        }
    }
    
    if (matched == 0) {
        if (lexer_valid_decimal(text)) {
            token->type = TOKEN_CONSTANT;
            token->constant = atoi(text);
        }
        else if (lexer_valid_label(text)) {
            token->type = TOKEN_LABEL;
        }
    }
    
    return token;
}


struct _token * lexer_token_string (const char * string, int length, int line) {
    struct _token * token;
    
    token = (struct _token *) malloc(sizeof(struct _token));
    token->type = TOKEN_STRING;
    token->text = (char *) malloc(length + 1);
    strcpy(token->text, string);
    token->line = line;
    token->next = NULL;
    
    return token;
}



struct _token * lexer (const char * input) {
    struct _token * tokens = NULL;
    struct _token * token = NULL;
    struct _token * next = NULL;
    int comment_line = 0;
    char * string_begin = NULL;
    int input_i, buf_i;
    int line = 1;
    char c, ca, cp, buf[LEXER_BUF_SIZE];
    
    buf_i = 0;
    buf[buf_i] = 0;
    c = -1;
    
    for (input_i = 0; input_i < strlen(input); input_i++) {
        cp = c;
        c = input[input_i];
        if (input_i < strlen(input) - 1)
            ca = input[input_i + 1];
        else
            ca = -1;
        
        switch (c) {
            case '\n' :
                comment_line = 0;
                break;
            case '"' :
                if (string_begin == NULL)
                    string_begin = (char *) &(input[input_i + 1]);
                else if (cp != '\\') {
                    next = lexer_token_string(string_begin, 
                                              &(input[input_i]) - string_begin,
                                              line);
                    if (tokens == NULL) {
                        tokens = next;
                        token = next;
                    }
                    else {
                        token->next = next;
                        token = next;
                    }
                }
            case ';' :
                comment_line = 1;
                break;
        }
        if (comment_line) continue;
        
        switch (c) {
            case ' ' :
            case '\t' :
            case '\r' :
                break;
            case '\n' :
            case ']' :
            case '[' :
            case ':' :
            case ';' :
            case ',' :
                buf[0] = c;
                buf[1] = 0;
                if (tokens == NULL) {
                    tokens = lexer_token_create(buf, line);
                    token = tokens;
                }
                else {
                    token->next = lexer_token_create(buf, line);
                    token = token->next;
                }
                buf[0] = 0;
                break;
            default :
                buf[buf_i] = c;
                buf_i++;
                buf[buf_i] = 0;
        }
        
        if (c == '\n') line++;
        
        switch (ca) {
            case ' '  :
            case '\t' :
            case '\n' :
            case ']'  :
            case '['  :
            case ':'  :
            case ';'  :
            case ','  :
                if (buf_i > 0) {
                    if (tokens == NULL) {
                        tokens = lexer_token_create(buf, line);
                        token = tokens;
                    }
                    else {
                        token->next = lexer_token_create(buf, line);
                        token = token->next;
                    }
                    if (token->type == -1) {
                        fprintf(stderr, "unrecognized token: '%s'\n", buf);
                        exit(0);
                    }
                    buf_i = 0;
                    buf[buf_i] = 0;
                }
                break;
        }
    }
    return tokens;
}
