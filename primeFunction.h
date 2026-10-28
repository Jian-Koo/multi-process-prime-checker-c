#ifndef PRIME_FUNCTION_H
#define PRIME_FUNCTION_H 

#define MAX_INT_SIZE 11
#define TRUE 1
#define FALSE 0

int strToInt(char* pStrValue, int pLength);
int intToStr(int pIntValue, char* pStrValue);
int readIntFromFile(int pFD, char* pIntDest);
char primalityCheck(int pIntValue);
void writeResult(char* pChildNum, int pChildNumLen, 
                char* pCheckedInt, int pCheckedIntLen, char pPrimeResult);

# endif

