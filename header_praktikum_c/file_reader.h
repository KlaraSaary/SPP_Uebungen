/* *********************************************************************
 * IMPORTANT: This header file declares the publicly visible functions
 *            for the task. Do not modify this header, but use it as
 *            it is. Modifications of the header result in a loss of
 *            points.
 *            Please implement the declared types and funtions in a
 *            source file with the same base name.
 * ********************************************************************/

#ifndef FILE_READER_H
#define FILE_READER_H

#include "list.h"

/**
 * Opens a text file and reads the file. The text of the file is stored
 * in memory in blocks of size blockSize. The linked list with the text is
 * returned by the function. Each block should contain only complete words.
 * If a word is split by the end of the block, the last letters should be
 * moved into the next text block. Each text block must be NULL-terminated.
 * If the reading of the file fails, the program should return a meaningful
 * error message.
 */
LinkedList*
read_text_file( const char* filename, int blockSize );

#endif
