/**************************************************************
* Class:  CSC-415
* Name: Alex Shirazi, Duy Nguyen, Vi Dao, Manuel Hernandez
* Student ID: 918794583,917131389,920353977,(unknown)
* TEAM-NAME:A-TEAM
* Project: Basic File System
*
* File: fsShell.c
*
* Description: bitmap to keep track of available logical block adresses
*
**************************************************************/
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include "fsLow.h"
#include "mfs.h"
#include "bitmap.h"

void initBitMap(int numBlocks,int pos){
    block_t *bit_map = (block_t *)calloc(1,512);
    for(int i = 0; i < 512; i++){
        if(i <1){
            bit_map->bytes[i]=1;
        }
        else{
        bit_map->bytes[i]=255;
        }
    }
    LBAwrite(bit_map,1,pos);//first block write
    pos++;
    bit_map->bytes[0]=255;
   //For bitmap 1 = free block,0 = taken
    for(int i = 2; i < 6;i++){//next four
        LBAwrite(bit_map,1,pos);
        pos++;
    }
    free(bit_map); 
}

int allocate(){
    unsigned char byte,Temp,CurrentByte;
    int LBA=0;
    block_t *bit= (block_t *)calloc(1,512);
    int STATE=0;

    for(int x = 1; x <=5; x++){
        LBAread(bit,1,x);//read through each block of the bit map
        for(int y = 0; y < 512; y++){
            byte = 128;//byte holds the bit we are currently looking at so starting off in first position in binary this is 1000 0000
            CurrentByte = bit->bytes[y];// holds the current byte we are looking through
            for(int z = 0; z < 8; z++){// going through each bit
            Temp = CurrentByte & byte;// temp holds the result if the byte is free
            if (byte == Temp){//this if statement checks if temp is holding a freebyte
                byte = ~byte;//flip bits in order to make change to currentByte, check if this how it works
                CurrentByte = CurrentByte & byte;
                z = 8;
                STATE=1;//WE CHANGED THE BYTE TIME TO EXIT OIT OF LOOP
            }
            byte=byte>>1;// shift the 1 in bytes right 1 unit
            LBA++;//update logical block address
        }
        bit->bytes[y] = CurrentByte;//sets the current byte at position Y to the result of currentByte
        if(STATE==1){// this if statement checks if we've found enough open blocks and if so will set for loop values so that we exit in this iteration
            y = 512;
        }
        
        }
        LBAwrite(bit,1,x);// writes the new reulting block of the bitmap
        if(STATE==1){// this if statement checks if we've found enough open blocks and if so will set for loop values so that we exit in this iteration
            x = 6;
        }
    }
    return LBA-1;//is the -1 correct?
}

void deallocate(int BlockNeeded){//parameters is am array of SORTED ints of the LBA needed to open up 
    int offset,CurrByte;
    unsigned char byte;
    block_t *bit= (block_t *)calloc(1,512);     
    CurrByte=0;
    unsigned char ByteVal;
    offset = 128;                                                                                                                                                                   
    int currentBlock=1+(BlockNeeded/4096);//hardcoded change later
    LBAread(bit,1,currentBlock);//increment by 1 since bitmap starts at LBA 1
 
    CurrByte = (BlockNeeded % 4096) / 8;//finds the needed byte
    offset = offset >> ( ( BlockNeeded % 4096  ) % 8);
    ByteVal = bit->bytes[CurrByte];
    bit->bytes[CurrByte]= ByteVal | offset;
    LBAwrite(bit,1,currentBlock);      
}