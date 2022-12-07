``````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````/**************************************************************
* Class:  CSC-415
* Name: Professor Bierman
* Student ID: N/A
* Project: Basic File System
*
* File: fsLowDemo.c
*
* Description: This is a demo to show how to use the fsLow
* 	routines.
*
**************************************************************/


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include "fsLow.h"
#include "mfs.h"
#include "VCB.h"
#include "bitmap.h"
//**************** move to new header file ************

int main (int argc, char *argv[])
	{	
	char * filename;
	uint64_t volumeSize;
	uint64_t blockSize;
    int retVal;
    
	if (argc > 3)
		{
		filename = argv[1];
		volumeSize = atoll (argv[2]);
		blockSize = atoll (argv[3]);
		}
	else
		{
		printf ("Usage: fsLowDriver volumeFileName volumeSize blockSize\n");
		return -1;
		}
		
	retVal = startPartitionSystem (filename, &volumeSize, &blockSize);	
	printf("Opened %s, Volume Size: %llu;  BlockSize: %llu; Return %d\n", filename, (ull_t)volumeSize, (ull_t)blockSize, retVal);
	initVCB(filename,volumeSize,blockSize);

	initBitMap(5,1);
	printf("init Bit Map successfully!\n");

	//********** start of vcb initialization (needs to be moved to init() function later) ********
	//********************************************************************************************
	/*
	struct VCB testVcb = {"VCB", MAGICNUMBER, volumeSize/blockSize, 2, 3 ,volumeSize, volumeSize};//creating vcb
	struct VCB* vPtr;//vcb pointer
	char* buf = malloc(blockSize);//create some memory (block size 512)
	memset(buf,0,blockSize);

	//displaying empty memory in buf
	printf("***** before ******** \n");
	for(int i = 0; i < sizeof(testVcb); i++){
		printf("%02X ",buf[i]);
	}
	printf("\n");

	//copying vcb struct to empty buf 
	memcpy(buf, (const unsigned char*)&testVcb, sizeof(testVcb));

	//displaying buf in hex after copy
	printf("***** after ******** \n");
	for(int i = 0; i < sizeof(testVcb); i++){
		printf("%02X ",buf[i]);
	}
	printf("\n");

	//writing to 'disk'
	int writeS;
	writeS = LBAwrite(buf, 1, 0);

	//accessing data from buf using a vcb pointer (cast buf as vcb pointer)
	vPtr = (struct VCB*) buf;
	printf("***** title: %s\n", vPtr->title);
	printf("***** magic num: %d\n",vPtr->magicNumber);
	printf("********** size: %d\n", vPtr->numberOfBlocks);
	printf("****** freeSpace lba: %d\n", vPtr->LBA_of_freeSpace);


	//testing LBAread with saved struct
	char* buf2 = malloc(blockSize);
	memset(buf2, 0, blockSize);

	LBAread(buf2, 1, 0);//same position written	
	struct VCB* vPtr2;
	vPtr2 = (struct VCB*) buf2;//testing read data
	printf("####### title: %s\n", vPtr2->title);
	printf("######## magicNum: %d\n", vPtr2->magicNumber);
	printf("######## size %d\n", vPtr2->numberOfBlocks);


	*/
	closePartitionSystem();
	return 0;	
	}
	
