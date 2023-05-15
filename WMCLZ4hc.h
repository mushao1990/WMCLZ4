//
//  WMCLZ4hc.h
//  WMCLZ4hc
//
//  Created by muser on 2023/5/15.
//  Copyright © 2023 Mac. All rights reserved.
//

#pragma once


#if defined (__cplusplus)
extern "C" {
#endif


int WMCLZ4_compressHC (const char* source, char* dest, int inputSize);
/*
WMCLZ4_compressHC :
    return : the number of bytes in compressed buffer dest
             or 0 if compression fails.
    note : destination buffer must be already allocated. 
        To avoid any problem, size it to handle worst cases situations (input data not compressible)
        Worst case size evaluation is provided by function WMCLZ4_compressBound() (see "WMCLZ4.h")
*/

int WMCLZ4_compressHC_limitedOutput (const char* source, char* dest, int inputSize, int maxOutputSize);
/*
WMCLZ4_compress_limitedOutput() :
    Compress 'inputSize' bytes from 'source' into an output buffer 'dest' of maximum size 'maxOutputSize'.
    If it cannot achieve it, compression will stop, and result of the function will be zero.
    This function never writes outside of provided output buffer.

    inputSize  : Max supported value is 1 GB
    maxOutputSize : is maximum allowed size into the destination buffer (which must be already allocated)
    return : the number of output bytes written in buffer 'dest'
             or 0 if compression fails.
*/


/* Note :
Decompression functions are provided within WMCLZ4 source code (see "WMCLZ4.h") (BSD license)
*/


/* Advanced Functions */

void* WMCLZ4_createHC (const char* inputBuffer);
int   WMCLZ4_compressHC_continue (void* WMCLZ4HC_Data, const char* source, char* dest, int inputSize);
int   WMCLZ4_compressHC_limitedOutput_continue (void* WMCLZ4HC_Data, const char* source, char* dest, int inputSize, int maxOutputSize);
char* WMCLZ4_slideInputBufferHC (void* WMCLZ4HC_Data);
int   WMCLZ4_freeHC (void* WMCLZ4HC_Data);

/* 
These functions allow the compression of dependent blocks, where each block benefits from prior 64 KB within preceding blocks.
In order to achieve this, it is necessary to start creating the WMCLZ4HC Data Structure, thanks to the function :

void* WMCLZ4_createHC (const char* inputBuffer);
The result of the function is the (void*) pointer on the WMCLZ4HC Data Structure.
This pointer will be needed in all other functions.
If the pointer returned is NULL, then the allocation has failed, and compression must be aborted.
The only parameter 'const char* inputBuffer' must, obviously, point at the beginning of input buffer.
The input buffer must be already allocated, and size at least 192KB.
'inputBuffer' will also be the 'const char* source' of the first block.

All blocks are expected to lay next to each other within the input buffer, starting from 'inputBuffer'.
To compress each block, use either WMCLZ4_compressHC_continue() or WMCLZ4_compressHC_limitedOutput_continue().
Their behavior are identical to WMCLZ4_compressHC() or WMCLZ4_compressHC_limitedOutput(),
but require the WMCLZ4HC Data Structure as their first argument, and check that each block starts right after the previous one.
If next block does not begin immediately after the previous one, the compression will fail (return 0).

When it's no longer possible to lay the next block after the previous one (not enough space left into input buffer), a call to : 
char* WMCLZ4_slideInputBufferHC(void* WMCLZ4HC_Data);
must be performed. It will typically copy the latest 64KB of input at the beginning of input buffer.
Note that, for this function to work properly, minimum size of an input buffer must be 192KB.
==> The memory position where the next input data block must start is provided as the result of the function.

Compression can then resume, using WMCLZ4_compressHC_continue() or WMCLZ4_compressHC_limitedOutput_continue(), as usual.

When compression is completed, a call to WMCLZ4_freeHC() will release the memory used by the WMCLZ4HC Data Structure.
*/


#if defined (__cplusplus)
}
#endif
