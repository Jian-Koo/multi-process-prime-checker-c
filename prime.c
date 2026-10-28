#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "primeFunction.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        write(2, "Usage : ./prime <filename>\n", 27);
        return 1;
    }

    /* Open the file and read only */
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        write(2, "Error : Failed to open file\n", 28);
        return 1;
    }

    int arrayLength = 0, i, j;
    char buffer[MAX_INT_SIZE];
    
    /* Read an integer from the file and convert it to an int datatype 
     * So that it can be used to create numArray, pipeArrays, pidArray, and 
     * lenArray.  */
    arrayLength = strToInt(buffer, readIntFromFile(fd, buffer));
    
    int parentToChildPipes[arrayLength][2]; /* Pipes for parent to talk to each child */
    int childToParentPipes[arrayLength][2]; /* Pipes for parent to receive from each child */
    char** numArray = (char**)malloc(sizeof(char*) * arrayLength);
    pid_t pids[arrayLength]; /* Stores pids */
    int len[arrayLength]; /* Stores length of each integer (string) */

    for (i = 0; i < arrayLength; i++)
        numArray[i] = (char*)malloc(sizeof(char) * MAX_INT_SIZE);
    
    /* Loops through both pipe arrays and pipe all pipe pairs */
    for (i = 0; i < arrayLength; i++)
    {
        if (pipe(parentToChildPipes[i]) < 0 || pipe(childToParentPipes[i]) < 0)
        {
            write(2, "Error : Failed to create pipe\n", 30);
            return 1;
        }
    }

    /* Read the rest of the integer and store them into numArray(string) 
     * Meanwhile, the length of each integer will be stored in lenArray */
    for (i = 0; i < arrayLength; i++)
        len[i] = readIntFromFile(fd, numArray[i]);
    
    for (i = 0; i < arrayLength; i++)
    {
        pids[i] = fork();
        if (pids[i] == 0) /* Child process */
        {
            /* Because a child process will inherit every pipe created prior, even those 
             * that are not prepared to it, so it has to close all of them except the pipe 
             * pairs that are ready for this particular child process. */
            for (int j = 0; j < arrayLength; j++)
            {
                if (j != i)
                {
                    close(childToParentPipes[j][0]);
                    close(childToParentPipes[j][1]);
                    close(parentToChildPipes[j][0]);
                    close(parentToChildPipes[j][1]);
                }
            }
            close(childToParentPipes[i][0]); /* Closes its reading end p[0] */
            close(parentToChildPipes[i][1]); /* Closes its writing end p[1] */

            int intToCheck, primalityResult;
            if (read(parentToChildPipes[i][0], &intToCheck, sizeof(int)) <= 0)
            {
                write(2, "Error : Failed to read integer from parent\n", 43);
                return 1;
            }
            close(parentToChildPipes[i][0]);
            primalityResult = primalityCheck(intToCheck);
            
            /* Write the primality result back to the parent process */
            write(childToParentPipes[i][1], &primalityResult, sizeof(int));
            close(childToParentPipes[i][1]);
            
            exit(0);
        }
        else if (pids[i] > 0) /* Parent process */
        {
            close(parentToChildPipes[i][0]); /* Closes its reading end p[0] */
            close(childToParentPipes[i][1]); /* Closes its writing end p[1] */
        }
        else /* Error case, fork() failed */
        {
            write(2, "Error : Failed to create child process by fork()\n", 49);
            return 1;
        }
    }

    for (i = 0; i < arrayLength; i++) 
    {
        /* Write the integer to each child following the order 
         * so child 0 will get numArray[0] and pipe[0] from both 
         * pipe arrays */
        int tempInt = strToInt(numArray[i], len[i]);
        write(parentToChildPipes[i][1], &tempInt, sizeof(int));
        close(parentToChildPipes[i][1]);
    }

    for (i = 0; i < arrayLength; i++)
    {
        /* Waiting for any finished child, FCFS mechanism */
        int status;
        pid_t finishedPid = wait(&status); 

        for (j = 0; j < arrayLength; j++)
        {
            /* The pid is not important in this program but the index of the pid in 
             * pids array is important, because that indicates the respective number 
             * in the numArray and the length of the integer string in the len array 
             * as well */
            if (pids[j] == finishedPid)
            {
                char result; 
                char childNum[MAX_INT_SIZE]; /* Leave one more space for null terminator */ 
                int childNumLen = intToStr(j, childNum);

                if (read(childToParentPipes[j][0], &result, 1) <= 0)
                {
                    write(2, "Error : Failed to read result from child\n", 41);
                    return 1;
                }
                close(childToParentPipes[j][0]);
                writeResult(childNum, childNumLen, numArray[j], len[j], result);
            }
        }
    }

    /* Clean up the resources */
    for (i = 0; i < arrayLength; i++)
        free(numArray[i]);
    free(numArray);
    numArray = NULL;
    close(fd);

    return 0;
}

