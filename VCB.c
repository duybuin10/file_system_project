/**************************************************************
* Class:  CSC-415
* Name: Alex Shirazi, Duy Nguyen, Vi Dao, Manuel Hernandez
* Student ID: 918794583,917131389,920353977,(unknown)
* TEAM-NAME:A-TEAM
* Project: Basic File System
*
* File: VCB.c
*
* Description: VCB.c initiated the volume control block with arguements passed in by the user
*
**************************************************************/
#include "VCB.h"

void initVCB(char * filename,uint64_t volumeSize,uint64_t blockSize){
    VCB* vcb;
    int MAGICNUMBER = 314156;
    int NumBlocksVCB= numblocks(volumeSize,blockSize);
    vcb = malloc(sizeof(vcb));
    strcpy(vcb->header,"Volume Control Block");
    strcpy(vcb->title,filename);
    vcb->magicNumber=MAGICNUMBER;
    vcb->VolSize = volumeSize;
    vcb->BlockSize = blockSize;
    vcb->FSBlock=1;
    vcb->RootBlock=6;
    vcb->state = 1121;
    vcb->numberOfBlocks=volumeSize/blockSize;
    vcb->NumBlocks=volumeSize/blockSize;
    LBAwrite(vcb,1,0);
}
int numblocks(int VCBSize,int BlockSize){
    uint64_t NumBlks= VCBSize/BlockSize;
    if(VCBSize%BlockSize>0){
        NumBlks+=1;
    }
    return NumBlks;

}

