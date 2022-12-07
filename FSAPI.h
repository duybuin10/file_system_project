/**************************************************************
* Class:  CSC-415
* Name: Alex Shirazi, Duy Nguyen, Vi Dao, Manuel Hernandez
* Student ID: 918794583,917131389,920353977,(unknown)
* TEAM-NAME:A-TEAM
* Project: Basic File System
*
* File: FSAPI.h
* Description: header file for FSAPI.C
*
**************************************************************/
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "b_io.h"
#include <dirent.h>
#include "mfs.h"

int initRoot();// inits root directory
void initBlk(int x);// inits a block of a directory

void initChildDir(int x,int y);
int childLBA(char* buf);//returns lba of the currentDirectory
int absolutePathChild(char *path);
int dirLoop(char* token,int lba);
int charCountInString(char* string,char check);
int pathCheck(char* path);
char* convertToAbsolutePath(char* path);

void DelFCB(int LBA);// Deltes an FCB
void DelFileDataBlk(int LBA);//deletes a file data block 
void DelDir(int LBA);//deletes a directory block
void setCurrentWorkingDirectory(char* path);
