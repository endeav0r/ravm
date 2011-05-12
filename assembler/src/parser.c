#include "parser.h"


int rules[PARSER_RULES][PARSER_RULES_NONTERMS] = {
    {TOKEN_ADD, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_REG, -1},         // RULE_ADDR
    {TOKEN_SUB, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_REG, -1},         // RULE_SUB
    {TOKEN_MUL, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_REG, -1},         // RULE_MUL
    {TOKEN_DIV, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_REG, -1},         // RULE_DIV
    {TOKEN_MOD, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_REG, -1},         // RULE_MOD
    {TOKEN_ADD, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_CONSTANT, -1},    // RULE_ADDC
    {TOKEN_MOV, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_CONSTANT, -1},    // RULE_MOVC
    {TOKEN_MOV, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_REG, -1},         // RULE_MOVR
    {TOKEN_MOV, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_BRACKET_OPEN,
                TOKEN_REG, TOKEN_BRACKET_CLOSE, -1},                // RULE_MOVL
    {TOKEN_MOV, TOKEN_BRACKET_OPEN, TOKEN_REG, TOKEN_BRACKET_CLOSE,
                TOKEN_SEPERATOR, TOKEN_REG, -1},                    // RULE_MOVS
    {TOKEN_JMP, TOKEN_LABEL, -1},                                   // RULE_JMP
    {TOKEN_JZ,  TOKEN_LABEL, -1},                                   // RULE_JZ
    {TOKEN_JE,  TOKEN_LABEL, -1},                                   // RULE_JE
    {TOKEN_JG,  TOKEN_LABEL, -1},                                   // RULE_JG
    {TOKEN_CALL, TOKEN_LABEL, -1},                                  // RULE_CALL
    {TOKEN_RET, -1},                                                // RULE_RET
    {TOKEN_PUSH, TOKEN_REG, -1},                                    // RULE_PUSH
    {TOKEN_POP, TOKEN_REG, -1},                                     // RULE_POP
    {TOKEN_CMP, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_REG, -1},         // RULE_CMPR
    {TOKEN_CMP, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_CONSTANT, -1},    // RULE_CMPC
    {TOKEN_HLT, -1},                                                // RULE_HLT
    {TOKEN_NOP, -1},                                                // RULE_NOP
    {TOKEN_LABEL, TOKEN_COLON, -1},                                 // RULE_LABEL
    {TOKEN_AND, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_REG, -1},         // RULE_ANDR
    {TOKEN_AND, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_CONSTANT, -1},    // RULE_ANDC
    {TOKEN_OR, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_REG, -1},          // RULE_ORR
    {TOKEN_OR, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_CONSTANT, -1},     // RULE_ORC
    {TOKEN_XOR, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_REG, -1},         // RULE_XORR
    {TOKEN_XOR, TOKEN_REG, TOKEN_SEPERATOR, TOKEN_CONSTANT, -1}     // RULE_XORC
};


unsigned char rule_to_assembler_op [] = {
    OP_ADDR, // RULE_ADDR
    OP_SUB,  // RULE_SUB
    OP_MUL,  // RULE_MUL
    OP_DIV,  // RULE_DIV
    OP_MOD,  // RULE_MOD
    OP_ADDC, // RULE_ADDC
    OP_MOVC, // RULE_MOVC
    OP_MOVR, // RULE_MOVR
    OP_MOVL, // RULE_MOVL
    OP_MOVS, // RULE_MOVS
    OP_JMP,  // RULE_JMP
    OP_JZ,   // RULE_JZ
    OP_JE,   // RULE_JE
    OP_JG,   // RULE_JG
    OP_CALL, // RULE_CALL
    OP_RET,  // RULE_RET
    OP_PUSH, // RULE_PUSH
    OP_POP,  // RULE_POP
    OP_CMPR, // RULE_CMPR
    OP_CMPC, // RULE_CMPC
    OP_HLT,  // RULE_HLT
    OP_NOP,  // RULE_NOP
    -1,      // RULE_LABEL
    OP_ANDR, // RULE_ANDR
    OP_ANDC, // RULE_ANDC
    OP_ORR,  // RULE_ORR
    OP_ORC,  // RULE_ORC
    OP_XORR, // RULE_XORR
    OP_XORC  // RULE_XORC
};


int token_to_assembler_reg[PARSER_REGISTERS][2] = {
    {TOKEN_R0,  (int) R0},
    {TOKEN_R1,  (int) R1},
    {TOKEN_R2,  (int) R2},
    {TOKEN_R3,  (int) R3},
    {TOKEN_R4,  (int) R4},
    {TOKEN_R5,  (int) R5},
    {TOKEN_R6,  (int) R6},
    {TOKEN_R7,  (int) R7},
    {TOKEN_RSP, (int) RSP},
    {TOKEN_RBP, (int) RBP}
};


