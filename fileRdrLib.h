#ifndef fileRdrLib_h
#define fileRdrLib_h

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <errno.h>

struct oPut{
  char f1[200];
  char f2[200];
  double val;
  struct oPut *next;
};

//List of words within file
struct lWords{
  char  word[100];
  double freq;
  struct lWords *next;
};

//List of files wihtin directory
struct lFiles{
  char  fileName[200];
  double totalWrds;
  struct lWords *headWord;
  struct lFiles *next;
};

struct arg_addWord{
  struct lFiles *head;
  char* word;
};
struct arg_addFile{
  struct lFiles *head;
  char *fileName;
};

//struct oPut* op = NULL;

void *safeMalc(size_t size);

//Math
void meanCons(struct lFiles *headA, struct lFiles *headB);

void prnOpt(struct oPut*head);

//LLoLL ops
//void addFile(struct lFiles **head, char *fileName);
void* addFile(void* args);

//void addWord(struct lFiles *head, char* word);
void* addWord(void* args);

void readLLLL(struct lFiles *currF);

//void incFreq(struct lWords this, int freq);

//fileRdr
void directHndlr(char* direcName);
void fileHndlr(struct stat fileinfo, struct dirent *dp, DIR* dirp);

#endif
