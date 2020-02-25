#ifndef GLOBAL_H
#define GLOBAL_H

void allocMemoryPool();
void readFromFile();

int nextToken();
void match(int tk);

void lexicalAnalysis();

void syntaxAnalysis();

void displayAST();

void rearrange();

#endif