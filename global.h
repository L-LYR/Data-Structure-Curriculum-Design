#ifndef GLOBAL_H
#define GLOBAL_H

void allocMemoryPool(); // memoryPool.c
void readFromFile();
void freeAll();

int nextToken(); // tokenizer.c
void match(int tk);

void lexicalAnalysis(); // lexer.c

void syntaxAnalysis(); // parser.c

void displayAST(); // display.c

void rearrange(); // rearrange.c
#endif