unsigned char parser_token_to_assembler_reg (int token_reg) {
    int i;
    for (i = 0; i < PARSER_REGISTERS; i++) {
        if (token_to_assembler_reg[i][0] == token_reg)
            return (unsigned char) token_to_assembler_reg[i][1];
    }
    fprintf(stderr, "failed trying to find token register %d\n", token_reg);
    exit(0);
    return -1;
}


int rule_match (int token) {
    switch (token) {
        case TOKEN_R0 :
        case TOKEN_R1 :
        case TOKEN_R2 :
        case TOKEN_R3 :
        case TOKEN_R4 :
        case TOKEN_R5 :
        case TOKEN_R6 :
        case TOKEN_R7 :
        case TOKEN_RSP :
        case TOKEN_RBP :
            return TOKEN_REG;
    }
    return token;
}


struct _instruction * parser_instruction (int rule,
                                          struct _token * token_stack[PARSER_RULES_NONTERMS],
                                          struct _parser * parser) {
    struct _instruction * instruction;
    
    instruction = (struct _instruction *) malloc(sizeof(struct _instruction));
    instruction->next = NULL;
    instruction->op = rule_to_assembler_op[rule];
    instruction->location = parser->size;
    
    switch (rule) {
        case RULE_ADDR :
        case RULE_SUB :
        case RULE_MUL :
        case RULE_DIV :
        case RULE_MOD :
        case RULE_ANDR :
        case RULE_ORR  :
        case RULE_XORR :
        case RULE_MOVR :
        case RULE_CMPR :
            instruction->rd = parser_token_to_assembler_reg(token_stack[1]->type);
            instruction->rs = parser_token_to_assembler_reg(token_stack[3]->type);
            instruction->size = 3;
            break;
        case RULE_ADDC :
        case RULE_ANDC :
        case RULE_ORC  :
        case RULE_XORC :
        case RULE_MOVC :
        case RULE_CMPC :
            instruction->rd = parser_token_to_assembler_reg(token_stack[1]->type);
            instruction->constant = atoi(token_stack[3]->text);
            instruction->size = 6;
            break;
        case RULE_MOVL :
            instruction->rd = parser_token_to_assembler_reg(token_stack[1]->type);
            instruction->rs = parser_token_to_assembler_reg(token_stack[4]->type);
            instruction->size = 3;
            break;
        case RULE_MOVS :
            instruction->rd = parser_token_to_assembler_reg(token_stack[2]->type);
            instruction->rs = parser_token_to_assembler_reg(token_stack[5]->type);
            instruction->size = 3;
            break;
        case RULE_JMP  :
        case RULE_JZ   :
        case RULE_JE   :
        case RULE_JG   :
        case RULE_CALL :
            // oh boy labels!
            instruction->jump = label_get(&(parser->labels), token_stack[1]->text);
            instruction->size = 5;
            break;
        case RULE_PUSH :
        case RULE_POP :
            instruction->rd = parser_token_to_assembler_reg(token_stack[1]->type);
            instruction->size = 2;
            break;
        default :
            instruction->size = 1;
    }
    
    return instruction;
}


int parser_reduce (struct _parser * parser,
                   struct _token * token_stack[PARSER_RULES_NONTERMS]) {
    struct _label * label;
    int rule, term_i, match, i;
    
    for (rule = 0; rule < PARSER_RULES; rule++) {
        match = 1;
        for (term_i = 0; rules[rule][term_i] != -1; term_i++) {
            if (token_stack[term_i] == NULL) {
                match = 0;
                break;
            }
            if (rule_match(token_stack[term_i]->type) != rules[rule][term_i]) {
                match = 0;
                break;
            }
        }
        if (match) break;
    }
    
    if (match == 0) {
        fprintf(stderr, "parser error, line %d\n", token_stack[0]->line);
        i = 0;
        while (token_stack[i] != NULL) {
            fprintf(stderr, "%d\t%s\n", token_stack[i]->type, token_stack[i]->text);
            i++;
        }
        exit(0);
    }
    
    if (rule == RULE_LABEL) {
        label = label_get(&(parser->labels), token_stack[0]->text);
        label->location = parser->size;
    }
    else {
        if (parser->instructions == NULL) {
            parser->instructions = parser_instruction(rule, token_stack,
                                                      parser);
            parser->last_instruction = parser->instructions;
        }
        else {
            parser->last_instruction->next = parser_instruction(rule, token_stack,
                                                                parser);
            parser->last_instruction = parser->last_instruction->next;
        }
        parser->size += parser->last_instruction->size;
    }
    return 0;
}
            
            


int parser_parse (struct _parser * parser, struct _token * tokens) {
    int token_stack_top;
    struct _token * token;
    struct _token * token_stack[PARSER_RULES_NONTERMS];
    
    token_stack_top = 0;
    token = tokens;
    
    while (token != NULL) {
        if (token->type == TOKEN_TERMINATOR) {
            if (token_stack_top > 0) {
                token_stack[token_stack_top] = NULL;
                parser_reduce(parser, token_stack);
                token_stack_top = 0;
            }
        }
        else
            token_stack[token_stack_top++] = token;
        
        token = token->next;
    }
    
    return 0;
}
