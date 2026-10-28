#include <unistd.h>
#include <time.h>
#include <math.h>
#include "primeFunction.h"

/** 
 * @brief Converts a string value to an integer number
 * @param pStrValue a string value
 * @param pLength the length of the string given
 * @return an integer from the given string
 */
int strToInt(char* pStrValue, int pLength)
{
    int digit;
    int result = 0;

    for (int i = 0; i < pLength; i++)
    {
        digit = pStrValue[i] - '0';  /* '0' is equivalent to 48 (int) on ACSII table */
        result = result * 10 + digit;
    }
    return result;
}

/** 
 * @brief Convert the given integer to a string of the same literal value
 * @param pIntValue the initial integer value 
 * @param pStrValue a string that stores the converted value
 * @return the length of the string converted from the given integer
 */
int intToStr(int pIntValue, char* pStrValue)
{
    int currValue = pIntValue;
    int length = 0;
    int i;

    /* If the value is 0, just do it straight away */
    if (currValue == 0)
    {
        pStrValue[0] = '0';
        pStrValue[1] = '\0';
        return 1; /* Length is 1 straight away */
    }

    /* If it is larger than 0, we count the length of the integer first */
    while(currValue > 0)
    {
        /* This operation will take away a digit every iteration, 321 will 
        become 32 and become 3 and 0 */
        currValue /= 10; 
        length++;
    }

    pStrValue[length] = '\0'; /* Set the null terminator after the last digit */
    
    /* Reset the currValue again */
    currValue = pIntValue;
    /* Places the string character from the end to the beginning 
    Every iteration, we get the residual value after modulus 10 
    that will be the last digit of the integer value, and then we 
    divide the integer by 10 to eliminate the last digit, repeat the 
    operation */

    /* e.g., 321, get 1, insert 1 into strValue[2], become 32
    get 2, insert 2 into strValue[1], become 3 
    get 3, insert into strValue[0], become 0, end*/
    for (i = length - 1; i >= 0; i--)
    {
        pStrValue[i] = (currValue % 10) + '0'; /* '0' is equivalent to 48 (int) on ACSII table */
        currValue /= 10;
    }
    return length;
}

/** 
 * @brief Read a integer and store it into pIntDest as the destination as a string
 * @param pFD a file descriptor
 * @param pIntDest a string that will store the string read from the file
 * @return index but it is actually the length of the string value read 
 */
int readIntFromFile(int pFD, char* pIntDest)
{
    char ch;
    int index = 0;

    /* It keeps reading from the file until read() returns 0 or smaller, indicating EOF, 
    the while loop ends or when it reads a \n character, the while loops breaks */
    while (read(pFD, &ch, 1) > 0) 
    {
        if (ch >= '0' && ch <= '9')
            pIntDest[index++] = ch;
        else if (ch == '\n')
            break;
    }
    return index;
}

/** 
 * @brief Check the primality of the given integer value
 * @param pIntValue the integer value that will be tested its primality
 * @return a character '1' or '0' indicates TRUE or FALSE respectively
 */
char primalityCheck(int pIntValue)
{
    int i;
    int sqrtValue = ceil(sqrt(pIntValue));
    struct timespec time;
    time.tv_sec = 0;
    time.tv_nsec = 1000000; /* 1 ms = 1 000 000 ns */

    /* Quick dealing with simple cases */
    if (pIntValue == 2 || pIntValue == 3) return TRUE;
    if (pIntValue <= 1 || pIntValue % 2 == 0) return FALSE;

    /* Increments by 2 because all even number should have been handled  
    by the earlier if statement */
    for (i = 3; i <= sqrtValue; i+=2)
    {
        /* Also a sleeping time of 1ms for every iteration */
        nanosleep(&time, NULL);

        if (pIntValue % i == 0) return FALSE;
    }
    return TRUE;
}

/** 
 * @brief Write the result of the primality of an integer value to stdout along 
 *        with child number
 * @param pChildNum the child number (string)
 * @param pChildNumLen the length of the child number as a string
 * @param pCheckedInt the integer that has been checked its primality
 * @param pCheckIntLen the length of the integer (string)
 * @param pPrimeResult a character that indicates the primality of the given integer
 * @return void / nothing
 */
void writeResult(char* pChildNum, int pChildNumLen, 
                char* pCheckedInt, int pCheckedIntLen, char pPrimeResult)
{
    write(1, "Child ", 6);
    write(1, pChildNum, pChildNumLen);
    write(1, " completed the task: ", 22);
    write(1, pCheckedInt, pCheckedIntLen);
    write(1, " is ", 4);
    if (pPrimeResult == FALSE) 
        write(1, "NOT ", 4);
    write(1, "a prime number\n", 15);
}

