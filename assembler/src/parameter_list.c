#include "parameter_list.h"

struct _parameter_list * parameter_list_create (struct _token * tokens) {
    int bytes = 0;
    struct _parameter_list * parameter_list = NULL;
    struct _parameter_list * last = NULL;
    struct _parameter_list * next = NULL;
    struct _token * token;
    
    token = tokens;
    if ((token->type != TOKEN_STRING) && (token->type != TOKEN_CONSTANT)) {
        fprintf(stderr,
                "parameter list must start with string or word, line: %d\n",
                token->line);
        exit(0);
    }
    
    while (1) {
        switch (token->type) {
            case TOKEN_STRING :
                next = (struct _parameter_list *) malloc(sizeof(struct _parameter_list));
                next->type = PARAMETER_STRING;
                next->string = (char *) malloc(strlen(token->text) + 1);
                strcpy(next->string, token->text);
                next->next = NULL;
                bytes += strlen(token->text) + 1;
                break;
            case TOKEN_CONSTANT :
                next = (struct _parameter_list *) malloc(sizeof(struct _parameter_list));
                next->type = PARAMETER_WORD;
                next->word = strtol(token->text, NULL, 0);
                next->next = NULL;
                bytes += 4;
                break;
            default :
                fprintf(stderr, "illegal token in parameter list, line: %d\n",
                        token->line);
                exit(0);
        }
        
        if (parameter_list == NULL)
            parameter_list = next;
        else 
            last->next = next;
        last = next;
        
        token = token->next;
        if (token->type == TOKEN_SEPERATOR) token = token->next;
        else if (token->type == TOKEN_TERMINATOR) break;
        else {
            fprintf(stderr, "illegal token in parameter list, line %d\n",
                    token->line);
            exit(0);
        }
    }

    parameter_list->bytes = bytes;
    return parameter_list;

}
