/**************************************************************
* Class:  CSC-415
* Name: Alex Shirazi, Duy Nguyen, Vi Dao, Manuel Hernandez
* Student ID: 918794583,917131389,920353977,(unknown)
* TEAM-NAME:A-TEAM
* Project: Basic File System
*
* File: bitmap.h
*
* Description: header file for bitmap
*

*
**************************************************************/
#include "mfs.h"

typedef struct block
{
	unsigned char bytes[512];//128 int = 512 bytes
} block_t;
void initBitMap(int numBlocks,int pos);
int  allocate();//allocates free block and returns the LBA
void deallocate(int BlockNeeded);//takes in the LBA and deallocates the block