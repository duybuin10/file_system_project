/**************************************************************
* Class:  CSC-415
* Name: Alex Shirazi, Duy Nguyen, Vi Dao, Manuel Hernandez
* Student ID: 918794583,917131389,920353977,(unknown)
* TEAM-NAME:A-TEAM
* Project: Basic File System
*
* File: fsBoot.c
*
* Description: Called by fsshell and contains all the calls to init the main components 
* of our file system such as the volume control block, bitmap, and root directory
**************************************************************/
#include "fsBoot.h"


int fsBoot(int argc, char *argv[]){
    char * filename;
	uint64_t volumeSize;
	uint64_t blockSize;
    int retVal;
	
	CurrentDir="/root";
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
    //Might need to use magic number around here 
	VCB* vcb;
    vcb = malloc( sizeof(vcb) );
	retVal = startPartitionSystem (filename, &volumeSize, &blockSize);
	LBAread(vcb,1,0);
	if(vcb->state != 1121){
		printf("Opened %s, Volume Size: %llu;  BlockSize: %llu; Return %d\n", filename, (ull_t)volumeSize, (ull_t)blockSize, retVal);
		printf("initing VCB\n");
    	initVCB(filename,volumeSize,blockSize);
		printf("initing free space\n");
		initBitMap(5,1);
		printf("initing root\n");
		initRoot();
		
	}
	return 0;
}