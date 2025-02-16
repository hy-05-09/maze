#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNVISITED -1

char** readMaze(int* Rm, int* Cm);
int**  allocMark(int Rm, int Cm);
void   freeMaze(char*** maze, int*** mark, int Rm, int Cm);
int    findShortest(char** maze, int** mark, int Rm, int Cm);
void   printMaze(char** maze, int Rm, int Cm, int t, int distance, bool lastFlag);
void   printMark(int** mark, int Rm, int Cm); // for debugging only(one can use this freely)


typedef struct _queue {     // linked list queue structure
    int row;
    int col;
    struct _queue* p;
} queue;
typedef queue* queuePtr;

#define MALLOC(ptr, type, size) \
 if (!((ptr)=(type)malloc(size))) {\
   fprintf(stderr, "Insufficient memory"); \
   exit(EXIT_FAILURE); \
 }

#define MAX2(x, y) ((x) > (y) ? (x) : (y))